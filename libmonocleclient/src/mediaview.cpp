// mediaview.cpp
//

///// Includes /////

#include "monocleclient/mediaview.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <future>
#include <monocleprotocol/objects_generated.h>
#include <network/uri.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QResource>
#include <QStandardPaths>

#include "monocleclient/h265decoder.h"
#include "monocleclient/h264decoder.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/media.h"
#include "monocleclient/mjpegdecoder.h"
#include "monocleclient/mpeg4decoder.h"
#include "monocleclient/recording.h"
#include "monocleclient/stream.h"
#include "monocleclient/videowidget.h"

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum CONTROLTYPE
{
  CONTROLTYPE_DESTROY,
  CONTROLTYPE_PAUSETIME,
  CONTROLTYPE_PAUSE,
  CONTROLTYPE_PLAYBACK,
  CONTROLTYPE_FRAMESTEP
};

class ControlRequest
{
 public:

  ControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest);
  virtual ~ControlRequest();

  virtual CONTROLTYPE GetControlType() const = 0;

  std::future< std::unique_ptr<ControlRequest> >&& GetNextRequest() { return std::move(nextcontrolrequest_); }

private:

  std::future< std::unique_ptr<ControlRequest> > nextcontrolrequest_;

};

class DestroyControlRequest : public ControlRequest
{
 public:

  DestroyControlRequest();

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_DESTROY; }

};

class PauseTimeControlRequest : public ControlRequest
{
 public:

   PauseTimeControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const uint64_t time, const bool iframes);

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_PAUSETIME; }

  uint64_t playrequestindex_;
  uint64_t time_;
  bool iframes_;

};

class PauseControlRequest : public ControlRequest
{
 public:

  PauseControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const boost::optional<uint64_t>& frame);

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_PAUSE; }

  boost::optional<uint64_t> frame_;

};

class PlaybackControlRequest : public ControlRequest
{
 public:

  PlaybackControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes);

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_PLAYBACK; }

  uint64_t playrequestindex_;
  bool fetchmarker_;
  bool ratecontrol_;
  bool forwards_;
  boost::optional<uint64_t> starttime_;
  boost::optional<uint64_t> endtime_;
  boost::optional<uint64_t> numframes_;

};

class PlaybackControlFrameStepRequest : public ControlRequest
{
 public:

  PlaybackControlFrameStepRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum);

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_FRAMESTEP; }

  uint64_t playrequestindex_;
  bool forwards_;
  uint64_t frame_;

};

///// Methods /////

ControlRequest::ControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest) :
  nextcontrolrequest_(std::move(nextcontrolrequest))
{

}

ControlRequest::~ControlRequest()
{

}

DestroyControlRequest::DestroyControlRequest() :
  ControlRequest(std::future< std::unique_ptr<ControlRequest> >())
{

}

PlaybackControlRequest::PlaybackControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes) :
  ControlRequest(std::move(nextcontrolrequest)),
  playrequestindex_(playrequestindex),
  fetchmarker_(fetchmarker),
  ratecontrol_(ratecontrol),
  forwards_(forwards),
  starttime_(starttime),
  endtime_(endtime),
  numframes_(numframes)
{

}

PlaybackControlFrameStepRequest::PlaybackControlFrameStepRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const bool forwards, const uint64_t frame) :
  ControlRequest(std::move(nextcontrolrequest)),
  playrequestindex_(playrequestindex),
  forwards_(forwards),
  frame_(frame)
{

}

PauseTimeControlRequest::PauseTimeControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const uint64_t time, const bool iframes) :
  ControlRequest(std::move(nextcontrolrequest)),
  playrequestindex_(playrequestindex),
  time_(time),
  iframes_(iframes)
{

}

PauseControlRequest::PauseControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const boost::optional<uint64_t>& frame) :
  ControlRequest(std::move(nextcontrolrequest)),
  frame_(frame)
{

}

MediaView::MediaStream::MediaStream(const file::DEVICE& device, const file::RECORDING& recording, const file::TRACK& track, const uint64_t index) :
  device_(device),
  recording_(recording),
  track_(track),
  index_(index),
  running_(false)
{

}

MediaView::MediaStream::~MediaStream()
{
  Destroy();

}

void MediaView::MediaStream::Init(MediaView* mediaview, const bool mainstream, const uint64_t playrequestindex)
{
  Destroy();

  running_ = true;
  thread_ = std::thread(&MediaView::PlayThread, mediaview, shared_from_this(), mainstream, playrequestindex, std::move(promise_.get_future()));
}

void MediaView::MediaStream::Destroy()
{
  if (thread_.joinable())
  {
    running_ = false;
    promise_.set_value(std::make_unique<DestroyControlRequest>());
    promise_ = std::promise< std::unique_ptr<ControlRequest> >();
    thread_.join();
  }
}

std::vector< std::shared_ptr<file::FRAMEHEADER> >::const_iterator MediaView::MediaStream::GetFrame(const uint64_t time) const
{
  auto i = track_.frameheaders_.cend();
  if (track_.frameheaders_.size() && (time > track_.frameheaders_.back()->time_)) // lower_bound doesn't work beyond the end, so hack this one in...
  {
    i = track_.frameheaders_.cend() - 1;

  }
  else
  {
    const std::shared_ptr<file::FRAMEHEADER> tmp = std::make_shared<file::METADATAFRAMEHEADER>(0, 0, 0, time, file::MetadataFrameType::ONVIF_XML, std::vector<uint8_t>()); // Only time matters, the rest of this can be ignored
    i = std::lower_bound(track_.frameheaders_.cbegin(), track_.frameheaders_.cend(), tmp, [](const std::shared_ptr<file::FRAMEHEADER>& lhs, const std::shared_ptr<file::FRAMEHEADER>& rhs) { return (lhs->time_ < rhs->time_); });
  }
  if (i == track_.frameheaders_.cend())
  {

    return track_.frameheaders_.cend();
  }

  while ((i > track_.frameheaders_.cbegin()) && ((*i)->time_ > time))
  {
    --i;

  }

  return i;
}

MediaView::MediaView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool showinfo, const bool showobjects, const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t videotrackindex, const QResource* arial) :
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, rotation, mirror, stretch, showinfo, showobjects, arial, true, true, true, true, true, true, true),
  media_(media),
  nextstreamindex_(0),
  buffersize_(0)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  SetMessage(GetPlayRequestIndex(), false, QString("Loading Track"));

  boost::container::flat_set<file::DEVICE>::const_iterator device = std::find_if(media_->GetMedia().GetFile().devices_.cbegin(), media_->GetMedia().GetFile().devices_.cend(), [deviceindex](const file::DEVICE& device) { return (device.index_ == deviceindex); });
  if (device == media_->GetMedia().GetFile().devices_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find device in file: ") + QString::number(deviceindex));
    return;
  }

  boost::container::flat_set<file::RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [recordingindex](const file::RECORDING& recording) { return (recording.index_ == recordingindex); });
  if (recording == device->recordings_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find recording in file: ") + QString::number(recordingindex));
    return;
  }

  boost::container::flat_set<file::TRACK>::const_iterator videotrack = std::find_if(recording->videotracks_.cbegin(), recording->videotracks_.cend(), [videotrackindex](const file::TRACK& track) { return (track.index_ == videotrackindex); });
  if (videotrack == recording->videotracks_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find track in file: ") + QString::number(videotrackindex));
    return;
  }
  
  // Load public key
  if (device->signingkey_.size())
  {
    if (signingkey_.Init(device->signingkey_))
    {
      LOG_GUI_WARNING(QString("PublicKey::Init failed to initialise"));

    }
  }

  Init(deviceindex, recordingindex, videotrackindex, boost::none);
}

MediaView::~MediaView()
{
  if (videostream_)
  {
    videostream_->Destroy();
    videostream_.reset();
  }
  
  for (boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatastream->Destroy();
    metadatastream.reset();
  }
  metadatastreams_.clear();
}

void MediaView::GetMenu(QMenu& parent)
{
  if (!videostream_)
  {

    return;
  }

  boost::container::flat_set<file::DEVICE>::const_iterator device = std::find_if(media_->GetMedia().GetFile().devices_.cbegin(), media_->GetMedia().GetFile().devices_.cend(), [this](const file::DEVICE& device) { return (device.index_ == videostream_->GetDeviceIndex()); });
  if (device == media_->GetMedia().GetFile().devices_.cend())
  {
    LOG_GUI_WARNING(QString("Unable to find device: ") + QString::number(videostream_->GetDeviceIndex()));
    return;
  }

  boost::container::flat_set<file::RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [this](const file::RECORDING& recording) { return (recording.index_ == videostream_->GetRecordingIndex()); });
  if (recording == device->recordings_.cend())
  {
    LOG_GUI_WARNING(QString("Unable to find recording: ") + QString::number(videostream_->GetRecordingIndex()));
    return;
  }

  QMenu* tracks = new QMenu("Tracks", &parent);
  for (const file::TRACK& track : recording->videotracks_)
  {
    const QString description = track.description_.empty() ? QString() : QString("(" + QString::fromStdString(track.description_) + ")");
    QAction* action = tracks->addAction(QString::number(track.index_) + description, [this, deviceindex = videostream_->GetDeviceIndex(), recordingindex = videostream_->GetRecordingIndex(), videotrackindex = track.index_]()
    {
      if ((deviceindex == videostream_->GetDeviceIndex()) && (recordingindex == videostream_->GetRecordingIndex()) && (videotrackindex == videostream_->GetTrackIndex())) // Ignore switching to ourself
      {

        return;
      }

      boost::container::flat_set<file::DEVICE>::const_iterator device = std::find_if(media_->GetMedia().GetFile().devices_.cbegin(), media_->GetMedia().GetFile().devices_.cend(), [deviceindex](const file::DEVICE& device) { return (device.index_ == deviceindex); });
      if (device == media_->GetMedia().GetFile().devices_.cend())
      {
        LOG_GUI_WARNING(QString("Unable to find device: ") + QString::number(deviceindex));
        return;
      }

      boost::container::flat_set<file::RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [recordingindex](const file::RECORDING& recording) { return (recording.index_ == recordingindex); });
      if (recording == device->recordings_.cend())
      {
        LOG_GUI_WARNING(QString("Unable to find recording: ") + QString::number(recordingindex));
        return;
      }

      boost::container::flat_set<file::TRACK>::const_iterator track = std::find_if(recording->videotracks_.cbegin(), recording->videotracks_.cend(), [videotrackindex](const file::TRACK& track) { return (track.index_ == videotrackindex); });
      if (recording == device->recordings_.cend())
      {
        LOG_GUI_WARNING(QString("Unable to find track: ") + QString::number(videotrackindex));
        return;
      }

      Init(deviceindex, recordingindex, videotrackindex, time_); // Restart playback on another track
    });
    action->setCheckable(true);
    action->setChecked(track.index_ == videostream_->GetTrackIndex());
  }
  parent.addMenu(tracks);

  View::GetMenu(parent);
}

double MediaView::GetAspectRatio() const
{
  if (imagewidth_ && imageheight_)
  {

    return (static_cast<double>(imagewidth_) / static_cast<double>(imageheight_));
  }
  else
  {

    return 0.0;
  }
}

bool MediaView::GetImage(ImageBuffer& imagebuffer)
{
  bool hasimage = false;
  ImageBuffer previmagebuffer;
  while (true)
  {
    if (imagequeue_.pop(imagebuffer))
    {
      bandwidthsizes_.push_back(std::make_pair(std::chrono::steady_clock::now(), imagebuffer.originalsize_));

      // If we have skipped frames, we should place them back into the temporary list, or destroy them if there is no room
      if (previmagebuffer.buffer_ || imagebuffer.cudacontext_)
      {
        if (paused_)
        {
          cache_.AddImage(previmagebuffer);

        }
        else
        {
          freeimagequeue_.AddFreeImage(previmagebuffer);

        }
      }
      previmagebuffer = imagebuffer;
      hasimage = true;
    }
    else
    {
      if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
      {
        imagewidth_ = imagebuffer.widths_[0];
        imageheight_ = imagebuffer.heights_[0];
      }
      return hasimage;
    }
  }
  return true;
}

void MediaView::FrameStep(const bool forwards)
{
  paused_ = true;
  ImageBuffer imagebuffer = cache_.GetImage(forwards, GetPlayRequestIndex(), sequencenum_);
  if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
  {
    videostream_->SendControlRequest<PlaybackControlFrameStepRequest>(GetPlayRequestIndex(), forwards, sequencenum_);

  }
  else
  {
    WriteFrame(imagebuffer);
    
  }

  for (boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatastream->SendControlRequest<PlaybackControlRequest>(GetPlayRequestIndex(), 0, true, forwards, time_, boost::none, 2);

  }
}

void MediaView::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  {
    std::lock_guard<std::recursive_mutex> lock(decodersmutex_);
    ResetDecoders();
  }

  const uint64_t playrequestindex = GetNextPlayRequestIndex(true);
  videostream_->SendControlRequest<PlaybackControlRequest>(playrequestindex, true, true, true, time, boost::none, numframes);
  for (boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatastream->SendControlRequest<PlaybackControlRequest>(playrequestindex, true, true, true, time, boost::none, numframes);

  }

  if (numframes.is_initialized() && ((*numframes == 0) || (*numframes == 1)))
  {
    paused_ = true;

  }
  else
  {
    paused_ = false;

  }
}

void MediaView::Pause(const boost::optional<uint64_t>& frame)
{
  SetPaused(true, frame);
  
}

void MediaView::Stop()
{
  {
    std::lock_guard<std::recursive_mutex> lock(decodersmutex_);
    ResetDecoders();
  }

  if (videostream_)
  {
    Init(videostream_->GetDeviceIndex(), videostream_->GetRecordingIndex(), videostream_->GetTrackIndex(), boost::none);

  }
}

void MediaView::Scrub(const uint64_t time)
{
  {
    std::lock_guard<std::recursive_mutex> lock(decodersmutex_);
    ResetDecoders();
  }

  paused_ = true;
  const uint64_t playrequestindex = GetNextPlayRequestIndex(false);
  videostream_->SendControlRequest<PauseTimeControlRequest>(playrequestindex, time, true);
  for (boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatastream->SendControlRequest<PlaybackControlRequest>(GetPlayRequestIndex(), 0, true, false, time_, boost::none, 5);

  }
}

bool MediaView::HasHardwareDecoder() const
{
  return (cudacontext_ && h264decoders_.size());
}

file::TRACK MediaView::GetVideoTrack() const
{
  if (!videostream_)
  {

    return file::TRACK();
  }
  return videostream_->track_;
}

std::vector<file::TRACK> MediaView::GetMetadataTracks() const
{
  std::vector<file::TRACK> metadatatracks;
  metadatatracks.reserve(metadatastreams_.size());
  for (const boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatatracks.push_back(metadatastream->track_);

  }
  return metadatatracks;
}

void MediaView::SetPaused(const bool paused, const boost::optional<uint64_t>& frame)
{
  View::SetPaused(paused);
  videostream_->SendControlRequest<PauseControlRequest>(frame);
  for (boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatastream->SendControlRequest<PauseControlRequest>(boost::none); // The video frame doesn't match up with the metadata frame

  }
}

void MediaView::ObjectDetectorCallback(const uint64_t playrequestindex, const size_t streamindex, const size_t frame)
{
  if (GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< boost::shared_ptr<MediaStream> >::const_iterator stream = std::find_if(metadatastreams_.cbegin(), metadatastreams_.cend(), [streamindex](const boost::shared_ptr<MediaStream>& metadatastream) { return (metadatastream->GetIndex() == streamindex); });
  if (stream == metadatastreams_.cend())
  {

    return;
  }

  std::shared_ptr<file::FRAMEHEADER> frameheader = (*stream)->track_.frameheaders_[frame];
  if (frameheader->Type() != file::FrameHeaderType::OBJECTDETECTOR)
  {

    return;
  }
 
  file::OBJECTDETECTORFRAMEHEADER* objectdetectorframeheader = static_cast<file::OBJECTDETECTORFRAMEHEADER*>(frameheader.get());
  if (objectdetectorframeheader->objectdetectorframetype_ != file::ObjectDetectorFrameType::OBJECT_DETECTOR)
  {

    return;
  }

  const uint64_t newsize = frameheader->size_ + 1024;
  if (buffersize_ < newsize)
  {
    buffer_ = utility::AlignedAllocArray<uint8_t>(PAGE_SIZE, newsize);
    buffersize_ = newsize;
  }

  if (media_->Read(frameheader->offset_, frameheader->size_, buffer_.get()))
  {

    return;
  }

  if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(buffer_.get()), frameheader->size_).VerifyBuffer<monocle::Objects>(nullptr))
  {

    return;
  }

  const monocle::Objects* objects = flatbuffers::GetRoot<monocle::Objects>(buffer_.get());
  if ((objects == nullptr) || (objects->objects() == nullptr))
  {

    return;
  }
  
  videowidget_->makeCurrent();
  UpdateObjects(objects, frameheader->time_);
  videowidget_->doneCurrent();
  videowidget_->update();
}

void MediaView::Init(const size_t deviceindex, const size_t recordingindex, const size_t trackindex, boost::optional<uint64_t> starttime)
{
  boost::container::flat_set<file::DEVICE>::const_iterator device = std::find_if(media_->GetMedia().GetFile().devices_.cbegin(), media_->GetMedia().GetFile().devices_.cend(), [deviceindex](const file::DEVICE& device) { return (device.index_ == deviceindex); });
  if (device == media_->GetMedia().GetFile().devices_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find device in file: ") + QString::number(deviceindex));
    return;
  }

  boost::container::flat_set<file::RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [recordingindex](const file::RECORDING& recording) { return (recording.index_ == recordingindex); });
  if (recording == device->recordings_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find recording in file: ") + QString::number(recordingindex));
    return;
  }

  boost::container::flat_set<file::TRACK>::const_iterator videotrack = std::find_if(recording->videotracks_.cbegin(), recording->videotracks_.cend(), [trackindex](const file::TRACK& track) { return (track.index_ == trackindex); });
  if (videotrack == recording->videotracks_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find track in file: ") + QString::number(trackindex));
    return;
  }
  
  {
    // Destroy the old decoders
    std::lock_guard<std::recursive_mutex> lock(decodersmutex_);
    mjpegdecoder_.reset();
    h265decoders_.clear();
    h264decoders_.clear();
    mpeg4decoders_.clear();

    // Initialise the decoders
    for (const file::CODEC& codec : videotrack->codecs_)
    {
      switch (static_cast<monocle::Codec>(codec.codec_))
      {
        case monocle::Codec::METADATA:
        {
          // Ignore for the time being
          break;
        }
        case monocle::Codec::MJPEG:
        {
          if (!mjpegdecoder_)
          {
            mjpegdecoder_ = std::make_unique<MJpegDecoder>(codec.index_, signingkey_);
            if (mjpegdecoder_->Init())
            {
              LOG_GUI_WARNING(QString("MJpegDecoder::Init failed for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_));
    
            }
          }
          break;
        }
        case monocle::Codec::H265:
        {
          if (std::find_if(h265decoders_.cbegin(), h265decoders_.cend(), [&codec](const std::unique_ptr<H265Decoder>& h265decoder) { return (h265decoder->GetId() == codec.index_); }) != h265decoders_.cend())
          {
            LOG_GUI_WARNING(QString("H265Decoder::Init duplicate H265 codec for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::number(codec.index_));
            break;
          }
    
          std::vector<std::string> parameters;
          boost::split(parameters, codec.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);
    
          std::unique_ptr<H265Decoder> h265decoder = std::make_unique<H265Decoder>(codec.index_, signingkey_);
          if (h265decoder->Init(parameters))
          {
            LOG_GUI_WARNING(QString("H265Decoder::Init failed for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::fromStdString(codec.parameters_));
            break;
          }
          h265decoders_.emplace_back(std::move(h265decoder));
          break;
        }
        case monocle::Codec::H264:
        {
          if (std::find_if(h264decoders_.cbegin(), h264decoders_.cend(), [&codec](const std::unique_ptr<H264Decoder>& h264decoder) { return (h264decoder->GetId() == codec.index_); }) != h264decoders_.cend())
          {
            LOG_GUI_WARNING(QString("H264Decoder::Init duplicate H264 codec for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::number(codec.index_));
            break;
          }
    
          std::vector<std::string> parameters;
          boost::split(parameters, codec.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);
    
          std::unique_ptr<H264Decoder> h264decoder = std::make_unique<H264Decoder>(codec.index_, signingkey_, cudacontext_);
          if (h264decoder->Init(parameters))
          {
            LOG_GUI_WARNING(QString("H264Decoder::Init failed for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::fromStdString(codec.parameters_));
            break;
          }
          h264decoders_.emplace_back(std::move(h264decoder));
          break;
        }
        case monocle::Codec::MPEG4:
        {
          if (std::find_if(mpeg4decoders_.cbegin(), mpeg4decoders_.cend(), [&codec](const std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == codec.index_); }) != mpeg4decoders_.cend())
          {
            LOG_GUI_WARNING(QString("MJpegDecoder::Init duplicate MPEG4 codec for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::number(codec.index_));
            break;
          }
    
          std::vector<std::string> parameters;
          boost::split(parameters, codec.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);
    
          std::unique_ptr<MPEG4Decoder> mpeg4decoder = std::make_unique<MPEG4Decoder>(codec.index_, signingkey_);
          if (mpeg4decoder->Init(parameters))
          {
            LOG_GUI_WARNING(QString("MPEG4Decoder::Init failed for recording: ") + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::fromStdString(codec.parameters_));
            break;
          }
          mpeg4decoders_.emplace_back(std::move(mpeg4decoder));
          break;
        }
        case monocle::Codec::OBJECTDETECTOR:
        {
          // Ignore for the time being
          break;
        }
        default:
        {
          LOG_GUI_WARNING(QString("Unrecognised codec: ") + QString::number(codec.codec_) + " " + QString::fromStdString(recording->name_) + " " + QString::number(recording->index_) + " " + QString::fromStdString(codec.parameters_));
          break;
        }
      }
    }
  }

  videolocation_ = recording->location_;
  videoname_ = recording->name_;

  // Kill the streams if they are already running
  if (videostream_)
  {
    videostream_->Destroy();
    videostream_.reset();
  }

  for (const boost::shared_ptr<MediaStream>& metadatastream : metadatastreams_)
  {
    metadatastream->Destroy();

  }
  metadatastreams_.clear();

  // If no time is selected, start from the beginning of the video track
  if (!starttime.is_initialized() && videotrack->frameheaders_.size())
  {
    starttime = videotrack->frameheaders_[0]->time_;

  }

  // Start the reading thread
  const uint64_t playrequestindex = GetNextPlayRequestIndex(true);
  videostream_ = boost::make_shared<MediaStream>(*device, *recording, *videotrack, nextstreamindex_++);
  videostream_->Init(this, true, playrequestindex);
  videostream_->SendControlRequest<PlaybackControlRequest>(playrequestindex, 0, true, true, starttime, boost::none, boost::none);
  for (const file::TRACK& objectdetectortrack : recording->objectdetectortracks_)
  {
    boost::shared_ptr<MediaStream> objectdetectorstream = boost::make_shared<MediaStream>(*device, *recording, objectdetectortrack, nextstreamindex_++);
    objectdetectorstream->Init(this, false, playrequestindex);
    objectdetectorstream->SendControlRequest<PlaybackControlRequest>(playrequestindex, 0, true, true, starttime, boost::none, boost::none);
    metadatastreams_.emplace_back(objectdetectorstream);
  }
}

void MediaView::FrameStepForwards(const bool mainstream, const uint64_t playrequestindex, const boost::shared_ptr<MediaStream>& stream, size_t& frame, std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> >& buffer, size_t& buffersize)
{
  if ((frame + 1) >= stream->track_.frameheaders_.size())
  {

    return;
  }

  const size_t currentframe = frame;
  std::vector<size_t> frames; // The frames to send

  const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[frame + 1];
  if (!frameheader->marker_) // If the next frame isn't the beginning of a GOP, we need to start from the beginning of the current GOP
  {
    // Collect all the previous frames
    for (int64_t i = static_cast<int64_t>(frame); (i >= 0) && (i > (static_cast<int64_t>(currentframe) - 500)); --i)
    {
      frames.push_back(i);
      const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[i];
      if (frameheader->marker_)
      {

        break;
      }
    }

    // Collect all the following frames up to the next GOP
    for (int64_t i = static_cast<int64_t>(frame) + 1; (i < static_cast<int64_t>(stream->track_.frameheaders_.size()) && (i < (static_cast<int64_t>(currentframe) + 500))); ++i)
    {
      const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[i];
      if (frameheader->marker_)
      {
        frame = i - 1;
        break;
      }
      else
      {
        frames.push_back(i);

      }
    }
  }

  // Gather the following GOP
  frames.push_back(frame + 1);
  for (int64_t i = static_cast<int64_t>(frame) + 2; (i < static_cast<int64_t>(stream->track_.frameheaders_.size()) && (i < (static_cast<int64_t>(currentframe) + 500))); ++i)
  {
    const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[i];
    if (frameheader->marker_)
    {

      break;
    }
    else
    {
      frames.push_back(i);

    }
  }

  // Send the frames to the GUI thread in order
  size_t nextframe = std::numeric_limits<uint64_t>::max();
  std::sort(frames.begin(), frames.end());
  for (size_t f : frames)
  {
    const std::pair<int, bool> ret = SendFrame(playrequestindex, mainstream, stream, f, buffer, buffersize);
    if (ret.first)
    {

      return;
    }
    frame = f;

    if (ret.second && (f > currentframe) && (f < nextframe)) // The callback will need to know the exacty frame to show
    {
      nextframe = f;

    }
  }

  if (nextframe == std::numeric_limits<uint64_t>::max())
  {
    // Not much we can do here
    return;
  }

  QMetaObject::invokeMethod(this, [this, playrequestindex, nextframe]() // Call the GUI thread to draw the frame
  {
    if (GetPlayRequestIndex() != playrequestindex)
    {

      return;
    }

    imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.AddImage(imagebuffer); });
    ImageBuffer imagebuffer = cache_.GetImage(playrequestindex, nextframe);
    if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
    {

      return;
    }
    WriteFrame(imagebuffer);
  }, Qt::QueuedConnection);
}

void MediaView::FrameStepBackwards(const bool mainstream, const uint64_t playrequestindex, const boost::shared_ptr<MediaStream>& stream, size_t& frame, std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> >& buffer, size_t& buffersize)
{
  if (frame == 0)
  {

    return;
  }

  const size_t currentframe = frame;
  std::vector<size_t> frames; // The frames to send in the correct order

  const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[frame];
  if (!frameheader->marker_) // Playback the current GOP up to the current frame
  {
    for (int64_t i = static_cast<int64_t>(frame) - 1; (i >= 0) && (i > (static_cast<int64_t>(currentframe) - 500)); --i)
    {
      frames.push_back(i);
      const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[i];
      if (frameheader->marker_)
      {
        frame = i;
        break;
      }
    }
  }

  // Playback the previous GOP
  for (int64_t i = static_cast<int64_t>(frame) - 1; (i >= 0) && (i > (static_cast<int64_t>(currentframe) - 500)); --i)
  {
    frames.push_back(i);
    const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[i];
    if (frameheader->marker_)
    {
      frame = i;
      break;
    }
  }

  // Send the frames to the GUI thread in order
  size_t nextframe = 0;
  std::sort(frames.begin(), frames.end());
  for (size_t f : frames)
  {
    const std::pair<int, bool> ret = SendFrame(playrequestindex, mainstream, stream, f, buffer, buffersize);
    if (ret.first)
    {

      return;
    }
    frame = f;
    
    if (ret.second && (f < currentframe) && (f > nextframe)) // The callback will need to know the exacty frame to show
    {
      nextframe = f;

    }
  }

  if (nextframe == 0)
  {
    // Not much we can do here
    return;
  }

  QMetaObject::invokeMethod(this, [this, playrequestindex, nextframe]() // Call the GUI thread to draw the frame
  {
    if (GetPlayRequestIndex() != playrequestindex)
    {

      return;
    }

    imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.AddImage(imagebuffer); });
    ImageBuffer imagebuffer = cache_.GetImage(playrequestindex, nextframe);
    if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
    {

      return;
    }
    WriteFrame(imagebuffer);
  }, Qt::QueuedConnection);
}

std::pair<int, bool> MediaView::SendFrame(const uint64_t playrequestindex, const bool mainstream, const boost::shared_ptr<MediaStream>& stream, const size_t frame, std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> >& buffer, size_t& buffersize)
{
  const std::shared_ptr<file::FRAMEHEADER> frameheader = stream->track_.frameheaders_[frame];

  // Make sure the buffer is big enough
  const uint64_t newsize = frameheader->size_ + 1024; // We may need a bit of extra space for the H264 or JPEG headers
  if (buffersize < newsize)
  {
    buffer = utility::AlignedAllocArray<uint8_t>(PAGE_SIZE, newsize);
    buffersize = newsize;
  }
  
  const uint8_t* signature = nullptr;
  if (frameheader->signature_.size())
  {
    signature = reinterpret_cast<const uint8_t*>(frameheader->signature_.data());

  }

  // Decode and send the frame
  std::lock_guard<std::recursive_mutex> lock(decodersmutex_);
  ImageBuffer imagebuffer;
  bool newframe = false;
  if (mainstream && (frameheader->Type() == file::FrameHeaderType::H265))
  {
    std::vector< std::unique_ptr<H265Decoder> >::iterator h265decoder = std::find_if(h265decoders_.begin(), h265decoders_.end(), [&frameheader](const std::unique_ptr<H265Decoder>& h265decoder) { return (h265decoder->GetId() == frameheader->codecindex_); });
    if (h265decoder != h265decoders_.end())
    {
      // Read the frame
      if (media_->Read(frameheader->offset_, frameheader->size_, buffer.get()))
      {

        return std::make_pair(1, false);
      }

      const std::shared_ptr<file::H265FRAMEHEADER> h265frameheader = std::static_pointer_cast<file::H265FRAMEHEADER>(frameheader);
      std::vector<unsigned char> buf;
      if (BuildH265Frame(h265frameheader->donlfield_, h265frameheader->offsets_.data(), h265frameheader->offsets_.size(), reinterpret_cast<const char*>(buffer.get()), frameheader->size_, buf))
      {

        return std::make_pair(2, false);
      }

      if (buf.empty())
      {

        return std::make_pair(3, false);
      }

      imagebuffer = (*h265decoder)->Decode(playrequestindex, frameheader->marker_, frameheader->time_, frame, signature, frameheader->signature_.size(), reinterpret_cast<const char*>(buffer.get()), frameheader->size_, reinterpret_cast<const uint8_t*>(buf.data()), static_cast<int>(buf.size()), &freeimagequeue_);
      if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
      {
        if (imagequeue_.write_available())
        {
          newframe = true;
          imagequeue_.push(imagebuffer);
        }
        else
        {
          imagebuffer.Destroy();

        }
      }
    }
  }
  else if (mainstream && (frameheader->Type() == file::FrameHeaderType::H264))
  {
    std::vector< std::unique_ptr<H264Decoder> >::iterator h264decoder = std::find_if(h264decoders_.begin(), h264decoders_.end(), [&frameheader](const std::unique_ptr<H264Decoder>& h264decoder) { return (h264decoder->GetId() == frameheader->codecindex_); });
    if (h264decoder != h264decoders_.end())
    {
      // Read the frame
      if (media_->Read(frameheader->offset_, frameheader->size_, buffer.get()))
      {

        return std::make_pair(1, false);
      }

      const std::shared_ptr<file::H264FRAMEHEADER> h264frameheader = std::static_pointer_cast<file::H264FRAMEHEADER>(frameheader);
      std::vector<unsigned char> buf;
      if (BuildH264Frame(h264frameheader->offsets_.data(), h264frameheader->offsets_.size(), reinterpret_cast<const char*>(buffer.get()), frameheader->size_, buf))
      {

        return std::make_pair(2, false);
      }

      if (buf.empty())
      {

        return std::make_pair(3, false);
      }

      imagebuffer = (*h264decoder)->Decode(playrequestindex, frameheader->marker_, frameheader->time_, frame, signature, frameheader->signature_.size(), reinterpret_cast<const char*>(buffer.get()), frameheader->size_, reinterpret_cast<const uint8_t*>(buf.data()), static_cast<int>(buf.size()), &freeimagequeue_);
      if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
      {
        if (imagequeue_.write_available())
        {
          newframe = true;
          imagequeue_.push(imagebuffer);
        }
        else
        {
          imagebuffer.Destroy();

        }
      }
    }
  }
  else if (mainstream && (frameheader->Type() == file::FrameHeaderType::MPEG4))
  {
    std::vector< std::unique_ptr<MPEG4Decoder> >::iterator mpeg4decoder = std::find_if(mpeg4decoders_.begin(), mpeg4decoders_.end(), [&frameheader](const std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == frameheader->codecindex_); });
    if (mpeg4decoder != mpeg4decoders_.end())
    {
      // Read the frame
      if (media_->Read(frameheader->offset_, frameheader->size_, buffer.get()))
      {

        return std::make_pair(1, false);
      }

      imagebuffer = (*mpeg4decoder)->Decode(playrequestindex, frameheader->time_, frame, signature, frameheader->signature_.size(), reinterpret_cast<const char*>(buffer.get()), frameheader->size_, reinterpret_cast<const uint8_t*>(buffer.get()), static_cast<int>(frameheader->size_), &freeimagequeue_);
      if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
      {
        if (imagequeue_.write_available())
        {
          newframe = true;
          imagequeue_.push(imagebuffer);
        }
        else
        {
          imagebuffer.Destroy();

        }
      }
    }
  }
  else if (mainstream && (frameheader->Type() == file::FrameHeaderType::JPEG))
  {
    if (mjpegdecoder_ && (mjpegdecoder_->GetId() == frameheader->codecindex_))
    {
      const std::shared_ptr<file::JPEGFRAMEHEADER> jpegframeheader = std::static_pointer_cast<file::JPEGFRAMEHEADER>(frameheader);
      std::vector<unsigned char> tmp;
      JPEGHeader(jpegframeheader->restartinterval_, jpegframeheader->typespecificfragmentoffset_, jpegframeheader->type_, jpegframeheader->q_, jpegframeheader->width_, jpegframeheader->height_, jpegframeheader->lqt_.data(), jpegframeheader->cqt_.data(), tmp);
      std::memcpy(buffer.get(), tmp.data(), tmp.size());

      // Read the frame
      if (media_->Read(frameheader->offset_, frameheader->size_, buffer.get() + tmp.size()))
      {

        return std::make_pair(1, false);
      }

      imagebuffer = mjpegdecoder_->Decode(playrequestindex, frameheader->time_, frame, signature, frameheader->signature_.size(), reinterpret_cast<const char*>(buffer.get()) + tmp.size(), frameheader->size_, reinterpret_cast<const uint8_t*>(buffer.get()), static_cast<unsigned int>(frameheader->size_ + tmp.size()), &freeimagequeue_);
      if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
      {
        if (imagequeue_.write_available())
        {
          newframe = true;
          imagequeue_.push(imagebuffer);
        }
        else
        {
          imagebuffer.Destroy();

        }
      }
    }
  }
  else if (!mainstream && (frameheader->Type() == file::FrameHeaderType::OBJECTDETECTOR))
  {
    QMetaObject::invokeMethod(this, [this, playrequestindex, stream, frame]() { ObjectDetectorCallback(playrequestindex, stream->index_, frame); }, Qt::QueuedConnection);

  }
  else
  {
    // Ignore

  }

  return std::make_pair(0, newframe);
}

void MediaView::ResetDecoders()
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

void MediaView::PlayThread(boost::shared_ptr<MediaStream> stream, const bool mainstream, uint64_t playrequestindex, std::future< std::unique_ptr<ControlRequest> >&& future)
{
  std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> > buffer;
  size_t buffersize = 0;
  size_t frame = 0;
  std::chrono::milliseconds delay(0);
  uint64_t starttime = 0;
  bool ratecontrol = true;
  bool forwards = true;
  bool playing = true;
  boost::optional<uint64_t> numframes;
  std::chrono::steady_clock::time_point requesttime;
  bool scrub = false;
  bool iframes = false;
  while (stream->IsRunning())
  {
    if (mainstream && numframes.is_initialized() && (*numframes == 0))
    {
      QMetaObject::invokeMethod(this, [this, scrub, playrequestindex]()
      {
        imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.AddImage(imagebuffer); });
        ImageBuffer imagebuffer;
        if (scrub)
        {
          imagebuffer = cache_.GetLatestImage(playrequestindex);

        }
        else
        {
          imagebuffer = cache_.GetLatestImageBySequence(playrequestindex);

        }

        if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
        {

          return;
        }
        WriteFrame(imagebuffer);
        cache_.Clear();
      }, Qt::QueuedConnection);
      playing = false;
    }

    std::unique_ptr<ControlRequest> controlrequest;
    if (!playing || (future.wait_for(abs(delay)) == std::future_status::ready))
    {
      numframes.reset();
      controlrequest = future.get();
    }

    if (controlrequest)
    {
      if (controlrequest->GetControlType() == CONTROLTYPE_DESTROY)
      {

        break;
      }
      else if (controlrequest->GetControlType() == CONTROLTYPE_PAUSETIME)
      {
        PauseTimeControlRequest* pausetimecontrolrequest = reinterpret_cast<PauseTimeControlRequest*>(controlrequest.get());
        playrequestindex = pausetimecontrolrequest->playrequestindex_;
        ratecontrol = false;
        forwards = true;
        playing = true; // We will pause when we are done with the frames...
        frame = 0;
        numframes = 1;
        scrub = true;
        iframes = pausetimecontrolrequest->iframes_;

        auto frameheader = stream->GetFrame(pausetimecontrolrequest->time_);
        if (frameheader == stream->track_.frameheaders_.cend())
        {
          // This shouldn't really happen given we check this earlier, but lets just start at the beginning just in case
          frame = 0;
        }
        else
        {
          frame = std::distance(stream->track_.frameheaders_.cbegin(), frameheader);
          for (uint64_t i = frame; (i > (frame - 500)) && (i <= frame); --i)
          {
            if (stream->track_.frameheaders_[i]->marker_)
            {
              if (!iframes && numframes.is_initialized())
              {
                *numframes += (frame - i);

              }
              frame = i;
              break;
            }
          }
        }
      }
      else if (controlrequest->GetControlType() == CONTROLTYPE_PAUSE)
      {
        PauseControlRequest* pausecontrolrequest = reinterpret_cast<PauseControlRequest*>(controlrequest.get());
        ratecontrol = false;
        forwards = true;
        playing = false;
        numframes.reset();
        scrub = false;
        iframes = false;
        if (pausecontrolrequest->frame_.is_initialized())
        {
          frame = *pausecontrolrequest->frame_;

        }
      }
      else if (controlrequest->GetControlType() == CONTROLTYPE_PLAYBACK)
      {
        scrub = false;
        iframes = false;
        if (stream->track_.frameheaders_.empty())
        {
          playing = false;
          frame = 0;
        }
        else
        {
          PlaybackControlRequest* playbackcontrolrequest = reinterpret_cast<PlaybackControlRequest*>(controlrequest.get());
          playrequestindex = playbackcontrolrequest->playrequestindex_;
          ratecontrol = playbackcontrolrequest->ratecontrol_;
          forwards = playbackcontrolrequest->forwards_;
          numframes = playbackcontrolrequest->numframes_;
          uint64_t time = 0;
          if (playbackcontrolrequest->starttime_.is_initialized())
          {
            if (stream->track_.frameheaders_[0]->time_ > *playbackcontrolrequest->starttime_)
            {
              time = stream->track_.frameheaders_[0]->time_;
              if (ratecontrol)
              {
                delay = std::chrono::milliseconds(stream->track_.frameheaders_[0]->time_ - *playbackcontrolrequest->starttime_);
                requesttime = std::chrono::steady_clock::now() + delay;
              }
              else
              {
                delay = std::chrono::milliseconds::zero();
                requesttime = std::chrono::steady_clock::now();
              }
            }
            else
            {
              time = *playbackcontrolrequest->starttime_;
              delay = std::chrono::milliseconds::zero();
              requesttime = std::chrono::steady_clock::now();
            }
          }
          else // If no time is specified, start at the beginning
          {
            frame = 0;
            time = stream->track_.frameheaders_[0]->time_;
            delay = std::chrono::milliseconds::zero();
            requesttime = std::chrono::steady_clock::now();
          }
          playing = true;
          starttime = time;

          std::vector< std::shared_ptr<file::FRAMEHEADER> >::const_iterator frameheader = stream->GetFrame(time);
          if (frameheader == stream->track_.frameheaders_.cend())
          {
            // This shouldn't really happen given we check this earlier, but lets just start at the beginning just in case
            frame = 0;
          }
          else
          {
            frame = std::distance(stream->track_.frameheaders_.cbegin(), frameheader);
            if (forwards && playbackcontrolrequest->fetchmarker_) // Search for previous iframe
            {
              for (uint64_t i = frame; (i > (frame - 500)) && (i <= frame); --i)
              {
                if (stream->track_.frameheaders_[i]->marker_)
                {
                  if (numframes.is_initialized())
                  {
                    *numframes += (frame - i);

                  }
                  frame = i;
                  break;
                }
              }
            }
          }
        }
      }
      else if (controlrequest->GetControlType() == CONTROLTYPE_FRAMESTEP)
      {
        PlaybackControlFrameStepRequest* playbackcontrolframesteprequest = reinterpret_cast<PlaybackControlFrameStepRequest*>(controlrequest.get());
        playing = false;
        scrub = false;
        iframes = false;
        playrequestindex = playbackcontrolframesteprequest->playrequestindex_;

        {
          std::lock_guard<std::recursive_mutex> lock(decodersmutex_);
          ResetDecoders();
        }

        if (playbackcontrolframesteprequest->forwards_)
        {
          FrameStepForwards(mainstream, playrequestindex, stream, playbackcontrolframesteprequest->frame_, buffer, buffersize);

        }
        else
        {
          FrameStepBackwards(mainstream, playrequestindex, stream, playbackcontrolframesteprequest->frame_, buffer, buffersize);

        }
      }
      else
      {
        // Ignore

      }
      future = controlrequest->GetNextRequest();
    }
    else
    {
      if (frame >= stream->track_.frameheaders_.size())
      {
        playing = false;
        if (mainstream)
        {
          paused_ = true;

        }
        break;
      }

      if (SendFrame(playrequestindex, mainstream, stream, frame, buffer, buffersize).first)
      {
        if (mainstream)
        {
          paused_ = true;

        }
        break;
      }

      if (numframes.is_initialized())
      {
        --(*numframes);

      }

      if (forwards)
      {
        ++frame;

      }
      else
      {
        if (frame != 0)
        {
          --frame;

        }
      }

      if ((forwards && (frame >= stream->track_.frameheaders_.size())) || (!forwards && (frame <= 0)))
      {
        playing = false;
        if (mainstream)
        {
          paused_ = true;

        }
        continue;
      }

      const std::shared_ptr<file::FRAMEHEADER> nextframe = stream->track_.frameheaders_[frame];
      if ((ratecontrol == false) || (starttime >= nextframe->time_))
      {
        delay = std::chrono::milliseconds(0);

      }
      else
      {
        const uint64_t frameduration = (nextframe->time_ - starttime);
        const uint64_t realduration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - requesttime).count();
        if (frameduration >= realduration)
        {
          delay = std::chrono::milliseconds(static_cast<int64_t>(frameduration) - static_cast<int64_t>(realduration));

        }
        else
        {
          delay = std::chrono::milliseconds(0);

        }
      }
    }
  }
}

}
