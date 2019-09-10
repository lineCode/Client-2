// findmotionwindow.cpp
//

///// Includes /////

#include "monocleclient/findmotionwindow.h"

#include <boost/algorithm/string.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <QImage>
#include <QItemDelegate>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QSettings>
#include <utility/utility.hpp>

#include "monocleclient/connection.h"
#include "monocleclient/device.h"
#include "monocleclient/exportwindow.h"
#include "monocleclient/h264decoder.h"
#include "monocleclient/h265decoder.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/mjpegdecoder.h"
#include "monocleclient/mpeg4decoder.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class ImageItemDelegate : public QItemDelegate
{
 public:

  virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
  {
    QVariant data = index.data(Qt::DecorationRole);
    if (!data.isNull())
    {
      const QPixmap pixmap = data.value<QPixmap>();
      painter->drawPixmap(option.rect, pixmap, pixmap.rect());
    }
  }
};

///// Globals /////

boost::optional<int> sensitivity;

///// Methods /////

FindMotionWindow::FindMotionWindow(QWidget* parent, const QImage& image, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingTrack>& track, const QVector4D& colour, const uint64_t starttime, const uint64_t endtime, const QRectF& rect) :
  QDialog(parent),
  device_(device),
  recording_(recording),
  track_(track)
{
  ui_.setupUi(this);

  // We do this out of order because otherwise we come before FindMotionPlaybackWidget::initialiseGL
  ui_.datetimestart->setDateTime(QDateTime::fromMSecsSinceEpoch(starttime, Qt::UTC));
  ui_.datetimeend->setDateTime(QDateTime::fromMSecsSinceEpoch(endtime, Qt::UTC));

  connect(recording_.data(), &Recording::TrackAdded, this, &FindMotionWindow::TrackAdded);
  connect(recording_.data(), &Recording::TrackRemoved, this, &FindMotionWindow::TrackRemoved);
  connect(ui_.datetimestart, &QDateTimeEdit::dateTimeChanged, this, &FindMotionWindow::StartDateTimeChanged);
  connect(ui_.datetimeend, &QDateTimeEdit::dateTimeChanged, this, &FindMotionWindow::EndDateTimeChanged);

  ui_.videowidget->SetImage(image);
  ui_.videowidget->SetSelectedRect(rect);
  ui_.playbackwidget->SetTrack(track);
  ui_.playbackwidget->SetColour(colour);

  ui_.tableresults->setItemDelegateForColumn(0, new ImageItemDelegate());

  // Track
  for (const QSharedPointer<RecordingTrack>& track : recording->GetTracks())
  {
    if (track->GetTrackType() != monocle::TrackType::Video)
    {

      continue;
    }
    ui_.combotracks->addItem(track->GetDescription(), track->GetId());
  }

  for (int i = 0; i < ui_.combotracks->count(); ++i)
  {
    if (ui_.combotracks->itemData(0, Qt::UserRole) == track->GetId())
    {
      ui_.combotracks->setCurrentIndex(i);
      break;
    }
  }

  // Sensitivity
  if (!sensitivity.is_initialized())
  {
    sensitivity = ui_.slidersensitivity->maximum() / 2;

  }
  ui_.slidersensitivity->setValue(*sensitivity);
  on_slidersensitivity_valueChanged(0);

  on_buttonsearch_clicked();
}

FindMotionWindow::~FindMotionWindow()
{
  if (connection_)
  {
    if (findmotiontoken_.is_initialized())
    {
      connection_->DestroyFindMotion(*findmotiontoken_);
      findmotiontoken_.reset();
    }

    if (streamtoken_.is_initialized())
    {
      connection_->DestroyStream(*streamtoken_, [](const std::chrono::steady_clock::duration latency, const monocle::client::DESTROYSTREAMRESPONSE&) {});
      streamtoken_.reset();
    }
    connection_->Destroy();
    connection_.reset();
  }
  sensitivity = ui_.slidersensitivity->value();
}

FindMotionPlaybackWidget* FindMotionWindow::GetPlaybackWidget() const
{
  return ui_.playbackwidget;
}

FindMotionVideoWidget* FindMotionWindow::GetVideoWidget() const
{
  return ui_.videowidget;
}

void FindMotionWindow::SetPaused(const bool paused)
{
  ui_.videowidget->SetPaused(paused);
}

void FindMotionWindow::Pause(const boost::optional<uint64_t>& time)
{
  SetPaused(true);
  if (streamtoken_.is_initialized())
  {
    connection_->ControlStreamPause(*streamtoken_, time);

  }
}

void FindMotionWindow::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  if (!streamtoken_.is_initialized())
  {

    return;
  }

  ResetDecoders();
  connection_->ControlStream(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex(), true, true, true, time + device_->GetTimeOffset(), boost::none, numframes);
  if (numframes.is_initialized() && ((*numframes == 0) || (*numframes == 1))) // Is this an effectively pause request...
  {
    controlstreamendcallback_ = [this](const uint64_t playrequestindex, const monocle::ErrorCode err)
    {
      if (ui_.videowidget->GetPlayRequestIndex() != playrequestindex)
      {
    
        return;
      }
    
      if (err != monocle::ErrorCode::Success)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Play failed");
        return;
      }
    
      ui_.videowidget->imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { ui_.videowidget->cache_.push_back(imagebuffer); });
    
      std::vector<ImageBuffer>::const_iterator imagebuffer = ui_.videowidget->cache_.end();
      for (std::vector<ImageBuffer>::const_iterator i = ui_.videowidget->cache_.begin(); i != ui_.videowidget->cache_.end(); ++i)
      {
        if (i->playrequestindex_ != playrequestindex)
        {
    
          continue;
        }
    
        if (imagebuffer == ui_.videowidget->cache_.end())
        {
          imagebuffer = i;
    
        }
        else
        {
          if (i->sequencenum_ > imagebuffer->sequencenum_)
          {
            imagebuffer = i;
    
          }
        }
      }
    
      if (imagebuffer == ui_.videowidget->cache_.end())
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Unable to find frame");
        return;
      }
    
      ui_.videowidget->WriteFrame(*imagebuffer);
    };
    ui_.videowidget->paused_ = true;
  }
  else
  {
    ui_.videowidget->paused_ = false;

  }
}

void FindMotionWindow::Stop()
{
  if (!streamtoken_.is_initialized())
  {

    return;
  }

  ui_.videowidget->SetPaused(false);
  ResetDecoders();
  connection_->ControlStreamLive(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex());
}

void FindMotionWindow::FrameStep(const bool forwards)
{
  ui_.videowidget->paused_ = true;
  std::vector<ImageBuffer>::iterator imagebuffer = ui_.videowidget->cache_.end();
  for (std::vector<ImageBuffer>::iterator i = ui_.videowidget->cache_.begin(); i != ui_.videowidget->cache_.end(); ++i)
  {
    if (i->playrequestindex_ != ui_.videowidget->GetPlayRequestIndex())
    {

      continue;
    }

    if (forwards)
    {
      if ((i->sequencenum_ > ui_.videowidget->sequencenum_) && ((imagebuffer == ui_.videowidget->cache_.end()) || ((imagebuffer != ui_.videowidget->cache_.end()) && (i->sequencenum_ < imagebuffer->sequencenum_))))
      {
        imagebuffer = i;

      }
    }
    else
    {
      if ((i->sequencenum_ < ui_.videowidget->sequencenum_) && ((imagebuffer == ui_.videowidget->cache_.end()) || ((imagebuffer != ui_.videowidget->cache_.end()) && (i->sequencenum_ > imagebuffer->sequencenum_))))
      {
        imagebuffer = i;

      }
    }
  }

  if (imagebuffer == ui_.videowidget->cache_.end())
  {
    controlstreamendcallback_ = [this, forwards](const uint64_t playrequestindex, const monocle::ErrorCode error)
    {
      if (ui_.videowidget->GetPlayRequestIndex() != playrequestindex)
      {

        return;
      }

      if (error != monocle::ErrorCode::Success)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Play failed");
        return;
      }

      ui_.videowidget->imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { ui_.videowidget->cache_.push_back(imagebuffer); });

      // Find the previous/next frame to go to
      std::vector<ImageBuffer>::const_iterator imagebuffer = ui_.videowidget->cache_.end();
      for (std::vector<ImageBuffer>::const_iterator i = ui_.videowidget->cache_.begin(); i != ui_.videowidget->cache_.end(); ++i)
      {
        if (i->playrequestindex_ != playrequestindex)
        {

          continue;
        }

        // Ignore all frames after or before in the opposite direction to what we are looking for
        if (forwards)
        {
          if (i->sequencenum_ <= ui_.videowidget->sequencenum_)
          {

            continue;
          }
        }
        else
        {
          if (i->sequencenum_ >= ui_.videowidget->sequencenum_)
          {

            continue;
          }
        }

        if (imagebuffer == ui_.videowidget->cache_.end())
        {
          imagebuffer = i;

        }
        else
        {
          if (forwards)
          {
            if (i->sequencenum_ < imagebuffer->sequencenum_)
            {
              imagebuffer = i;

            }
          }
          else
          {
            if (i->sequencenum_ > imagebuffer->sequencenum_)
            {
              imagebuffer = i;

            }
          }
        }
      }

      if (imagebuffer == ui_.videowidget->cache_.end())
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Unable to find frame");
        return;
      }

      ui_.videowidget->WriteFrame(*imagebuffer);
    };

    if (streamtoken_.is_initialized())
    {
      ResetDecoders();
      connection_->ControlStreamFrameStep(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex(), forwards, ui_.videowidget->sequencenum_);
    }
  }
  else
  {
    ui_.videowidget->WriteFrame(*imagebuffer);

  }
}

void FindMotionWindow::ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata)
{
  FindMotionWindow* videoview = reinterpret_cast<FindMotionWindow*>(callbackdata);
  if (videoview->controlstreamendcallback_)
  {
    videoview->controlstreamendcallback_(playrequestindex, error);

  }
}

void FindMotionWindow::H265Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata)
{
  FindMotionWindow* findmotionwindow = reinterpret_cast<FindMotionWindow*>(callbackdata);
  if (findmotionwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<H265Decoder> >::iterator h265decoder = std::find_if(findmotionwindow->h265decoders_.begin(), findmotionwindow->h265decoders_.end(), [codecindex](std::unique_ptr<H265Decoder>& h265decoder) { return (h265decoder->GetId() == codecindex); });
  if (h265decoder == findmotionwindow->h265decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*h265decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findmotionwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_)
  {
    if (findmotionwindow->GetImageQueue().write_available())
    {
      findmotionwindow->GetImageQueue().push(imagebuffer);
      findmotionwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindMotionWindow::H264Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata)
{
  FindMotionWindow* findmotionwindow = reinterpret_cast<FindMotionWindow*>(callbackdata);
  if (findmotionwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<H264Decoder> >::iterator h264decoder = std::find_if(findmotionwindow->h264decoders_.begin(), findmotionwindow->h264decoders_.end(), [codecindex](std::unique_ptr<H264Decoder>& h264decoder) { return (h264decoder->GetId() == codecindex); });
  if (h264decoder == findmotionwindow->h264decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*h264decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findmotionwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_)
  {
    if (findmotionwindow->GetImageQueue().write_available())
    {
      findmotionwindow->GetImageQueue().push(imagebuffer);
      findmotionwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindMotionWindow::MetadataCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{

}

void FindMotionWindow::JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata)
{
  FindMotionWindow* findmotionwindow = reinterpret_cast<FindMotionWindow*>(callbackdata);
  if (findmotionwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  if (!findmotionwindow->mjpegdecoder_)
  {
    findmotionwindow->mjpegdecoder_ = std::make_unique<MJpegDecoder>(codecindex, findmotionwindow->device_->GetPublicKey()); // Id doesn't matter because we always ignore it from this point onwards
    if (findmotionwindow->mjpegdecoder_->Init())
    {

      return;
    }
  }

  ImageBuffer imagebuffer = findmotionwindow->mjpegdecoder_->Decode(playrequestindex, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findmotionwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_)
  {
    if (findmotionwindow->GetImageQueue().write_available())
    {
      findmotionwindow->GetImageQueue().push(imagebuffer);
      findmotionwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindMotionWindow::MPEG4Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  FindMotionWindow* findmotionwindow = reinterpret_cast<FindMotionWindow*>(callbackdata);
  if (findmotionwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<MPEG4Decoder> >::iterator mpeg4decoder = std::find_if(findmotionwindow->mpeg4decoders_.begin(), findmotionwindow->mpeg4decoders_.end(), [codecindex](std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == codecindex); });
  if (mpeg4decoder == findmotionwindow->mpeg4decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*mpeg4decoder)->Decode(playrequestindex, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findmotionwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_)
  {
    if (findmotionwindow->GetImageQueue().write_available())
    {
      findmotionwindow->GetImageQueue().push(imagebuffer);
      findmotionwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindMotionWindow::NewCodecIndexCallback(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata)
{
  FindMotionWindow* findmotionwindow = reinterpret_cast<FindMotionWindow*>(callbackdata);
  findmotionwindow->AddCodecIndex(monocle::CODECINDEX(id, codec, parameters, timestamp));
}

void FindMotionWindow::AddCodecIndex(const monocle::CODECINDEX& codecindex)
{
  std::vector<std::string> parameterssplit;
  boost::split(parameterssplit, codecindex.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);

  if (codecindex.codec_ == monocle::Codec::MJPEG)
  {
    // MJPEG sorts itself out in the callback

  }
  else if (codecindex.codec_ == monocle::Codec::H265)
  {
    std::unique_ptr<H265Decoder> h265decoder = std::make_unique<H265Decoder>(codecindex.id_, device_->GetPublicKey());
    const DECODERERROR error = h265decoder->Init(parameterssplit);
    if (error)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "H265Decoder failed to initialise");
      return;
    }
    h265decoders_.emplace_back(std::move(h265decoder));
  }
  else if (codecindex.codec_ == monocle::Codec::H264)
  {
    std::unique_ptr<H264Decoder> h264decoder = std::make_unique<H264Decoder>(codecindex.id_, device_->GetPublicKey());
    const DECODERERROR error = h264decoder->Init(parameterssplit);
    if (error)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "H264Decoder failed to initialise");
      return;
    }
    h264decoders_.emplace_back(std::move(h264decoder));
  }
  else if (codecindex.codec_ == monocle::Codec::MPEG4)
  {
    std::unique_ptr<MPEG4Decoder> mpeg4decoder = std::make_unique<MPEG4Decoder>(codecindex.id_, device_->GetPublicKey());
    const DECODERERROR error = mpeg4decoder->Init(parameterssplit);
    if (error)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "MPEG4Decoder failed to initialise");
      return;
    }
    mpeg4decoders_.emplace_back(std::move(mpeg4decoder));
  }
}

void FindMotionWindow::DestroyDecoders()
{
  if (mjpegdecoder_)
  {
    mjpegdecoder_->Destroy();
    mjpegdecoder_.reset();
  }

  for (std::unique_ptr<H265Decoder>& h265decoder : h265decoders_)
  {
    h265decoder->Destroy();

  }
  h265decoders_.clear();

  for (std::unique_ptr<H264Decoder>& h264decoder : h264decoders_)
  {
    h264decoder->Destroy();

  }
  h264decoders_.clear();

  for (std::unique_ptr<MPEG4Decoder>& mpeg4decoder : mpeg4decoders_)
  {
    mpeg4decoder->Destroy();

  }
  mpeg4decoders_.clear();
}

void FindMotionWindow::ResetDecoders()
{
  for (std::unique_ptr<H264Decoder>& h264decoder : h264decoders_)
  {
    h264decoder->Reset();

  }

  for (std::unique_ptr<H265Decoder>& h265decoder : h265decoders_)
  {
    h265decoder->Reset();

  }

  for (std::unique_ptr<MPEG4Decoder>& mpeg4decoder : mpeg4decoders_)
  {
    mpeg4decoder->Reset();

  }
}

void FindMotionWindow::TrackAdded(const QSharedPointer<RecordingTrack>& track)
{
  if (track->GetTrackType() != monocle::TrackType::Video)
  {

    return;
  }
  ui_.combotracks->addItem(track->GetDescription(), track->GetId());
}

void FindMotionWindow::TrackRemoved(const uint32_t trackid)
{
  const int row = ui_.combotracks->findData(trackid);
  if (row == -1)
  {

    return;
  }
  ui_.combotracks->removeItem(row);
}

void FindMotionWindow::StartDateTimeChanged(const QDateTime& datetime)
{
  ui_.datetimeend->setMinimumDateTime(datetime);
  ui_.playbackwidget->SetExportStartTime(datetime.toMSecsSinceEpoch(), true);
}

void FindMotionWindow::EndDateTimeChanged(const QDateTime& datetime)
{
  ui_.datetimestart->setMaximumDateTime(datetime);
  ui_.playbackwidget->SetExportEndTime(datetime.toMSecsSinceEpoch(), true);
}

void FindMotionWindow::FindMotionEnd(const uint64_t token, const uint64_t ret)
{
  if (!findmotiontoken_.is_initialized() || (*findmotiontoken_ != token))
  {

    return;
  }

  ui_.progressbar->setValue(100);

  if (ret)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Find Motion failed: ") + QString::number(ret), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();

  }
}

void FindMotionWindow::FindMotionProgress(const uint64_t token, const float progress)
{
  ui_.progressbar->setValue(100.0f * progress);

}

void FindMotionWindow::FindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end)
{
  if (!findmotiontoken_.is_initialized() || (*findmotiontoken_ != token))
  {

    return;
  }

  getsnapshotconnections_.emplace_back(connection_->GetSnapshot(recording_->GetToken(), track_->GetId(), start, [this, start](const std::chrono::steady_clock::duration latency, const monocle::client::GETSNAPSHOTRESPONSE& getsnapshotresponse)
  {
    if (getsnapshotresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "Failed to get snapshot: " + QString::fromStdString(getsnapshotresponse.GetErrorText()));
      return;
    }

    QImage image;
    if (!image.loadFromData(getsnapshotresponse.data_.data(), static_cast<int>(getsnapshotresponse.data_.size()), "png"))
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "Failed to load snapshot");
      return;
    }

    for (int i = 0; i < ui_.tableresults->rowCount(); ++i)
    {
      if (ui_.tableresults->item(i, 1)->data(Qt::UserRole).toULongLong() == start)
      {
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setData(Qt::DecorationRole, QPixmap::fromImage(image));
        ui_.tableresults->setItem(i, 0, item);
        break;
      }
    }
  }));

  const int row = ui_.tableresults->rowCount();
  ui_.tableresults->insertRow(row);

  QTableWidgetItem* item = new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(start, Qt::UTC).toString());
  item->setData(Qt::UserRole, static_cast<qulonglong>(start)); // The start time is assume to be unique, which is a guarantee by the server
  ui_.tableresults->setItem(row, 1, item);

  ui_.playbackwidget->FindMotionResult(start, end);
}

void FindMotionWindow::on_buttonsearch_clicked()
{
  // Swap out the track playbackwidget indices
  const uint32_t trackid = ui_.combotracks->currentData().toUInt();
  const QSharedPointer<RecordingTrack> track = recording_->GetTrack(trackid);
  if ((track == nullptr) || (track->GetTrackType() != monocle::TrackType::Video))
  {

    return;
  }

  ui_.playbackwidget->SetTrack(track);

  // Clear up previous attempts
  connect_.Close();
  getauthenticatenonce_.Close();
  authenticate_.Close();
  createstream_.Close();
  createfindmotion_.Close();
  if (connection_)
  {
    connection_->Destroy();

  }

  ui_.tableresults->setRowCount(0);
  ui_.progressbar->setValue(0);

  // Kick off
  const std::string username = device_->GetUsername().toStdString();
  const std::string password = device_->GetPassword().toStdString();
  connection_ = boost::make_shared<Connection>(MainWindow::Instance()->GetGUIIOService(), device_->GetProxyParams(), device_->GetAddress(), device_->GetPort());
  connect(connection_.get(), &Connection::SignalFindMotionEnd, this, &FindMotionWindow::FindMotionEnd);
  connect(connection_.get(), &Connection::SignalFindMotionProgress, this, &FindMotionWindow::FindMotionProgress);
  connect(connection_.get(), &Connection::SignalFindMotionResult, this, &FindMotionWindow::FindMotionResult);
  streamtoken_.reset();
  findmotiontoken_.reset();
  connect_ = connection_->Connect([this, username, password](const boost::system::error_code& err)
  {
    if (err)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to connect"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    getauthenticatenonce_ = connection_->GetAuthenticationNonce([this, username, password](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
    {
      if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to get authentication nonce: ") + QString::fromStdString(getauthenticationnonceresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }

      const std::string clientnonce = utility::GenerateRandomString(32);
      authenticate_ = connection_->Authenticate(username, clientnonce, monocle::AuthenticateDigest(username, password, getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
      {
        if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to authenticate: ") + QString::fromStdString(authenticateresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          return;
        }

        createstream_ = connection_->CreateStream(recording_->GetToken(), track_->GetId(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
        {
          if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
          {
            QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to create stream: ") + QString::fromStdString(createstreamresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
            return;
          }

          streamtoken_ = createstreamresponse.token_;

          DestroyDecoders();
          for (const monocle::CODECINDEX& codecindex : createstreamresponse.codecindices_)
          {
            AddCodecIndex(codecindex);

          }

          const QRectF selectedrect = ui_.videowidget->GetSelectedRect();
          const float inverse = static_cast<float>(ui_.slidersensitivity->maximum() - ui_.slidersensitivity->value());
          const float sense = 0.5 + (0.002f * inverse * inverse);
          createfindmotion_ = connection_->CreateFindMotion(recording_->GetToken(), track_->GetId(), ui_.datetimestart->dateTime().toMSecsSinceEpoch(), ui_.datetimeend->dateTime().toMSecsSinceEpoch(), selectedrect.x(), selectedrect.y(), selectedrect.width(), selectedrect.height(), sense, ui_.checkfastsearch->isChecked(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::CREATEFINDMOTIONRESPONSE& createfindmotionresponse)
          {
            if (createfindmotionresponse.GetErrorCode() != monocle::ErrorCode::Success)
            {
              QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to create find motion detector: ") + QString::fromStdString(createfindmotionresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
              return;
            }

            std::for_each(getsnapshotconnections_.begin(), getsnapshotconnections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
            getsnapshotconnections_.clear();
            findmotiontoken_ = createfindmotionresponse.token_;
          });
        }, FindMotionWindow::ControlStreamEnd, FindMotionWindow::H265Callback, FindMotionWindow::H264Callback, FindMotionWindow::MetadataCallback, FindMotionWindow::JPEGCallback, FindMotionWindow::MPEG4Callback, FindMotionWindow::NewCodecIndexCallback, this);
      });
    });
  });
}

void FindMotionWindow::on_tableresults_clicked(QModelIndex)
{
  if (!connection_ || !streamtoken_.is_initialized())
  {

    return;
  }

  const int row = ui_.tableresults->currentRow();
  if (row == -1)
  {

    return;
  }

  // Begin playing at this new time
  QTableWidgetItem* item = ui_.tableresults->item(row, 1);
  if (item == nullptr)
  {

    return;
  }

  const uint64_t time = item->data(Qt::UserRole).toULongLong();
  if (ui_.playbackwidget->IsPaused())
  {
    Play(time, 1);
    ui_.videowidget->playmarkertime_ = time + GetDevice()->GetTimeOffset();
    ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
  }
  else
  {
    Play(time, boost::none);
    ui_.videowidget->playmarkertime_ = time + device_->GetTimeOffset();
    ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
  }
}

void FindMotionWindow::on_buttonframestepbackwards_clicked()
{
  if (ui_.videowidget->IsPaused())
  {
    FrameStep(false);

  }
  else
  {
    Pause(ui_.videowidget->time_);

  }
}

void FindMotionWindow::on_buttonplay_clicked()
{
  Play(ui_.videowidget->time_ - device_->GetTimeOffset(), boost::none);
  ui_.videowidget->playmarkertime_ = ui_.videowidget->time_;
  ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
}

void FindMotionWindow::on_buttonpause_clicked()
{
  ui_.videowidget->SetPaused(true);
  if (streamtoken_.is_initialized() && connection_)
  {
    connection_->ControlStreamPause(*streamtoken_, ui_.videowidget->time_);
    
  }
}

void FindMotionWindow::on_buttonstop_clicked()
{
  ui_.videowidget->playmarkertime_ = ui_.playbackwidget->endtime_ + device_->GetTimeOffset();
  ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
  ui_.videowidget->SetPaused(false);
  connection_->ControlStreamLive(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex());

  ui_.playbackwidget->makeCurrent();
  ui_.playbackwidget->UpdateRecordingBlocks();
  ui_.playbackwidget->doneCurrent();
}

void FindMotionWindow::on_buttonframestepforwards_clicked()
{
  if (ui_.videowidget->IsPaused())
  {
    FrameStep(true);

  }
  else
  {
    Pause(ui_.videowidget->time_);

  }
}

void FindMotionWindow::on_slidersensitivity_valueChanged(int)
{
  ui_.labelsensitivity->setText("Sensitivity: " + QString::number(ui_.slidersensitivity->value()));

}

void FindMotionWindow::on_buttonzoomin_clicked()
{
  ui_.playbackwidget->ZoomIn(width() / 2);
  ui_.playbackwidget->makeCurrent();
  ui_.playbackwidget->UpdateRecordingBlocks();
  ui_.playbackwidget->doneCurrent();
}

void FindMotionWindow::on_buttonzoomout_clicked()
{
  ui_.playbackwidget->ZoomOut(width() / 2);
  ui_.playbackwidget->makeCurrent();
  ui_.playbackwidget->UpdateRecordingBlocks();
  ui_.playbackwidget->doneCurrent();
}

void FindMotionWindow::on_buttonexport_clicked()
{
  ExportWindow(MainWindow::Instance(), { recording_ }, ui_.playbackwidget->GetExportStartTime(), ui_.playbackwidget->GetExportEndTime()).exec();

}

}
