// findobjectwindow.cpp
//

///// Includes /////

#include "monocleclient/findobjectwindow.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <monocleprotocol/metadataframetype_generated.h>
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

///// Globals /////

const int STARTTIME_ROLE = Qt::UserRole;
const int ENDTIME_ROLE = Qt::UserRole + 1;
const int OBJECTCLASS_ROLE = Qt::UserRole + 2;
const int OBJECTID_ROLE = Qt::UserRole + 3;
const float THUMBNAIL_EXPANSION = 0.3f;

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

FINDOBJECTRESULT::FINDOBJECTRESULT(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight) :
  token_(token),
  start_(start),
  end_(end),
  objectclass_(objectclass),
  id_(id),
  largesttime_(largesttime),
  largestx_(largestx),
  largesty_(largesty),
  largestwidth_(largestwidth),
  largestheight_(largestheight)
{

}

FindObjectWindow::FindObjectWindow(QWidget* parent, const QImage& image, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingTrack>& track, const QVector4D& colour, const uint64_t starttime, const uint64_t endtime, const QRectF& rect, const int imagewidth, const int imageheight, const bool mirror, const ROTATION rotation, const bool stretch, const bool showobjects) :
  QDialog(parent),
  cudacontext_(MainWindow::Instance()->GetNextCUDAContext()),
  device_(device),
  recording_(recording),
  track_(track),
  imagewidth_(imagewidth),
  imageheight_(imageheight),
  mirror_(mirror),
  rotation_(rotation),
  stretch_(stretch),
  showobjects_(showobjects)
{
  ui_.setupUi(this);

  ui_.tableresults->setColumnWidth(1, 240);

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

  // Tracks
  for (const QSharedPointer<RecordingTrack>& metadatatrack : recording->GetMetadataTracks())
  {
    const std::vector<monocle::CODECINDEX> codecindices = metadatatrack->GetCodecIndices(monocle::Codec::OBJECTDETECTOR);
    for (const monocle::CODECINDEX& codecindex : codecindices)
    {
      std::vector<std::string> parameters;
      boost::split(parameters, codecindex.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);

      std::vector<std::string>::const_iterator videotrackidparameter = std::find_if(parameters.cbegin(), parameters.cend(), [](const std::string& parameter) { return (boost::istarts_with(parameter, "VideoTrackId")); });
      if (videotrackidparameter == parameters.cend())
      {

        continue;
      }

      const std::string videotrackparametervalue = videotrackidparameter->substr(13, std::string::npos);
      try
      {
        const uint32_t videotrackid = boost::lexical_cast<uint32_t>(videotrackparametervalue);
        if (recording->GetTrack(videotrackid))
        {
          ui_.combotracks->addItem(track_->GetDescription() + "->" + metadatatrack->GetDescription(), videotrackid);
          ui_.combotracks->setItemData(ui_.combotracks->count() - 1, metadatatrack->GetId(), Qt::UserRole + 1);
          break;
        }
      }
      catch (...)
      {
        // Ignore and continue

      }
    }
  }

  if (ui_.combotracks->count() == 0)
  {
    // This shouldn't happen because we checked before opening this window it should be ok, but just in case provide user with a nice error...
    //TODO qmessagebox and reject in 1ms
    return;
  }
  else
  {
    // Select the appropriate drop down
    for (int i = 0; i < ui_.combotracks->count(); ++i)
    {
      if (ui_.combotracks->itemData(0, Qt::UserRole) == track->GetId())
      {
        ui_.combotracks->setCurrentIndex(i);
        break;
      }
    }

    startTimer(std::chrono::milliseconds(100));
    on_buttonsearch_clicked();
  }
}

FindObjectWindow::~FindObjectWindow()
{
  connect_.Close();
  getauthenticatenonce_.Close();
  authenticate_.Close();
  videocreatestream_.Close();
  metadatacreatestream_.Close();
  createfindobject_.Close();

  retrievethumbnails_.clear();
  getsnapshotconnection_.Close();

  if (connection_)
  {
    if (findobjecttoken_.is_initialized())
    {
      connection_->DestroyFindObject(*findobjecttoken_);
      findobjecttoken_.reset();
    }

    if (videostreamtoken_.is_initialized())
    {
      connection_->DestroyStream(*videostreamtoken_, [](const std::chrono::steady_clock::duration latency, const monocle::client::DESTROYSTREAMRESPONSE&) {});
      videostreamtoken_.reset();
    }

    if (metadatastreamtoken_.is_initialized())
    {
      connection_->DestroyStream(*metadatastreamtoken_, [](const std::chrono::steady_clock::duration latency, const monocle::client::DESTROYSTREAMRESPONSE&) {});
      metadatastreamtoken_.reset();
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
  if (videostreamtoken_.is_initialized())
  {
    connection_->ControlStreamPause(*videostreamtoken_, time);

  }
  if (metadatastreamtoken_.is_initialized())
  {
    connection_->ControlStreamPause(*metadatastreamtoken_, time);

  }
}

void FindObjectWindow::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  if (!videostreamtoken_.is_initialized() || !metadatastreamtoken_.is_initialized())
  {

    return;
  }

  ResetDecoders();
  connection_->ControlStream(*videostreamtoken_, ui_.videowidget->GetNextVideoPlayRequestIndex(), true, true, true, time + device_->GetTimeOffset(), boost::none, numframes, false);
  if (numframes.is_initialized() && ((*numframes == 0) || (*numframes == 1))) // Is this an effectively pause request...
  {
    controlstreamendcallback_ = [this](const uint64_t playrequestindex, const monocle::ErrorCode err)
    {
      if (ui_.videowidget->GetVideoPlayRequestIndex() != playrequestindex)
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
  //TODO test this works by clicking around
  connection_->ControlStream(*metadatastreamtoken_, ui_.videowidget->GetNextMetadataPlayRequestIndex(), true, !numframes.is_initialized(), true, time + device_->GetTimeOffset(), boost::none, numframes, false);
}

void FindObjectWindow::Stop()
{
  if (videostreamtoken_.is_initialized())
  {
    ui_.videowidget->SetPaused(false);
    ResetDecoders();
    connection_->ControlStreamLive(*videostreamtoken_, ui_.videowidget->GetNextVideoPlayRequestIndex());
  }

  //TODO metadatastream thing copy main window...
}

void FindObjectWindow::timerEvent(QTimerEvent*)
{
  if (retrievethumbnails_.size() && !getsnapshotconnection_.IsConnected())
  {
    const FINDOBJECTRESULT retrievethumbnail = retrievethumbnails_.front();
    retrievethumbnails_.erase(retrievethumbnails_.begin());

    getsnapshotconnection_ = connection_->GetSnapshot(recording_->GetToken(), track_->GetId(), retrievethumbnail.largesttime_, retrievethumbnail.largestx_, retrievethumbnail.largesty_, retrievethumbnail.largestwidth_, retrievethumbnail.largestheight_, [this, retrievethumbnail](const std::chrono::steady_clock::duration latency, const monocle::client::GETSNAPSHOTRESPONSE& getsnapshotresponse)
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

      }
      else
      {
        for (int i = 0; i < ui_.tableresults->rowCount(); ++i)
        {
          if ((ui_.tableresults->item(i, 1)->data(STARTTIME_ROLE).toULongLong() == retrievethumbnail.start_) && (ui_.tableresults->item(i, 1)->data(OBJECTCLASS_ROLE).toULongLong() == static_cast<qulonglong>(retrievethumbnail.objectclass_)) && (ui_.tableresults->item(i, 1)->data(OBJECTID_ROLE).toULongLong() == retrievethumbnail.id_))
          {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setData(Qt::DecorationRole, QPixmap::fromImage(image));
            ui_.tableresults->setItem(i, 0, item);
            break;
          }
        }
      }
    });
  }
}

void FindObjectWindow::FrameStep(const bool forwards)
{
  ui_.videowidget->paused_ = true;
  ImageBuffer imagebuffer = ui_.videowidget->cache_.GetImage(forwards, ui_.videowidget->GetVideoPlayRequestIndex(), ui_.videowidget->sequencenum_);
  if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
  {
    controlstreamendcallback_ = [this, forwards](const uint64_t playrequestindex, const monocle::ErrorCode error)
    {
      if (ui_.videowidget->GetVideoPlayRequestIndex() != playrequestindex)
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

    if (videostreamtoken_.is_initialized())
    {
      ResetDecoders();
      connection_->ControlStreamFrameStep(*videostreamtoken_, ui_.videowidget->GetNextVideoPlayRequestIndex(), forwards, ui_.videowidget->sequencenum_);
    }
  }
  else
  {
    ui_.videowidget->WriteFrame(imagebuffer);

  }

  //TODO metadatastreamtoken_ stuff, copy main window

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
  if (findobjectwindow->GetVideoWidget()->GetVideoPlayRequestIndex() != playrequestindex)
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
  if (findobjectwindow->GetVideoWidget()->GetVideoPlayRequestIndex() != playrequestindex)
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
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (findobjectwindow->ui_.videowidget->metadataplayrequestindex_ != playrequestindex)
  {

    return;
  }
  if (metadataframetype == monocle::MetadataFrameType::OBJECT_DETECTION)
  {
    if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(signaturedata), signaturedatasize).VerifyBuffer<monocle::Objects>(nullptr))
    {
      // Ignore illegal packets
      return;
    }

    const monocle::Objects* objects = flatbuffers::GetRoot<monocle::Objects>(signaturedata);
    if ((objects == nullptr) || (objects->objects() == nullptr))
    {

      return;
    }
    findobjectwindow->ui_.videowidget->makeCurrent();
    findobjectwindow->ui_.videowidget->objects_.Update(findobjectwindow->ui_.videowidget->GetImagePixelRectF(), findobjectwindow->mirror_, findobjectwindow->rotation_, objects, timestamp, findobjectwindow->ui_.videowidget->time_);
    findobjectwindow->ui_.videowidget->doneCurrent();
  }
}

void FindObjectWindow::JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata)
{
  FindObjectWindow* findobjectwindow = reinterpret_cast<FindObjectWindow*>(callbackdata);
  if (findobjectwindow->GetVideoWidget()->GetVideoPlayRequestIndex() != playrequestindex)
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
  if (findobjectwindow->GetVideoWidget()->GetVideoPlayRequestIndex() != playrequestindex)
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

bool FindObjectWindow::Filter(const monocle::ObjectClass objectclass)
{
  if (((objectclass == monocle::ObjectClass::Human)     && ui_.checkfilterhumans->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Bicycle)   && ui_.checkfilterbicycles->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Car)       && ui_.checkfiltercars->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Bicycle)   && ui_.checkfiltermotorbikes->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Aeroplane) && ui_.checkfilteraeroplanes->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Bus)       && ui_.checkfilterbuses->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Train)     && ui_.checkfiltertrains->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Truck)     && ui_.checkfiltertrucks->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Boat)      && ui_.checkfilterboats->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Cat)       && ui_.checkfiltercats->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Dog)       && ui_.checkfilterdogs->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Horse)     && ui_.checkfilterhorses->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Backpack)  && ui_.checkfilterbackpacks->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Umbrella)  && ui_.checkfilterumbrellas->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Handbag)   && ui_.checkfilterhandbags->isChecked()) ||
      ((objectclass == monocle::ObjectClass::Suitcase)  && ui_.checkfiltersuitcases->isChecked()))
  {

    return true;
  }
  else
  {

    return false;
  }
}

bool FindObjectWindow::Filter(const monocle::ObjectClass objectclass, const uint64_t start, const uint64_t end)
{
  if (Filter(objectclass))
  {
    if ((end - start) >= (ui_.spinminimumduration->value() * 1000))
    {

      return true;
    }
    else
    {

      return false;
    }
  }
  else
  {

    return false;
  }
}

void FindObjectWindow::UpdateFilter()
{
  for (int i = 0; i < ui_.tableresults->rowCount(); ++i)
  {
    if (Filter(static_cast<monocle::ObjectClass>(ui_.tableresults->item(i, 1)->data(OBJECTCLASS_ROLE).toULongLong()), ui_.tableresults->item(i, 1)->data(STARTTIME_ROLE).toULongLong(), ui_.tableresults->item(i, 1)->data(ENDTIME_ROLE).toULongLong()))
    {
      ui_.tableresults->showRow(i);

    }
    else
    {
      ui_.tableresults->hideRow(i);

    }
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

  const float x = std::max(0.0f, largestx - (largestwidth * THUMBNAIL_EXPANSION));
  const float y = std::max(0.0f, largesty - (largestheight * THUMBNAIL_EXPANSION));
  const float width = std::min(1.0f, largestwidth + (2 * largestwidth * THUMBNAIL_EXPANSION));
  const float height = std::min(1.0f, largestheight + (2 * largestheight * THUMBNAIL_EXPANSION));
  retrievethumbnails_.push_back(FINDOBJECTRESULT(token, start, end, objectclass, id, largesttime, x, y, width, height));
  
  // Insert at the correct location...
  int row = ui_.tableresults->rowCount();
  for (int i = 0; i < ui_.tableresults->rowCount(); ++i)
  {
    if (ui_.tableresults->item(i, 1)->data(STARTTIME_ROLE).toULongLong() > start)
    {
      row = i;
      break;
    }
  }
  ui_.tableresults->insertRow(row);
  
  QTableWidgetItem* item = new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(start, Qt::UTC).toString());
  item->setData(STARTTIME_ROLE, static_cast<qulonglong>(start));
  item->setData(ENDTIME_ROLE, static_cast<qulonglong>(end));
  item->setData(OBJECTCLASS_ROLE, static_cast<qulonglong>(objectclass));
  item->setData(OBJECTID_ROLE, static_cast<qulonglong>(id));
  ui_.tableresults->setItem(row, 1, item);
  const uint64_t durationseconds = (end - start) / 1000;
  const uint64_t durationminutes = durationseconds / 60;
  const uint64_t durationhours = durationminutes / 60;
  const QString seconds = (durationseconds % 60) < 10 ? ("0" + QString::number(durationseconds % 60)) : QString::number(durationseconds % 60);
  const QString minutes = durationminutes < 10 ? ("0" + QString::number(durationminutes)) : QString::number(durationminutes);
  const QString hours = durationhours < 10 ? ("0" + QString::number(durationhours)) : QString::number(durationhours);
  ui_.tableresults->setItem(row, 2, new QTableWidgetItem(hours + ":" + minutes + ":" + seconds));
  if (!Filter(objectclass, end, start))
  {
    ui_.tableresults->hideRow(row);

  }
  ui_.playbackwidget->FindObjectResult(start, end);
}

void FindObjectWindow::on_buttonsearch_clicked()
{
  // Swap out the track playbackwidget indices
  const uint32_t videotrackid = ui_.combotracks->currentData(Qt::UserRole).toUInt();
  const uint32_t metadatatrackid = ui_.combotracks->currentData(Qt::UserRole + 1).toUInt();
  const QSharedPointer<RecordingTrack> videotrack = recording_->GetTrack(videotrackid);
  const QSharedPointer<RecordingTrack> metadatatrack = recording_->GetTrack(metadatatrackid);
  if ((videotrack == nullptr) || (videotrack->GetTrackType() != monocle::TrackType::Video))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find video track: ") + QString::number(metadatatrackid), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  if ((metadatatrack == nullptr) || (metadatatrack->GetTrackType() != monocle::TrackType::Metadata))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find metadata track: ") + QString::number(metadatatrackid), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  ui_.playbackwidget->SetTrack(videotrack);
  track_ = videotrack;

  // Clear up previous attempts
  connect_.Close();
  getauthenticatenonce_.Close();
  authenticate_.Close();
  videocreatestream_.Close();
  metadatacreatestream_.Close();
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
  videostreamtoken_.reset();
  metadatastreamtoken_.reset();
  findobjecttoken_.reset();
  getsnapshotconnection_.Close();
  retrievethumbnails_.clear();
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
  
        videocreatestream_ = connection_->CreateStream(recording_->GetToken(), track_->GetId(), [this, metadatatrack](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
        {
          if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
          {
            QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to create video stream: ") + QString::fromStdString(createstreamresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
            return;
          }
  
          videostreamtoken_ = createstreamresponse.token_;
  
          DestroyDecoders();
          for (const monocle::CODECINDEX& codecindex : createstreamresponse.codecindices_)
          {
            AddCodecIndex(codecindex);
  
          }

          metadatacreatestream_ = connection_->CreateStream(recording_->GetToken(), metadatatrack->GetId(), [this, metadatatrack](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
          {
            if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
            {
              QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to create metadata stream: ") + QString::fromStdString(createstreamresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
              return;
            }

            metadatastreamtoken_ = createstreamresponse.token_;

            const QRectF selectedrect = ui_.videowidget->GetSelectedRect();
            createfindobject_ = connection_->CreateFindObject(recording_->GetToken(), metadatatrack->GetId(), ui_.datetimestart->dateTime().toMSecsSinceEpoch(), ui_.datetimeend->dateTime().toMSecsSinceEpoch(), selectedrect.x(), selectedrect.y(), selectedrect.width(), selectedrect.height(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::CREATEFINDOBJECTRESPONSE& createfindobjectresponse)
            {
              if (createfindobjectresponse.GetErrorCode() != monocle::ErrorCode::Success)
              {
                QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to create find object detector: ") + QString::fromStdString(createfindobjectresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
                return;
              }
              findobjecttoken_ = createfindobjectresponse.token_;
            });
          }, FindObjectWindow::ControlStreamEnd, FindObjectWindow::H265Callback, FindObjectWindow::H264Callback, FindObjectWindow::MetadataCallback, FindObjectWindow::JPEGCallback, FindObjectWindow::MPEG4Callback, FindObjectWindow::NewCodecIndexCallback, this);
        }, FindObjectWindow::ControlStreamEnd, FindObjectWindow::H265Callback, FindObjectWindow::H264Callback, FindObjectWindow::MetadataCallback, FindObjectWindow::JPEGCallback, FindObjectWindow::MPEG4Callback, FindObjectWindow::NewCodecIndexCallback, this);
      });
    });
  });
}

void FindObjectWindow::on_checkfilterhumans_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterbicycles_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfiltercars_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfiltermotorbikes_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilteraeroplanes_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterbuses_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfiltertrains_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfiltertrucks_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterboats_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfiltercats_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterdogs_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterhorses_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterbackpacks_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterumbrellas_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfilterhandbags_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_checkfiltersuitcases_clicked()
{
  UpdateFilter();

}

void FindObjectWindow::on_spinminimumduration_valueChanged(int)
{
  UpdateFilter();

}

void FindObjectWindow::on_tableresults_clicked(QModelIndex)
{
  if (!connection_ || !videostreamtoken_.is_initialized() || !metadatastreamtoken_.is_initialized())
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

  const uint64_t time = item->data(STARTTIME_ROLE).toULongLong() - 1500;
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
  if (videostreamtoken_.is_initialized() && metadatastreamtoken_.is_initialized() && connection_)
  {
    connection_->ControlStreamPause(*videostreamtoken_, ui_.videowidget->time_);
    //TODO metadata...
  }
}

void FindObjectWindow::on_buttonstop_clicked()
{
  if (videostreamtoken_.is_initialized() && metadatastreamtoken_.is_initialized() && connection_)
  {
    ui_.videowidget->playmarkertime_ = ui_.playbackwidget->endtime_ + device_->GetTimeOffset();
    ui_.videowidget->frametime_ = std::chrono::steady_clock::now();
    ui_.videowidget->SetPaused(false);
    connection_->ControlStreamLive(*videostreamtoken_, ui_.videowidget->GetNextVideoPlayRequestIndex());
    connection_->ControlStreamLive(*metadatastreamtoken_, ui_.videowidget->GetNextMetadataPlayRequestIndex());

    ui_.playbackwidget->makeCurrent();
    ui_.playbackwidget->UpdateRecordingBlocks();
    ui_.playbackwidget->doneCurrent();
  }
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
