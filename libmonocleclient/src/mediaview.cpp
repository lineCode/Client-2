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

   PauseTimeControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const uint64_t time);

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_PAUSETIME; }

  uint64_t playrequestindex_;
  uint64_t time_;

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

  PlaybackControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes);

  inline CONTROLTYPE GetControlType() const { return CONTROLTYPE_PLAYBACK; }

  uint64_t playrequestindex_;
  bool fetchmarker_;
  bool ratecontrol_;
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

PlaybackControlRequest::PlaybackControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes) :
  ControlRequest(std::move(nextcontrolrequest)),
  playrequestindex_(playrequestindex),
  fetchmarker_(fetchmarker),
  ratecontrol_(ratecontrol),
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

PauseTimeControlRequest::PauseTimeControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const uint64_t playrequestindex, const uint64_t time) :
  ControlRequest(std::move(nextcontrolrequest)),
  playrequestindex_(playrequestindex),
  time_(time)
{

}

PauseControlRequest::PauseControlRequest(std::future< std::unique_ptr<ControlRequest> >&& nextcontrolrequest, const boost::optional<uint64_t>& frame) :
  ControlRequest(std::move(nextcontrolrequest)),
  frame_(frame)
{

}

MediaView::MediaView(VideoWidget* videowidget, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool info, const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const QResource* arial) :
  View(videowidget, selectedcolour, x, y, width, height, rotation, mirror, stretch, info, arial, true, true, true),
  media_(media),
  deviceindex_(deviceindex),
  recordingindex_(recordingindex),
  trackindex_(trackindex)
{
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  SetMessage(GetPlayRequestIndex(), false, QString("Loading Track"));

  boost::container::flat_set<file::DEVICE>::const_iterator device = std::find_if(media_->GetMedia().GetFile().devices_.cbegin(), media_->GetMedia().GetFile().devices_.cend(), [deviceindex](const file::DEVICE& device) { return (device.index_ == deviceindex); });
  if (device == media_->GetMedia().GetFile().devices_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find device in file: ") + QString::number(deviceindex));
    return;
  }
  device_ = *device;

  boost::container::flat_set<file::RECORDING>::const_iterator recording = std::find_if(device->recordings_.cbegin(), device->recordings_.cend(), [recordingindex](const file::RECORDING& recording) { return (recording.index_ == recordingindex); });
  if (recording == device->recordings_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find recording in file: ") + QString::number(recordingindex));
    return;
  }
  recording_ = *recording;

  boost::container::flat_set<file::TRACK>::const_iterator track = std::find_if(recording->tracks_.cbegin(), recording->tracks_.cend(), [trackindex](const file::TRACK& track) { return (track.index_ == trackindex); });
  if (track == recording->tracks_.cend())
  {
    SetMessage(GetPlayRequestIndex(), true, QString("Unable to find track in file: ") + QString::number(trackindex));
    return;
  }
  currenttrack_ = *track;

  // Load public key
  if (device->signingkey_.size())
  {
    if (signingkey_.Init(device->signingkey_))
    {
      LOG_GUI_WARNING(QString("PublicKey::Init failed to initialise"));

    }
  }

  Init(boost::none);
}

MediaView::~MediaView()
{
  running_ = false;
  promise_.set_value(std::make_unique<DestroyControlRequest>());
  thread_.join();
}

void MediaView::GetMenu(QMenu& parent)
{
  QMenu* tracks = new QMenu("Tracks", &parent);
  for (const file::TRACK& track : recording_.tracks_)
  {
    const QString description = track.description_.empty() ? QString() : QString("(" + QString::fromStdString(track.description_) + ")");
    QAction* action = tracks->addAction(QString::number(track.index_) + description, [this, trackid = track.index_]()
    {
      if (trackid == currenttrack_.index_) // Ignore switching to ourself
      {

        return;
      }

      boost::container::flat_set<file::TRACK>::const_iterator track = std::find_if(recording_.tracks_.cbegin(), recording_.tracks_.cend(), [trackid](const file::TRACK& track) { return (track.index_ == trackid); });
      if (track == recording_.tracks_.cend())
      {
        LOG_GUI_WARNING(QString("Unable to find track: ") + QString::number(trackid));
        return;
      }
      currenttrack_ = *track;
      Init(time_); // Restart playback on another track
    });
    action->setCheckable(true);
    action->setChecked(track.index_ == currenttrack_.index_);
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
      if (previmagebuffer.buffer_)
      {
        if (paused_)
        {
          cache_.push_back(previmagebuffer);

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
      if (imagebuffer.buffer_)
      {
        imagewidth_ = imagebuffer.widths_[0];
        imageheight_ = imagebuffer.heights_[0];
      }
      return hasimage;
    }
  }
  return true;
}

void MediaView::Play()
{
  //TODO ResetDecoders here...

  paused_ = false;
  SendControlRequest<PlaybackControlRequest>(GetNextPlayRequestIndex(true), false, true, time_, boost::none, boost::none);
}

void MediaView::FrameStep(const bool forwards)
{
  paused_ = true;
  std::vector<ImageBuffer>::iterator imagebuffer = cache_.end();
  for (std::vector<ImageBuffer>::iterator i = cache_.begin(); i != cache_.end(); ++i)
  {
    if (i->playrequestindex_ != GetPlayRequestIndex())
    {

      continue;
    }

    if (forwards)
    {
      if ((i->sequencenum_ > sequencenum_) && (imagebuffer == cache_.end() || ((imagebuffer != cache_.end()) && (i->sequencenum_ < imagebuffer->sequencenum_))))
      {
        imagebuffer = i;

      }
    }
    else
    {
      if ((i->sequencenum_ < sequencenum_) && (imagebuffer == cache_.end() || ((imagebuffer != cache_.end()) && (i->sequencenum_ > imagebuffer->sequencenum_))))
      {
        imagebuffer = i;

      }
    }
  }

  if (imagebuffer == cache_.end())
  {
    SendControlRequest<PlaybackControlFrameStepRequest>(GetNextPlayRequestIndex(true), forwards, sequencenum_);

  }
  else
  {
    WriteFrame(*imagebuffer);
    
  }
}

void MediaView::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    ResetDecoders();
  }

  SendControlRequest<PlaybackControlRequest>(GetNextPlayRequestIndex(true), true, true, time, boost::none, numframes);
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
  paused_ = false;
  SendControlRequest<PlaybackControlRequest>(GetNextPlayRequestIndex(true), true, true, boost::none, boost::none, boost::none);
}

void MediaView::Scrub(const uint64_t time)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    ResetDecoders();
  }

  SendControlRequest<PauseTimeControlRequest>(GetNextPlayRequestIndex(false), time);
  paused_ = true;
}

std::vector<int> MediaView::GetCUDADevices() const
{
  std::lock_guard<std::mutex> lock(mutex_);
  std::vector<int> cudadevices;
  for (const std::unique_ptr<H264Decoder>& h264decoder : h264decoders_)
  {
    if (h264decoder->GetHardwareDevice().is_initialized())
    {
      if (utility::Contains(cudadevices, *h264decoder->GetHardwareDevice()))
      {

        continue;
      }
      cudadevices.push_back(*h264decoder->GetHardwareDevice());
    }
  }
  return cudadevices;
}

void MediaView::SetPaused(const bool paused, const boost::optional<uint64_t>& frame)
{
  View::SetPaused(paused);
  SendControlRequest<PauseControlRequest>(frame);
}

void MediaView::Init(const boost::optional<uint64_t>& starttime)
{
  // Kill the thread if it is already running
  if (thread_.joinable())
  {
    running_ = false;
    promise_.set_value(std::make_unique<DestroyControlRequest>());
    thread_.join();
    promise_ = std::promise< std::unique_ptr<ControlRequest> >();
  }

  std::lock_guard<std::mutex> lock(mutex_);
  mjpegdecoder_.reset();
  h265decoders_.clear();
  h264decoders_.clear();

  // Initialise the codecs
  for (const file::CODEC& codec : currenttrack_.codecs_)
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
            LOG_GUI_WARNING(QString("MJpegDecoder::Init failed for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_));

          }
        }
        break;
      }
      case monocle::Codec::H265:
      {
        if (std::find_if(h265decoders_.cbegin(), h265decoders_.cend(), [&codec](const std::unique_ptr<H265Decoder>& h265decoder) { return (h265decoder->GetId() == codec.index_); }) != h265decoders_.cend())
        {
          LOG_GUI_WARNING(QString("H265Decoder::Init duplicate H265 codec for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_) + " " + QString::number(codec.index_));
          break;
        }

        std::vector<std::string> parameters;
        boost::split(parameters, codec.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);

        std::unique_ptr<H265Decoder> h265decoder = std::make_unique<H265Decoder>(codec.index_, signingkey_);
        if (h265decoder->Init(parameters))
        {
          LOG_GUI_WARNING(QString("H265Decoder::Init failed for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_) + " " + QString::fromStdString(codec.parameters_));
          break;
        }
        h265decoders_.emplace_back(std::move(h265decoder));
        break;
      }
      case monocle::Codec::H264:
      {
        if (std::find_if(h264decoders_.cbegin(), h264decoders_.cend(), [&codec](const std::unique_ptr<H264Decoder>& h264decoder) { return (h264decoder->GetId() == codec.index_); }) != h264decoders_.cend())
        {
          LOG_GUI_WARNING(QString("H264Decoder::Init duplicate H264 codec for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_) + " " + QString::number(codec.index_));
          break;
        }

        std::vector<std::string> parameters;
        boost::split(parameters, codec.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);

        std::unique_ptr<H264Decoder> h264decoder = std::make_unique<H264Decoder>(codec.index_, signingkey_);
        if (h264decoder->Init(parameters))
        {
          LOG_GUI_WARNING(QString("H264Decoder::Init failed for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_) + " " + QString::fromStdString(codec.parameters_));
          break;
        }
        h264decoders_.emplace_back(std::move(h264decoder));
        break;
      }
      case monocle::Codec::MPEG4:
      {
        if (std::find_if(mpeg4decoders_.cbegin(), mpeg4decoders_.cend(), [&codec](const std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == codec.index_); }) != mpeg4decoders_.cend())
        {
          LOG_GUI_WARNING(QString("MJpegDecoder::Init duplicate MPEG4 codec for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_) + " " + QString::number(codec.index_));
          break;
        }

        std::vector<std::string> parameters;
        boost::split(parameters, codec.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);

        std::unique_ptr<MPEG4Decoder> mpeg4decoder = std::make_unique<MPEG4Decoder>(codec.index_, signingkey_);
        if (mpeg4decoder->Init(parameters))
        {
          LOG_GUI_WARNING(QString("MPEG4Decoder::Init failed for recording: ") + QString::fromStdString(recording_.name_) + " " + QString::number(recording_.index_) + " " + QString::fromStdString(codec.parameters_));
          break;
        }
        mpeg4decoders_.emplace_back(std::move(mpeg4decoder));
        break;
      }
      default:
      {
        // Unrecognised codec, log message
        break;
      }
    }
  }

  // Start the reading thread
  running_ = true;
  std::future< std::unique_ptr<ControlRequest> > future = promise_.get_future();
  SendControlRequest<PlaybackControlRequest>(GetPlayRequestIndex(), 0, true, starttime, boost::none, boost::none);
  thread_ = std::thread([this, playrequestindex = GetPlayRequestIndex(), future = std::move(future)]() mutable
  {
    std::unique_ptr< char[], utility::DeleteAligned<char> > buffer;
    size_t buffersize = 0;
    size_t frame = 0;
    std::chrono::milliseconds delay(0);
    uint64_t starttime = 0;
    bool ratecontrol = true;
    bool playing = true;
    boost::optional<uint64_t> numframes;
    std::chrono::steady_clock::time_point requesttime;
    bool scrub = false;
    while (running_)
    {
      if (numframes.is_initialized() && (*numframes == 0))
      {
        QMetaObject::invokeMethod(this, [this, scrub, playrequestindex]()
        {
          imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.push_back(imagebuffer); });
          std::vector<ImageBuffer>::const_iterator imagebuffer = cache_.end();
          if (scrub)
          {
            // Find the latest play request
            if (cache_.size())
            {
              std::sort(cache_.begin(), cache_.end(), [](const ImageBuffer& lhs, const ImageBuffer& rhs)
              {
                if (lhs.playrequestindex_ == rhs.playrequestindex_)
                {

                  return (lhs.time_ < rhs.time_);
                }
                else
                {

                  return (lhs.playrequestindex_ < rhs.playrequestindex_);
                }
              });

              imagebuffer = cache_.end() - 1;
            }
          }
          else
          {
            if (GetPlayRequestIndex() != playrequestindex)
            {
              
              return;
            }

            for (std::vector<ImageBuffer>::const_iterator i = cache_.begin(); i != cache_.end(); ++i)
            {
              if (i->playrequestindex_ != playrequestindex)
              {

                continue;
              }

              if (imagebuffer == cache_.end())
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
          }

          if (imagebuffer == cache_.end())
          {

            return;
          }

          WriteFrame(*imagebuffer);
          std::for_each(cache_.begin(), cache_.end(), [](ImageBuffer& imagebuffer) { imagebuffer.Destroy(); });
          cache_.clear();
        }, Qt::QueuedConnection);
        playing = false;
      }

      std::unique_ptr<ControlRequest> controlrequest;
      if (!playing || (future.wait_for(std::chrono::milliseconds(delay)) == std::future_status::ready))
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
          playing = true; // We will pause when we are done with the frames...
          frame = 0;
          numframes = 1;
          scrub = true;

          //TODO this can be really REALLY Slow on large files... binary search I think
          std::vector< std::shared_ptr<file::FRAMEHEADER> >::const_reverse_iterator frameheader = std::find_if(currenttrack_.frameheaders_.crbegin(), currenttrack_.frameheaders_.crend(), [pausetimecontrolrequest](const std::shared_ptr<file::FRAMEHEADER>& frameheader) { return (frameheader->time_ <= pausetimecontrolrequest->time_); });
          if (frameheader == currenttrack_.frameheaders_.crend())
          {
            // This shouldn't really happen given we check this earlier, but lets just start at the beginning just in case
            frame = 0;
          }
          else
          {
            frame = (currenttrack_.frameheaders_.size() - 1) - std::distance(currenttrack_.frameheaders_.crbegin(), frameheader);
            for (uint64_t i = frame; (i > (frame - 500)) && (i <= frame); --i)
            {
              if (currenttrack_.frameheaders_[i]->marker_)
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
        else if (controlrequest->GetControlType() == CONTROLTYPE_PAUSE)
        {
          PauseControlRequest* pausecontrolrequest = reinterpret_cast<PauseControlRequest*>(controlrequest.get());
          ratecontrol = false;
          playing = false;
          numframes.reset();
          scrub = false;
          if (pausecontrolrequest->frame_.is_initialized())
          {
            frame = *pausecontrolrequest->frame_;

          }
        }
        else if (controlrequest->GetControlType() == CONTROLTYPE_PLAYBACK)
        {
          scrub = false;
          if (currenttrack_.frameheaders_.empty())
          {
            playing = false;
            frame = 0;
          }
          else
          {
            PlaybackControlRequest* playbackcontrolrequest = reinterpret_cast<PlaybackControlRequest*>(controlrequest.get());
            playrequestindex = playbackcontrolrequest->playrequestindex_;
            ratecontrol = playbackcontrolrequest->ratecontrol_;
            numframes = playbackcontrolrequest->numframes_;
            uint64_t time = 0;
            if (playbackcontrolrequest->starttime_.is_initialized())
            {
              if (currenttrack_.frameheaders_[0]->time_ > *playbackcontrolrequest->starttime_)
              {
                time = currenttrack_.frameheaders_[0]->time_;
                if (ratecontrol)
                {
                  delay = std::chrono::milliseconds(currenttrack_.frameheaders_[0]->time_ - *playbackcontrolrequest->starttime_);
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
              time = currenttrack_.frameheaders_[0]->time_;
              delay = std::chrono::milliseconds::zero();
              requesttime = std::chrono::steady_clock::now();
            }
            playing = true;
            starttime = time;

            //TODO this can be really REALLY Slow on large files... binary search I think
            std::vector< std::shared_ptr<file::FRAMEHEADER> >::const_reverse_iterator frameheader = std::find_if(currenttrack_.frameheaders_.crbegin(), currenttrack_.frameheaders_.crend(), [time](const std::shared_ptr<file::FRAMEHEADER>& frameheader) { return (frameheader->time_ <= time); });
            if (frameheader == currenttrack_.frameheaders_.crend())
            {
              // This shouldn't really happen given we check this earlier, but lets just start at the beginning just in case
              frame = 0;
            }
            else
            {
              frame = (currenttrack_.frameheaders_.size() - 1) - std::distance(currenttrack_.frameheaders_.crbegin(), frameheader);
              if (playbackcontrolrequest->fetchmarker_) // Search for previous iframe
              {
                for (uint64_t i = frame; (i > (frame - 500)) && (i <= frame); --i)
                {
                  if (currenttrack_.frameheaders_[i]->marker_)
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
          playrequestindex = playbackcontrolframesteprequest->playrequestindex_;
          if (playbackcontrolframesteprequest->forwards_)
          {
            FrameStepForwards(playrequestindex, playbackcontrolframesteprequest->frame_, buffer, buffersize);

          }
          else
          {
            FrameStepBackwards(playrequestindex, playbackcontrolframesteprequest->frame_, buffer, buffersize);

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
        if (frame >= currenttrack_.frameheaders_.size())
        {
          playing = false;
          paused_ = true;
          break;
        }

        if (SendFrame(playrequestindex, frame, buffer, buffersize).first)
        {
          paused_ = true;
          break;
        }

        if (numframes.is_initialized())
        {
          --(*numframes);

        }

        if (++frame >= currenttrack_.frameheaders_.size())
        {
          playing = false;
          paused_ = true;
          continue;
        }

        const std::shared_ptr<file::FRAMEHEADER> nextframe = currenttrack_.frameheaders_[frame];
        if (ratecontrol == false || (starttime >= nextframe->time_))
        {
          delay = std::chrono::milliseconds(0);

        }
        else
        {
          const uint64_t frameduration = (nextframe->time_ - starttime);
          const uint64_t realduration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - requesttime).count();
          if (frameduration >= realduration)
          {
            delay = std::chrono::milliseconds(frameduration - realduration);

          }
          else
          {
            delay = std::chrono::milliseconds(0);

          }
        }
      }
    }
  });
}

void MediaView::FrameStepForwards(const uint64_t playrequestindex, size_t& frame, std::unique_ptr< char[], utility::DeleteAligned<char> >& buffer, size_t& buffersize)
{
  if ((frame + 1) >= currenttrack_.frameheaders_.size())
  {

    return;
  }

  const size_t currentframe = frame;
  std::vector<size_t> frames; // The frames to send

  const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[frame + 1];
  if (!frameheader->marker_) // If the next frame isn't the beginning of a GOP, we need to start from the beginning of the current GOP
  {
    // Collect all the previous frames
    for (int64_t i = static_cast<int64_t>(frame); (i >= 0) && (i > (static_cast<int64_t>(currentframe) - 500)); --i)
    {
      frames.push_back(i);
      const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[i];
      if (frameheader->marker_)
      {

        break;
      }
    }

    // Collect all the following frames up to the next GOP
    for (int64_t i = static_cast<int64_t>(frame) + 1; (i < static_cast<int64_t>(currenttrack_.frameheaders_.size()) && (i < (static_cast<int64_t>(currentframe) + 500))); ++i)
    {
      const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[i];
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
  for (int64_t i = static_cast<int64_t>(frame) + 2; (i < static_cast<int64_t>(currenttrack_.frameheaders_.size()) && (i < (static_cast<int64_t>(currentframe) + 500))); ++i)
  {
    const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[i];
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
    const std::pair<int, bool> ret = SendFrame(playrequestindex, f, buffer, buffersize);
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

    imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.push_back(imagebuffer); });

    std::vector<ImageBuffer>::const_iterator imagebuffer = std::find_if(cache_.cbegin(), cache_.cend(), [playrequestindex, nextframe](const ImageBuffer& imagebuffer) { return ((imagebuffer.playrequestindex_ == playrequestindex) && (imagebuffer.sequencenum_ == nextframe)); });
    if (imagebuffer == cache_.cend())
    {

      return;
    }

    WriteFrame(*imagebuffer);
  }, Qt::QueuedConnection);
}

void MediaView::FrameStepBackwards(const uint64_t playrequestindex, size_t& frame, std::unique_ptr< char[], utility::DeleteAligned<char> >& buffer, size_t& buffersize)
{
  if (frame == 0)
  {

    return;
  }

  const size_t currentframe = frame;
  std::vector<size_t> frames; // The frames to send in the correct order

  const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[frame];
  if (!frameheader->marker_) // Playback the current GOP up to the current frame
  {
    for (int64_t i = static_cast<int64_t>(frame) - 1; (i >= 0) && (i > (static_cast<int64_t>(currentframe) - 500)); --i)
    {
      frames.push_back(i);
      const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[i];
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
    const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[i];
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
    const std::pair<int, bool> ret = SendFrame(playrequestindex, f, buffer, buffersize);
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

    imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.push_back(imagebuffer); });

    std::vector<ImageBuffer>::const_iterator imagebuffer = std::find_if(cache_.cbegin(), cache_.cend(), [playrequestindex, nextframe](const ImageBuffer& imagebuffer) { return ((imagebuffer.playrequestindex_ == playrequestindex) && (imagebuffer.sequencenum_ == nextframe)); });
    if (imagebuffer == cache_.cend())
    {

      return;
    }
    WriteFrame(*imagebuffer);
  }, Qt::QueuedConnection);
}

std::pair<int, bool> MediaView::SendFrame(const uint64_t playrequestindex, const size_t frame, std::unique_ptr< char[], utility::DeleteAligned<char> >& buffer, size_t& buffersize)
{
  const std::shared_ptr<file::FRAMEHEADER> frameheader = currenttrack_.frameheaders_[frame];
  
  // Make sure the buffer is big enough
  const uint64_t newsize = frameheader->size_ + 1024; // We may need a bit of extra space for the H264 or JPEG headers
  if (buffersize < newsize)
  {
    buffer = utility::AlignedAllocArray<char>(PAGE_SIZE, newsize);
    buffersize = newsize;
  }
  
  const uint8_t* signature = nullptr;
  if (frameheader->signature_.size())
  {
    signature = reinterpret_cast<const uint8_t*>(frameheader->signature_.data());

  }

  // Decode and send the frame
  std::lock_guard<std::mutex> lock(mutex_);
  ImageBuffer imagebuffer;
  bool newframe = false;
  if (frameheader->Type() == file::FrameHeaderType::H265)
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
      if (BuildH265Frame(h265frameheader->donlfield_, h265frameheader->offsets_.data(), h265frameheader->offsets_.size(), buffer.get(), frameheader->size_, buf))
      {

        return std::make_pair(2, false);
      }

      if (buf.empty())
      {

        return std::make_pair(3, false);
      }

      imagebuffer = (*h265decoder)->Decode(playrequestindex, frameheader->marker_, frameheader->time_, frame, signature, frameheader->signature_.size(), buffer.get(), frameheader->size_, reinterpret_cast<const uint8_t*>(buf.data()), static_cast<int>(buf.size()), &freeimagequeue_);
      if (imagebuffer.buffer_)
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
  else if (frameheader->Type() == file::FrameHeaderType::H264)
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
      if (BuildH264Frame(h264frameheader->offsets_.data(), h264frameheader->offsets_.size(), buffer.get(), frameheader->size_, buf))
      {

        return std::make_pair(2, false);
      }

      if (buf.empty())
      {

        return std::make_pair(3, false);
      }

      imagebuffer = (*h264decoder)->Decode(playrequestindex, frameheader->marker_, frameheader->time_, frame, signature, frameheader->signature_.size(), buffer.get(), frameheader->size_, reinterpret_cast<const uint8_t*>(buf.data()), static_cast<int>(buf.size()), &freeimagequeue_);
      if (imagebuffer.buffer_)
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
  else if (frameheader->Type() == file::FrameHeaderType::MPEG4)
  {
    std::vector< std::unique_ptr<MPEG4Decoder> >::iterator mpeg4decoder = std::find_if(mpeg4decoders_.begin(), mpeg4decoders_.end(), [&frameheader](const std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == frameheader->codecindex_); });
    if (mpeg4decoder != mpeg4decoders_.end())
    {
      // Read the frame
      if (media_->Read(frameheader->offset_, frameheader->size_, buffer.get()))
      {

        return std::make_pair(1, false);
      }

      imagebuffer = (*mpeg4decoder)->Decode(playrequestindex, frameheader->time_, frame, signature, frameheader->signature_.size(), buffer.get(), frameheader->size_, reinterpret_cast<const uint8_t*>(buffer.get()), static_cast<int>(frameheader->size_), &freeimagequeue_);
      if (imagebuffer.buffer_)
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
  else if (frameheader->Type() == file::FrameHeaderType::JPEG)
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

      imagebuffer = mjpegdecoder_->Decode(playrequestindex, frameheader->time_, frame, signature, frameheader->signature_.size(), buffer.get() + tmp.size(), frameheader->size_, reinterpret_cast<const uint8_t*>(buffer.get()), static_cast<unsigned int>(frameheader->size_ + tmp.size()), &freeimagequeue_);
      if (imagebuffer.buffer_)
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
  else if (frameheader->Type() == file::FrameHeaderType::METADATA)
  {
    // Ignore

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

}
