// findobjectwindow.cpp
//

///// Includes /////

#include "monocleclient/findobjectwindow.h"

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

///// Methods /////

FindObjectWindow::FindObjectWindow(QWidget* parent, const QImage& image, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingTrack>& track, const QVector4D& colour, const uint64_t starttime, const uint64_t endtime, const QRectF& rect, const int imagewidth, const int imageheight, const bool mirror, const ROTATION rotation, const bool stretch) :
  QDialog(parent),
  cudacontext_(MainWindow::Instance()->GetNextCUDAContext()),
  device_(device),
  recording_(recording),
  track_(track),
  imagewidth_(imagewidth),
  imageheight_(imageheight),
  mirror_(mirror),
  rotation_(rotation),
  stretch_(stretch)
{
  ui_.setupUi(this);

  // We do this out of order because otherwise we come before FindObjectPlaybackWidget::initialiseGL
  ui_.datetimestart->setDateTime(QDateTime::fromMSecsSinceEpoch(starttime, Qt::UTC));
  ui_.datetimeend->setDateTime(QDateTime::fromMSecsSinceEpoch(endtime, Qt::UTC));

  connect(recording_.data(), &Recording::TrackAdded, this, &FindObjectWindow::TrackAdded);
  connect(recording_.data(), &Recording::TrackRemoved, this, &FindObjectWindow::TrackRemoved);
  connect(ui_.datetimestart, &QDateTimeEdit::dateTimeChanged, this, &FindObjectWindow::StartDateTimeChanged);
  connect(ui_.datetimeend, &QDateTimeEdit::dateTimeChanged, this, &FindObjectWindow::EndDateTimeChanged);

  ui_.videowidget->SetImage(image);
  ui_.videowidget->SetSelectedRect(rect);
  ui_.playbackwidget->SetTrack(track);
  ui_.playbackwidget->SetColour(colour);

  ui_.tableresults->setItemDelegateForColumn(0, new ImageItemDelegate());

  // Track
  for (const QSharedPointer<RecordingTrack>& track : recording->GetTracks())
  {
//TODO we need to do some checks here to see whether they have object detectors in them(by checking the new codec indices)
  //TODO then we see which video track those object detectors are attached too, and then display those
    if (track->GetTrackType() != monocle::TrackType::Metadata)
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

  on_buttonsearch_clicked();
}

FindObjectWindow::~FindObjectWindow()
{
  if (connection_)
  {
    if (findobjecttoken_.is_initialized())
    {
      connection_->DestroyFindObject(*findobjecttoken_);
      findobjecttoken_.reset();
    }

    if (streamtoken_.is_initialized())
    {
      connection_->DestroyStream(*streamtoken_, [](const std::chrono::steady_clock::duration latency, const monocle::client::DESTROYSTREAMRESPONSE&) {});
      streamtoken_.reset();
    }
    connection_->Destroy();
    connection_.reset();
  }
}

FindObjectPlaybackWidget* FindObjectWindow::GetPlaybackWidget() const
{
  return ui_.playbackwidget;
}

FindObjectVideoWidget* FindObjectWindow::GetVideoWidget() const
{
  return ui_.videowidget;
}

void FindObjectWindow::SetPaused(const bool paused)
{
  ui_.videowidget->SetPaused(paused);
}

void FindObjectWindow::Pause(const boost::optional<uint64_t>& time)
{
  SetPaused(true);
  if (streamtoken_.is_initialized())
  {
    connection_->ControlStreamPause(*streamtoken_, time);

  }
}

void FindObjectWindow::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  if (!streamtoken_.is_initialized())
  {

    return;
  }

  ResetDecoders();
  connection_->ControlStream(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex(), true, true, true, time + device_->GetTimeOffset(), boost::none, numframes, false);
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
    
      ui_.videowidget->imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { ui_.videowidget->cache_.AddImage(imagebuffer); });
    
      ImageBuffer imagebuffer = ui_.videowidget->cache_.GetLatestImageBySequence(playrequestindex);
      if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Unable to find frame");
        return;
      }
      ui_.videowidget->WriteFrame(imagebuffer);
    };
    ui_.videowidget->paused_ = true;
  }
  else
  {
    ui_.videowidget->paused_ = false;

  }
}

void FindObjectWindow::Stop()
{
  if (!streamtoken_.is_initialized())
  {

    return;
  }

  ui_.videowidget->SetPaused(false);
  ResetDecoders();
  connection_->ControlStreamLive(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex());
}

void FindObjectWindow::FrameStep(const bool forwards)
{
  ui_.videowidget->paused_ = true;
  ImageBuffer imagebuffer = ui_.videowidget->cache_.GetImage(forwards, ui_.videowidget->GetPlayRequestIndex(), ui_.videowidget->sequencenum_);
  if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
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

      ui_.videowidget->imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { ui_.videowidget->cache_.AddImage(imagebuffer); });

      // Find the previous/next frame to go to
      ImageBuffer imagebuffer = ui_.videowidget->cache_.GetImage(forwards, playrequestindex, ui_.videowidget->sequencenum_);
      if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Unable to find frame");
        return;
      }

      ui_.videowidget->WriteFrame(imagebuffer);
    };

    if (streamtoken_.is_initialized())
    {
      ResetDecoders();
      connection_->ControlStreamFrameStep(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex(), forwards, ui_.videowidget->sequencenum_);
    }
  }
  else
  {
    ui_.videowidget->WriteFrame(imagebuffer);

  }
}

void FindObjectWindow::ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata)
{
  FindObjectWindow* videoview = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (videoview->controlstreamendcallback_)
  {
    videoview->controlstreamendcallback_(playrequestindex, error);

  }
}

void FindObjectWindow::H265Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata)
{
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (findobjectwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<H265Decoder> >::iterator h265decoder = std::find_if(findobjectwindow->h265decoders_.begin(), findobjectwindow->h265decoders_.end(), [codecindex](std::unique_ptr<H265Decoder>& h265decoder) { return (h265decoder->GetId() == codecindex); });
  if (h265decoder == findobjectwindow->h265decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*h265decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findobjectwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (findobjectwindow->GetImageQueue().write_available())
    {
      findobjectwindow->GetImageQueue().push(imagebuffer);
      findobjectwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindObjectWindow::H264Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata)
{
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (findobjectwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<H264Decoder> >::iterator h264decoder = std::find_if(findobjectwindow->h264decoders_.begin(), findobjectwindow->h264decoders_.end(), [codecindex](std::unique_ptr<H264Decoder>& h264decoder) { return (h264decoder->GetId() == codecindex); });
  if (h264decoder == findobjectwindow->h264decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*h264decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findobjectwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (findobjectwindow->GetImageQueue().write_available())
    {
      findobjectwindow->GetImageQueue().push(imagebuffer);
      findobjectwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindObjectWindow::MetadataCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{

}

void FindObjectWindow::JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata)
{
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (findobjectwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  if (!findobjectwindow->mjpegdecoder_)
  {
    findobjectwindow->mjpegdecoder_ = std::make_unique<MJpegDecoder>(codecindex, findobjectwindow->device_->GetPublicKey()); // Id doesn't matter because we always ignore it from this point onwards
    if (findobjectwindow->mjpegdecoder_->Init())
    {

      return;
    }
  }

  ImageBuffer imagebuffer = findobjectwindow->mjpegdecoder_->Decode(playrequestindex, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findobjectwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (findobjectwindow->GetImageQueue().write_available())
    {
      findobjectwindow->GetImageQueue().push(imagebuffer);
      findobjectwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindObjectWindow::MPEG4Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (findobjectwindow->GetVideoWidget()->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<MPEG4Decoder> >::iterator mpeg4decoder = std::find_if(findobjectwindow->mpeg4decoders_.begin(), findobjectwindow->mpeg4decoders_.end(), [codecindex](std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == codecindex); });
  if (mpeg4decoder == findobjectwindow->mpeg4decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*mpeg4decoder)->Decode(playrequestindex, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &findobjectwindow->GetFreeImageQueue());
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (findobjectwindow->GetImageQueue().write_available())
    {
      findobjectwindow->GetImageQueue().push(imagebuffer);
      findobjectwindow->GetVideoWidget()->update();
    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void FindObjectWindow::NewCodecIndexCallback(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata)
{
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  findobjectwindow->AddCodecIndex(monocle::CODECINDEX(id, codec, parameters, timestamp));
}

void FindObjectWindow::AddCodecIndex(const monocle::CODECINDEX& codecindex)
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
    std::unique_ptr<H264Decoder> h264decoder = std::make_unique<H264Decoder>(codecindex.id_, device_->GetPublicKey(), cudacontext_);
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

void FindObjectWindow::DestroyDecoders()
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

void FindObjectWindow::ResetDecoders()
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

void FindObjectWindow::TrackAdded(const QSharedPointer<RecordingTrack>& track)
{
  if (track->GetTrackType() != monocle::TrackType::Video)
  {

    return;
  }
  ui_.combotracks->addItem(track->GetDescription(), track->GetId());
}

void FindObjectWindow::TrackRemoved(const uint32_t trackid)
{
  const int row = ui_.combotracks->findData(trackid);
  if (row == -1)
  {

    return;
  }
  ui_.combotracks->removeItem(row);
}

void FindObjectWindow::StartDateTimeChanged(const QDateTime& datetime)
{
  ui_.datetimeend->setMinimumDateTime(datetime);
  ui_.playbackwidget->SetExportStartTime(datetime.toMSecsSinceEpoch(), true);
}

void FindObjectWindow::EndDateTimeChanged(const QDateTime& datetime)
{
  ui_.datetimestart->setMaximumDateTime(datetime);
  ui_.playbackwidget->SetExportEndTime(datetime.toMSecsSinceEpoch(), true);
}

void FindObjectWindow::FindObjectEnd(const uint64_t token, const uint64_t ret)
{
  if (!findobjecttoken_.is_initialized() || (*findobjecttoken_ != token))
  {
  
    return;
  }
  
  ui_.progressbar->setValue(100);
  
  if (ret)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Find Object failed: ") + QString::number(ret), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
  
  }
}

void FindObjectWindow::FindObjectProgress(const uint64_t token, const float progress)
{
  ui_.progressbar->setValue(100.0f * progress);

}

void FindObjectWindow::FindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight)
{
  if (!findobjecttoken_.is_initialized() || (*findobjecttoken_ != token))
  {
  
    return;
  }
  //TODO this should request in a queue... one after the other
    //TODO do it inside itself I guess?
      //TODO unfortunately it just spanks it as it is at the moment and doesn't allow anyone else in
  getsnapshotconnections_.emplace_back(connection_->GetSnapshot(recording_->GetToken(), track_->GetId(), largesttime, [this, start, largesttime, largestx, largesty, largestwidth, largestheight](const std::chrono::steady_clock::duration latency, const monocle::client::GETSNAPSHOTRESPONSE& getsnapshotresponse)
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
    int x = largestx * image.width();
    int width = largestwidth * image.width();
    if (width < 32)//TODO THUMBNAIL_MIN_SIZE make it 64 I think
    {
      const int diff = (32 - width) / 2;
      if ((x + width + diff) > image.width())
      {
        width = width + (image.width() - (x + width));

      }
      else
      {
        width += (diff * 2);

      }
      x -= diff;
      x = std::max(0, x);
    }
    int y = largesty * image.height();
    int height = largestheight * image.height();
    if (height < 32)
    {
      const int diff = (32 - height) / 2;
      if ((y + height + diff) > image.height())
      {
        height = height + (image.height() - (y + height));

      }
      else
      {
        height += (diff * 2);

      }
      y -= diff;
      y = std::max(0, y);
    }
    image = image.copy(x, y, width, height);
  
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
  
  ui_.playbackwidget->FindObjectResult(start, end);
}

void FindObjectWindow::on_buttonsearch_clicked()
{
  // Swap out the track playbackwidget indices
  const uint32_t trackid = ui_.combotracks->currentData().toUInt();
  const QSharedPointer<RecordingTrack> metadatatrack = recording_->GetTrack(trackid);
  if ((metadatatrack == nullptr) || (metadatatrack->GetTrackType() != monocle::TrackType::Metadata))//TODO umm video now please or something?
  {

    return;
  }

  //TODO ui_.playbackwidget->SetTrack(track);

  // Clear up previous attempts
  connect_.Close();
  getauthenticatenonce_.Close();
  authenticate_.Close();
  createstream_.Close();
  createfindobject_.Close();
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
  connect(connection_.get(), &Connection::SignalFindObjectEnd, this, &FindObjectWindow::FindObjectEnd);
  connect(connection_.get(), &Connection::SignalFindObjectProgress, this, &FindObjectWindow::FindObjectProgress);
  connect(connection_.get(), &Connection::SignalFindObjectResult, this, &FindObjectWindow::FindObjectResult);
  streamtoken_.reset();
  findobjecttoken_.reset();
  connect_ = connection_->Connect([this, metadatatrack, username, password](const boost::system::error_code& err)
  {
    if (err)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to connect"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  
    getauthenticatenonce_ = connection_->GetAuthenticationNonce([this, metadatatrack, username, password](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
    {
      if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to get authentication nonce: ") + QString::fromStdString(getauthenticationnonceresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
  
      const std::string clientnonce = utility::GenerateRandomString(32);
      authenticate_ = connection_->Authenticate(username, clientnonce, monocle::AuthenticateDigest(username, password, getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this, metadatatrack](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
      {
        if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to authenticate: ") + QString::fromStdString(authenticateresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          return;
        }
  
        createstream_ = connection_->CreateStream(recording_->GetToken(), track_->GetId(), [this, metadatatrack](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
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
          createfindobject_ = connection_->CreateFindObject(recording_->GetToken(), metadatatrack->GetId(), ui_.datetimestart->dateTime().toMSecsSinceEpoch(), ui_.datetimeend->dateTime().toMSecsSinceEpoch(), ui_.spinminimumduration->value() * 1000, selectedrect.x(), selectedrect.y(), selectedrect.width(), selectedrect.height(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::CREATEFINDOBJECTRESPONSE& createfindobjectresponse)
          {
            if (createfindobjectresponse.GetErrorCode() != monocle::ErrorCode::Success)
            {
              QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to create find object detector: ") + QString::fromStdString(createfindobjectresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
              return;
            }
          
            std::for_each(getsnapshotconnections_.begin(), getsnapshotconnections_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
            getsnapshotconnections_.clear();
            findobjecttoken_ = createfindobjectresponse.token_;
          });
        }, FindObjectWindow::ControlStreamEnd, FindObjectWindow::H265Callback, FindObjectWindow::H264Callback, FindObjectWindow::MetadataCallback, FindObjectWindow::JPEGCallback, FindObjectWindow::MPEG4Callback, FindObjectWindow::NewCodecIndexCallback, this);
      });
    });
  });
}

void FindObjectWindow::on_tableresults_clicked(QModelIndex)
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

void FindObjectWindow::on_buttonframestepbackwards_clicked()
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

void FindObjectWindow::on_buttonplay_clicked()
{
  Play(ui_.videowidget->time_ - device_->GetTimeOffset(), boost::none);
  ui_.videowidget->playmarkertime_ = ui_.videowidget->time_;
  ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
}

void FindObjectWindow::on_buttonpause_clicked()
{
  ui_.videowidget->SetPaused(true);
  if (streamtoken_.is_initialized() && connection_)
  {
    connection_->ControlStreamPause(*streamtoken_, ui_.videowidget->time_);
    
  }
}

void FindObjectWindow::on_buttonstop_clicked()
{
  ui_.videowidget->playmarkertime_ = ui_.playbackwidget->endtime_ + device_->GetTimeOffset();
  ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
  ui_.videowidget->SetPaused(false);
  connection_->ControlStreamLive(*streamtoken_, ui_.videowidget->GetNextPlayRequestIndex());

  ui_.playbackwidget->makeCurrent();
  ui_.playbackwidget->UpdateRecordingBlocks();
  ui_.playbackwidget->doneCurrent();
}

void FindObjectWindow::on_buttonframestepforwards_clicked()
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

void FindObjectWindow::on_buttonzoomin_clicked()
{
  ui_.playbackwidget->ZoomIn(width() / 2);
  ui_.playbackwidget->makeCurrent();
  ui_.playbackwidget->UpdateRecordingBlocks();
  ui_.playbackwidget->doneCurrent();
}

void FindObjectWindow::on_buttonzoomout_clicked()
{
  ui_.playbackwidget->ZoomOut(width() / 2);
  ui_.playbackwidget->makeCurrent();
  ui_.playbackwidget->UpdateRecordingBlocks();
  ui_.playbackwidget->doneCurrent();
}

void FindObjectWindow::on_buttonexport_clicked()
{
  ExportWindow(MainWindow::Instance(), { recording_ }, ui_.playbackwidget->GetExportStartTime(), ui_.playbackwidget->GetExportEndTime()).exec();

}

}
