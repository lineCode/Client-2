// videoview.cpp
//

///// Includes /////

#include "monocleclient/videoview.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/bind.hpp>
#include <monocleprotocol/objects_generated.h>
#include <monocleprotocol/metadataframetype_generated.h>
#include <monocleprotocol/objectdetectorframetype_generated.h>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/eventclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <onvifclient/ptzclient.hpp>
#include <network/uri.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QResource>
#include <QStandardPaths>
#include <QUrl>
#include <utility/utility.hpp>

#include "monocleclient/device.h"
#include "monocleclient/h265decoder.h"
#include "monocleclient/h264decoder.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/mjpegdecoder.h"
#include "monocleclient/mpeg4decoder.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/videowidget.h"
#include "monocleclient/videoviewpropertieswindow.h"
#include "monocleclient/view.h"

///// Namespaces /////

namespace client
{

///// Methods /////

VideoView::VideoView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool showinfo, const bool showobjects, boost::shared_ptr<client::Device> device, QSharedPointer<client::Recording> recording, QSharedPointer<client::RecordingTrack> track, const QResource* arial) :
  View(videowidget, cudacontext, selectedcolour, x, y, width, height, rotation, mirror, stretch, showinfo, showobjects, arial, true, true, true, true, true, true, true),
  mutex_(boost::make_shared<std::recursive_mutex>()),
  device_(device),
  recording_(recording),
  track_(track),
  actionreconnect_(new QAction(tr("Reconnect"), this)),
  actionproperties_(new QAction(tr("Properties"), this)),
  actionchartview_(new QAction(tr("Chart View"), this)),
  connection_(boost::make_shared<Connection>(MainWindow::Instance()->GetIOServicePool().GetIoService(), device_->GetProxyParams(), device_->GetAddress(), device_->GetPort())),
  metadataconnection_(boost::make_shared<Connection>(MainWindow::Instance()->GetGUIIOService(), device_->GetProxyParams(), device_->GetAddress(), device_->GetPort())),
  metadataplayrequestindex_(0),
  adaptivestreaming_(recording->GetAdaptiveStreaming())
{
  connect(actionreconnect_, &QAction::triggered, this, static_cast<void (VideoView::*)(bool)>(&VideoView::Reconnect));
  connect(actionproperties_, &QAction::triggered, this, static_cast<void (VideoView::*)(bool)>(&VideoView::Properties));
  connect(actionchartview_, &QAction::triggered, this, static_cast<void (VideoView::*)(bool)>(&VideoView::ChartView));
  connect(device_.get(), &Device::SignalStateChanged, this, &VideoView::DeviceStateChanged);
  connect(recording_.get(), &Recording::TrackAdded, this, &VideoView::TrackAdded);
  connect(recording_.get(), &Recording::TrackRemoved, this, &VideoView::TrackRemoved);
  connect(recording_.get(), &Recording::ActiveJobChanged, this, &VideoView::ActiveJobChanged);

  rotation_ = GetActiveRotation();
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);

  updatetimer_ = startTimer(std::chrono::milliseconds(150));
  metadatakeepalivetimer_ = startTimer(std::chrono::seconds(15));

  Connect();
}

VideoView::~VideoView()
{
  Disconnect();

}

void VideoView::Connect()
{
  Disconnect();

  //TODO when dragging and dropping the RECORDING, or double clicking hte RECORDING, this will be nullptr
    //TODO we can then try to determine the best track to stream
  if (track_ == nullptr)
  {


    //TODO SetMessage(GetNextPlayRequestIndex(true), false, "Invalid track");
    return;
  }

  if (!utility::Contains(recording_->GetTracks(), track_)) // This shouldn't happen but just in case...
  {
    SetMessage(GetNextPlayRequestIndex(true), false, "Invalid track");
    return;
  }

  const std::vector< QSharedPointer<client::Receiver> > receivers = recording_->GetActiveReceivers(track_);
  if (receivers.size() > 1)
  {
    LOG_GUI_WARNING_SOURCE(device_, "Multiple active receivers");

  }

  if (receivers.size() >= 1)
  {
    const QUrl url(receivers[0]->GetMediaUri());
    if ((url.scheme() == "http") && (url.path() == "/onvif/device_service")) // If it looks like an ONVIF url, lets try connecting through the servers http proxy
    {
      ConnectONVIF(receivers[0]);

    }
  }

  SetPaused(false);
  SetMessage(GetNextPlayRequestIndex(true), false, "Connecting");
  const std::string username = device_->GetUsername().toStdString();
  const std::string password = device_->GetPassword().toStdString();
  connect_ = connection_->Connect([this, username, password](const boost::system::error_code& err)
  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    if (err)
    {
      SetMessage(GetNextPlayRequestIndex(true), true, "Unable to connect");
      return;
    }

    SetMessage(GetPlayRequestIndex(), false, "Authenticating");
    getauthenticatenonce_ = connection_->GetAuthenticationNonce([this, username, password](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
    {
      std::lock_guard<std::recursive_mutex> lock(*mutex_);
      if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        SetMessage(GetNextPlayRequestIndex(true), true, "Unable to authenticate");
        return;
      }

      const std::string clientnonce = utility::GenerateRandomString(32);
      authenticate_ = connection_->Authenticate(username, clientnonce, monocle::AuthenticateDigest(username, password, getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
      {
        std::lock_guard<std::recursive_mutex> lock(*mutex_);
        if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          SetMessage(GetNextPlayRequestIndex(true), true, "Unable to authenticate");
          return;
        }

        SetMessage(GetPlayRequestIndex(), false, "Creating Streams");
        streamtokens_.clear();
        DestroyDecoders();
        for (const QSharedPointer<RecordingTrack>& track : recording_->GetTracks())
        {
          createstream_.push_back(connection_->CreateStream(recording_->GetToken(), track->GetId(), [this, track](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
          {
            std::lock_guard<std::recursive_mutex> lock(*mutex_);
            if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
            {
              SetMessage(GetNextPlayRequestIndex(true), true, "Unable to create stream: " + QString::fromStdString(createstreamresponse.GetErrorText()));
              return;
            }

            QMetaObject::invokeMethod(this, [this, trackid = track->GetId(), codecindices = createstreamresponse.codecindices_]()
            {
              std::lock_guard<std::recursive_mutex> lock(*mutex_);
              for (const monocle::CODECINDEX& codecindex : codecindices)
              {
                AddCodecIndex(trackid, codecindex);

              }
            }, Qt::QueuedConnection);

            streamtokens_.push_back(std::make_pair(track->GetId(), createstreamresponse.token_));
            if (track_ == track)
            {
              SetMessage(GetPlayRequestIndex(), false, "Requesting Live");
              activestreamtoken_ = createstreamresponse.token_;
              if (adaptivestreaming_)
              {
                activeadaptivestreamtoken_ = createstreamresponse.token_;

              }
              controlstream_ = connection_->ControlStreamLive(createstreamresponse.token_, GetPlayRequestIndex(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::CONTROLSTREAMRESPONSE& controlstreamresponse)
              {
                std::lock_guard<std::recursive_mutex> lock(*mutex_);
                if (controlstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
                {
                  SetMessage(GetNextPlayRequestIndex(true), true, "Unable to control stream");
                  return;
                }
                Keepalive();
              });
            }
          }, VideoView::ControlStreamEnd, VideoView::H265Callback, VideoView::H264Callback, nullptr, VideoView::JPEGCallback, VideoView::MPEG4Callback, VideoView::ObjectDetectorCallback, VideoView::NewCodecIndexCallback, this));
        }
      });
    });
  });

  metadatastreamtokens_.clear();
  metadataconnect_ = metadataconnection_->Connect([this, username, password](const boost::system::error_code& err)
  {
    if (err)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Failed to connect metadata stream"));
      return;
    }

    metadatagetauthenticatenonce_ = metadataconnection_->GetAuthenticationNonce([this, username, password](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
    {
      if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Failed to retrieve metadata stream authentication nonce"));
        return;
      }

      const std::string clientnonce = utility::GenerateRandomString(32);
      metadataauthenticate_ = metadataconnection_->Authenticate(username, clientnonce, monocle::AuthenticateDigest(username, password, getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
      {
        if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Failed to authenticate metadata stream"));
          return;
        }

        for (const QSharedPointer<RecordingTrack>& metadatatrack : recording_->GetMetadataTracks())
        {
          AddMetadataTrack(metadatatrack);

        }

        for (const QSharedPointer<RecordingTrack>& objectdetectortrack : recording_->GetObjectDetectorTracks())
        {
          AddMetadataTrack(objectdetectortrack);

        }
      });
    });
  });
}

void VideoView::Disconnect()
{
  connect_.Close();
  getauthenticatenonce_.Close();
  authenticate_.Close();
  std::for_each(createstream_.begin(), createstream_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  createstream_.clear();
  controlstream_.Close();
  keepalive_.Close();

  metadataconnect_.Close();
  metadatagetauthenticatenonce_.Close();
  metadataauthenticate_.Close();
  std::for_each(metadatacreatestreams_.begin(), metadatacreatestreams_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  metadatacreatestreams_.clear();
  std::for_each(metadatacontrolstreams_.begin(), metadatacontrolstreams_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  metadatacontrolstreams_.clear();
  std::for_each(metadatakeepalives_.begin(), metadatakeepalives_.end(), [](monocle::client::Connection& connection) { connection.Close(); });
  metadatakeepalives_.clear();

  connection_->Destroy();
  metadataconnection_->Destroy();

  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    freeimagequeue_.Destroy();
    DestroyDecoders();
  }

  if (onvifptz_)
  {
    onvifptz_->Destroy();
    onvifptz_.reset();
  }

  if (onvifmedia_)
  {
    onvifmedia_->Destroy();
    onvifmedia_.reset();
  }

  if (onvifevent_)
  {
    onvifevent_->Destroy();
    onvifevent_.reset();
  }

  if (onvifdevice_)
  {
    onvifdevice_->Destroy();
    onvifdevice_.reset();
  }

  ptzxmin_.reset();
  ptzxmax_.reset();
  ptzymin_.reset();
  ptzymax_.reset();
}

void VideoView::GetMenu(QMenu& parent)
{
  parent.addAction(actionreconnect_);
  parent.addAction(actionproperties_);

  // Tracks
  QMenu* tracks = new QMenu("Tracks", &parent);
  for (const QSharedPointer<client::RecordingTrack>& track : recording_->GetVideoTracks())
  {
    const QString description = track->GetDescription().isEmpty() ? track->GetToken() : QString("(" + track->GetDescription() + ")");
    QAction* action = tracks->addAction(description, [this, recordingtoken = recording_->GetToken(), trackid = track->GetId()]()
    {
      std::vector< std::pair<uint32_t, uint64_t> >::const_iterator streamtoken = std::find_if(streamtokens_.cbegin(), streamtokens_.cend(), [trackid](const std::pair<uint32_t, uint64_t>& streamtoken) { return (streamtoken.first == trackid); });
      if (streamtoken == streamtokens_.cend())
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Unable to retrieve stream token: ") + QString::number(trackid));
        return;
      }

      const QSharedPointer<RecordingTrack> track = recording_->GetTrack(trackid);
      if (!track)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Unable to retrieve track: ") + QString::number(trackid));
        return;
      }

      // Get the right stream
      //TODO this is insanity really...
      boost::optional<uint64_t> prevstream;//TODO needs more filling out...
      if (adaptivestreaming_)
      {
        if (activeadaptivestreamtoken_.is_initialized())//TODO I think we only this one condition and the rest is all the same result
        {
          prevstream = *activeadaptivestreamtoken_;
          activeadaptivestreamtoken_.reset();
        }
        else
        {
          if (activestreamtoken_.is_initialized())
          {
            if (*activestreamtoken_ == streamtoken->second)
            {

              return; // Don't need to do anything
            }
            else
            {
              activestreamtoken_ = streamtoken->second;

            }
          }
          else
          {
            activestreamtoken_ = streamtoken->second;

          }
        }
      }
      else
      {
        if (activestreamtoken_.is_initialized())
        {
          if (*activestreamtoken_ == streamtoken->second)
          {

            return; // Don't need to do anything
          }
          else
          {
            activestreamtoken_ = streamtoken->second;

          }
        }
        else
        {
          activestreamtoken_ = streamtoken->second;

        }
      }
      adaptivestreaming_ = false;

      if (prevstream.is_initialized())
      {
        connection_->ControlStreamPause(*prevstream, boost::none); // Pause the previous stream and start the new livestream

      }
      track_ = track;
      connection_->ControlStreamLive(*activestreamtoken_, GetNextPlayRequestIndex(true));
    });
    action->setCheckable(true);
    action->setChecked(track == track_);//TODO maybe not correct?
  }
  parent.addMenu(tracks);

  if (device_->SupportsGetTrackStatistics())
  {
    if (recording_->GetNumObjectDetectors())
    {
      parent.addAction(actionchartview_);

    }
  }
  else
  {
    parent.addAction(actionchartview_);

  }

  View::GetMenu(parent);
}

double VideoView::GetAspectRatio() const
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

bool VideoView::GetImage(ImageBuffer& imagebuffer)
{
  bool hasimage = false;
  ImageBuffer previmagebuffer;
  while (true)
  {
    if (imagequeue_.pop(imagebuffer))
    {
      bandwidthsizes_.push_back(std::make_pair(std::chrono::steady_clock::now(), imagebuffer.originalsize_));

      // If we have skipped frames, we should place them back into the temporary list, or destroy them if there is no room
      if (previmagebuffer.buffer_ || previmagebuffer.cudacontext_)
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

      return hasimage;
    }
  }
}

int64_t VideoView::GetTimeOffset() const
{
  return device_->GetTimeOffset();
}

void VideoView::FrameStep(const bool forwards)
{
  if (!activestreamtoken_.is_initialized())
  {

    return;
  }

  if (adaptivestreaming_ && activeadaptivestreamtoken_.is_initialized())
  {
    //TODO pause this stream and pause the activestreamtoken_ at the current time_

    return;
  }

  paused_ = true;
  ImageBuffer imagebuffer = cache_.GetImage(forwards, GetPlayRequestIndex(), sequencenum_);
  if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
  {
    {
      std::lock_guard<std::recursive_mutex> lock(*mutex_);
      ResetDecoders();
    }

    controlstreamendcallback_ = [this, forwards](const uint64_t playrequestindex, const monocle::ErrorCode error)
    {
      if (GetPlayRequestIndex() != playrequestindex)
      {

        return;
      }

      if (error != monocle::ErrorCode::Success)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Play failed");
        return;
      }

      imagequeue_.consume_all([this](const ImageBuffer& imagebuffer){ cache_.AddImage(imagebuffer); });

      // Find the previous/next frame to go to
      ImageBuffer imagebuffer = cache_.GetImage(forwards, playrequestindex, sequencenum_);
      if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Unable to find frame");
        return;
      }
      WriteFrame(imagebuffer);
    };

    activeadaptivestreamtoken_.reset();
    connection_->ControlStreamFrameStep(*activestreamtoken_, GetNextPlayRequestIndex(true), forwards, sequencenum_);//TODO this is difficult, because we may have been live, but now we have a sequencenum and everything else is fucked on this original stream?
  }
  else
  {
    WriteFrame(imagebuffer);
    
  }

  for (const std::pair<QSharedPointer<RecordingTrack>, uint64_t>& metadatastreamtoken : metadatastreamtokens_)
  {
    metadataconnection_->ControlStream(metadatastreamtoken.second, GetNextMetadataPlayRequestIndex(), true, false, forwards, time_, boost::none, 4, false); // Request some additional frames just in case

  }
}

void VideoView::Play(const uint64_t time, const boost::optional<uint64_t>& numframes)
{
  if (!activestreamtoken_.is_initialized())
  {

    return;
  }

  if (adaptivestreaming_ && activeadaptivestreamtoken_.is_initialized())
  {
    //TODO pause activeadaptivestreamtoken_ stream
    activeadaptivestreamtoken_.reset();
  }

  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    ResetDecoders();
  }

  connection_->ControlStream(*activestreamtoken_, GetNextPlayRequestIndex(true), true, !numframes.is_initialized(), true, time + GetTimeOffset(), boost::none, numframes, false);//TODO needs to control the correct stream. I think this is correct just now...
  if (numframes.is_initialized() && ((*numframes == 0) || (*numframes == 1))) // Is this an effectively pause request...
  {
    controlstreamendcallback_ = [this](const uint64_t playrequestindex, const monocle::ErrorCode err)
    {
      if (GetPlayRequestIndex() != playrequestindex)
      {

        return;
      }

      if (err != monocle::ErrorCode::Success)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Play failed");
        return;
      }

      imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.AddImage(imagebuffer); });
      ImageBuffer imagebuffer = cache_.GetLatestImageBySequence(playrequestindex);
      if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, "ControlStream Unable to find frame");
        return;
      }
      WriteFrame(imagebuffer);
    };
    paused_ = true;
  }
  else
  {
    paused_ = false;

  }

  for (const std::pair<QSharedPointer<RecordingTrack>, uint64_t>& metadatastreamtoken : metadatastreamtokens_)
  {
    metadataconnection_->ControlStream(metadatastreamtoken.second, GetNextMetadataPlayRequestIndex(), true, !numframes.is_initialized(), true, time + GetTimeOffset(), boost::none, numframes, false);

  }
}

void VideoView::Pause(const boost::optional<uint64_t>& time)
{
  if (!activestreamtoken_.is_initialized())
  {

    return;
  }

  if (adaptivestreaming_ && activeadaptivestreamtoken_.is_initialized())
  {
    //TODO pause activeadaptivestreamtoken_ stream
    activeadaptivestreamtoken_.reset();
  }

  SetPaused(true);
  connection_->ControlStreamPause(*activestreamtoken_, time);//TODO this should pause the adaptivestream if it is available first now...
  for (const std::pair<QSharedPointer<RecordingTrack>, uint64_t>& metadatastreamtoken : metadatastreamtokens_)
  {
    metadataconnection_->ControlStreamPause(metadatastreamtoken.second, time);

  }
}

void VideoView::Stop()
{
  if (!activestreamtoken_.is_initialized())
  {

    return;
  }

  if (adaptivestreaming_ && activeadaptivestreamtoken_.is_initialized())
  {
    //TODO pause activeadaptivestreamtoken_ stream
    activeadaptivestreamtoken_.reset();
  }

  uint64_t stream = 0;
  if (adaptivestreaming_)
  {
    int i = 0;//TODO pick the most appropriate stream, create a nice method from the other place where we decide on this

  }
  else
  {
    stream = *activestreamtoken_;

  }

  SetPaused(false);
  connection_->ControlStreamLive(stream, GetNextPlayRequestIndex(true));
  for (const std::pair<QSharedPointer<RecordingTrack>, uint64_t>& metadatastreamtoken : metadatastreamtokens_)
  {
    metadataconnection_->ControlStreamLive(metadatastreamtoken.second, GetNextMetadataPlayRequestIndex());

  }
}

void VideoView::Scrub(const uint64_t time)
{
  if (!activestreamtoken_.is_initialized())
  {

    return;
  }

  if (adaptivestreaming_ && activeadaptivestreamtoken_.is_initialized())
  {
    //TODO pause activeadaptivestreamtoken_ stream
    activeadaptivestreamtoken_.reset();
  }

  {
    std::lock_guard<std::recursive_mutex> lock(*mutex_);
    ResetDecoders();
  }

  connection_->ControlStream(*activestreamtoken_, GetNextPlayRequestIndex(false), true, false, true, time + GetTimeOffset(), boost::none, 1, true);
  controlstreamendcallback_ = [this](const uint64_t playrequestindex, const monocle::ErrorCode err)
  {
    if (err != monocle::ErrorCode::Success)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "Scrub failed");
      return;
    }

    imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { cache_.AddImage(imagebuffer); });

    ImageBuffer imagebuffer = cache_.GetLatestImage(playrequestindex);
    if (imagebuffer.type_ == IMAGEBUFFERTYPE_INVALID)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "Scrub failed to find frames");
      return;
    }
    WriteFrame(imagebuffer);
    cache_.Clear();
  };

  for (const std::pair<QSharedPointer<RecordingTrack>, uint64_t>& metadatastreamtoken : metadatastreamtokens_)
  {
    metadataconnection_->ControlStream(metadatastreamtoken.second, GetNextMetadataPlayRequestIndex(), true, false, false, time + GetTimeOffset(), boost::none, 30, false);

  }
  paused_ = true;
}

void VideoView::SetPosition(VideoWidget* videowidget, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent)
{
  View::SetPosition(videowidget, x, y, width, height, rotation, mirror, stretch, makecurrent);

  if (streamtokens_.empty())
  {

    return;
  }

  if (!adaptivestreaming_)
  {

    return;
  }
  
  if (!activeadaptivestreamtoken_.is_initialized()) // We can only do adaptive streaming when live
  {

    return;
  }

  const QSharedPointer<RecordingJob> activejob = recording_->GetActiveJob();
  if (!activejob)
  {

    return;
  }

  // Sort the recording tracks by id
  std::vector< std::pair<uint32_t, uint64_t> > trackarea; // <trackid, area>
  trackarea.reserve(5);
  for (const QSharedPointer<RecordingJobSource>& source : activejob->GetSources())
  {
    for (const QSharedPointer<RecordingJobSourceTrack>& sourcetrack : source->GetTracks())
    {
      const QSharedPointer<RecordingTrack> track = sourcetrack->GetTrack();
      if (!track)
      {

        continue;
      }

      if (sourcetrack->GetState() != monocle::RecordingJobState::Active)
      {

        continue;
      }

      try
      {
        const boost::optional<QString> activewidth = sourcetrack->GetActiveWidth();
        const boost::optional<QString> activeheight = sourcetrack->GetActiveHeight();
        if (!activewidth.is_initialized() || !activeheight.is_initialized())
        {

          continue;
        }

        const uint64_t width = boost::lexical_cast<uint64_t>(activewidth->toStdString());
        const uint64_t height = boost::lexical_cast<uint64_t>(activeheight->toStdString());
        trackarea.push_back(std::make_pair(track->GetId(), width * height));
      }
      catch (...)
      {

        continue;
      }
    }
  }

  if (trackarea.empty())
  {

    return;
  }
  
  // Find the best track for our current resolution
  std::sort(trackarea.begin(), trackarea.end(), [](const std::pair<uint32_t, uint64_t>& lhs, const std::pair<uint32_t, uint64_t>& rhs) { return (lhs.second < rhs.second); });
  const QRect pixelrect = GetPixelRect();
  const uint64_t currentarea = pixelrect.width() * pixelrect.height();
  std::vector< std::pair<uint32_t, uint64_t> >::iterator i = std::find_if(trackarea.begin(), trackarea.end(), [currentarea](const std::pair<uint32_t, uint64_t>& trackarea) { return (currentarea < trackarea.second); });
  if (i == trackarea.end())
  {
    i = trackarea.end() - 1;

  }
  
  std::vector< std::pair<uint32_t, uint64_t> >::const_iterator j = std::find_if(streamtokens_.cbegin(), streamtokens_.cend(), [i](const std::pair<uint32_t, uint64_t>& streamtoken) { return (streamtoken.first == i->first); });
  if (j == streamtokens_.cend())
  {

    return; // Shouldn't be possible but ok...
  }

  if (j->second == *activeadaptivestreamtoken_) // Don't bother changing anything if we are already streaming this track
  {

    return;
  }

  connection_->ControlStreamPause(*activeadaptivestreamtoken_, boost::none); // Pause the previous stream and start the new livestream
  activeadaptivestreamtoken_ = j->second;
  connection_->ControlStreamLive(*activeadaptivestreamtoken_, GetNextPlayRequestIndex(true));
}

bool VideoView::HasHardwareDecoder() const
{
  return (cudacontext_ && h264decoders_.size());
}

std::vector<QString> VideoView::GetProfileTokens() const
{
  return recording_->GetActiveProfileTokens(track_);
}

void VideoView::timerEvent(QTimerEvent* event)
{
  if (updatetimer_)
  {
    if (onvifdevice_ && onvifdevice_->IsInitialised())
    {
      onvifdevice_->Update();

    }

    if (onvifevent_ && onvifevent_->IsInitialised())
    {
      onvifevent_->Update();

    }

    if (onvifmedia_ && onvifmedia_->IsInitialised())
    {
      onvifmedia_->Update();

    }

    if (onvifptz_ && onvifptz_->IsInitialised())
    {
      onvifptz_->Update();

    }
  }
  else if (metadatakeepalivetimer_)
  {
    if (metadataconnection_)
    {
      metadataconnection_->Keepalive();

    }
  }
  
  View::timerEvent(event);
}

void VideoView::ControlStreamEnd(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata)
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  if (videoview->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  QMetaObject::invokeMethod(videoview, [videoview, playrequestindex, error]()
  {
    if (videoview->controlstreamendcallback_)
    {
      videoview->controlstreamendcallback_(playrequestindex, error);

    }
  }, Qt::QueuedConnection);
}

void VideoView::H265Callback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata) // This gets called by the Connection thread
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  std::lock_guard<std::recursive_mutex> lock(*videoview->mutex_);
  if (videoview->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<H265Decoder> >::iterator h265decoder = std::find_if(videoview->h265decoders_.begin(), videoview->h265decoders_.end(), [trackid, codecindex](std::unique_ptr<H265Decoder>& h265decoder) { return ((h265decoder->GetTrackId() == trackid) && (h265decoder->GetId() == codecindex)); });
  if (h265decoder == videoview->h265decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*h265decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &videoview->freeimagequeue_);
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (videoview->imagequeue_.write_available())
    {
      videoview->imagequeue_.push(imagebuffer);

    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void VideoView::H264Callback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata) // This gets called by the Connection thread
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  std::lock_guard<std::recursive_mutex> lock(*videoview->mutex_);
  if (videoview->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<H264Decoder> >::iterator h264decoder = std::find_if(videoview->h264decoders_.begin(), videoview->h264decoders_.end(), [trackid, codecindex](std::unique_ptr<H264Decoder>& h264decoder) { return ((h264decoder->GetTrackId() == trackid) && (h264decoder->GetId() == codecindex)); });
  if (h264decoder == videoview->h264decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*h264decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &videoview->freeimagequeue_);
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (videoview->imagequeue_.write_available())
    {
      videoview->imagequeue_.push(imagebuffer);

    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void VideoView::MetadataCallback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  if (videoview->metadataplayrequestindex_ != playrequestindex)
  {

    return;
  }
}

void VideoView::JPEGCallback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata) // This gets called by the Connection thread
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  std::lock_guard<std::recursive_mutex> lock(*videoview->mutex_);
  if (videoview->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  if (!videoview->mjpegdecoder_)
  {
    videoview->mjpegdecoder_ = std::make_unique<MJpegDecoder>(trackid, 0, videoview->device_->GetPublicKey()); // Id doesn't matter because we always ignore it from this point onwards
    const client::DECODERERROR error = videoview->mjpegdecoder_->Init();
    if (error)
    {
      videoview->mjpegdecoder_.reset();
      LOG_GUI_THREAD_WARNING_SOURCE(videoview->device_, "MJPEGDecoder failed to initialise");
      return;
    }
  }

  ImageBuffer imagebuffer = videoview->mjpegdecoder_->Decode(playrequestindex, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &videoview->freeimagequeue_);
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (videoview->imagequeue_.write_available())
    {
      videoview->imagequeue_.push(imagebuffer);

    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void VideoView::MPEG4Callback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata) // This gets called by the Connection thread
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  std::lock_guard<std::recursive_mutex> lock(*videoview->mutex_);
  if (videoview->GetPlayRequestIndex() != playrequestindex)
  {

    return;
  }

  std::vector< std::unique_ptr<MPEG4Decoder> >::iterator mpeg4decoder = std::find_if(videoview->mpeg4decoders_.begin(), videoview->mpeg4decoders_.end(), [trackid, codecindex](std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return ((mpeg4decoder->GetTrackId() == trackid) && (mpeg4decoder->GetId() == codecindex)); });
  if (mpeg4decoder == videoview->mpeg4decoders_.end())
  {

    return;
  }

  ImageBuffer imagebuffer = (*mpeg4decoder)->Decode(playrequestindex, timestamp, sequencenum, signature, signaturesize, signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &videoview->freeimagequeue_);
  if (imagebuffer.buffer_ || imagebuffer.cudacontext_)
  {
    if (videoview->imagequeue_.write_available())
    {
      videoview->imagequeue_.push(imagebuffer);

    }
    else
    {
      imagebuffer.Destroy();

    }
  }
}

void VideoView::ObjectDetectorCallback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::ObjectDetectorFrameType objectdetectorframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  if (videoview->metadataplayrequestindex_ != playrequestindex)
  {

    return;
  }

  if (objectdetectorframetype == monocle::ObjectDetectorFrameType::OBJECT_DETECTION)
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
    videoview->videowidget_->makeCurrent();
    videoview->UpdateObjects(objects, timestamp);
    videoview->videowidget_->doneCurrent();
  }
}

void VideoView::NewCodecIndexCallback(const uint32_t trackid, const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata)
{
  VideoView* videoview = reinterpret_cast<VideoView*>(callbackdata);
  std::lock_guard<std::recursive_mutex> lock(*videoview->mutex_);
  videoview->AddCodecIndex(trackid, monocle::CODECINDEX(id, codec, parameters, timestamp));
}

void VideoView::ConnectONVIF(const QSharedPointer<client::Receiver>& receiver)
{
  const std::vector<QString> profiletokens = recording_->GetActiveProfileTokens(track_);
  if (profiletokens.empty())
  {
    LOG_GUI_WARNING_SOURCE(device_, "No active profile tokens available to connect via ONVIF");
    return;
  }

  const std::string profile = profiletokens[0].toStdString();

  onvifdevice_ = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
  onvifevent_ = boost::make_shared<onvif::event::EventClient>(boost::make_shared<std::recursive_mutex>());
  onvifmedia_ = boost::make_shared<onvif::media::MediaClient>(boost::make_shared<std::recursive_mutex>());
  onvifptz_ = boost::make_shared<onvif::ptz::PTZClient>(boost::make_shared<std::recursive_mutex>());
  const std::string receiverusername = receiver->GetUsername().toStdString();
  const std::string receiverpassword = receiver->GetPassword().toStdString();

  if (onvifdevice_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), receiver->GetMediaUri().toStdString(), receiverusername, receiverpassword, 1, false, true))
  {
    LOG_GUI_WARNING_SOURCE(device_, "ONVIF device service initialisation failed");
    return;
  }

  onvifconnection_ = onvifdevice_->GetSystemDateAndTimeCallback([this, profile, receiverusername, receiverpassword](const onvif::device::GetSystemDateAndTimeResponse& getsystemdateandtimeresponse)
  {
    if (!getsystemdateandtimeresponse.Error())
    {
      if (!getsystemdateandtimeresponse.systemdatetime_.is_initialized())
      {
        LOG_GUI_WARNING_SOURCE(device_, "Failed to retrieve system date and time");

      }
      else
      {
        onvifdevice_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
        onvifevent_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
        onvifmedia_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
        onvifptz_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      }
    }

    onvifconnection_ = onvifdevice_->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY_ALL, [this, profile, receiverusername, receiverpassword](const onvif::device::GetCapabilitiesResponse& getcapabilitiesresponse)
    {
      if (getcapabilitiesresponse.Error())
      {
        LOG_GUI_WARNING_SOURCE(device_, "Failed to get capabilities");
        return;
      }

      if (!getcapabilitiesresponse.capabilities_.is_initialized())
      {
        LOG_GUI_WARNING_SOURCE(device_, "Invalid capabilities");
        return;
      }

      if (getcapabilitiesresponse.capabilities_->events_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->events_->xaddr_.is_initialized())
        {
          LOG_GUI_WARNING_SOURCE(device_, "Invalid event xaddr");
          return;
        }

        if (onvifevent_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), *getcapabilitiesresponse.capabilities_->events_->xaddr_, receiverusername, receiverpassword, 1, false, true))
        {
          LOG_GUI_WARNING_SOURCE(device_, "ONVIF event service initialisation failed");

        }
      }

      if (getcapabilitiesresponse.capabilities_->media_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->media_->xaddr_.is_initialized())
        {
          LOG_GUI_WARNING_SOURCE(device_, "Invalid media xaddr");
          return;
        }

        if (onvifmedia_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), *getcapabilitiesresponse.capabilities_->media_->xaddr_, receiverusername, receiverpassword, 1, false, true))
        {
          LOG_GUI_WARNING_SOURCE(device_, "ONVIF media service initialisation failed");

        }
      }

      if (getcapabilitiesresponse.capabilities_->ptz_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->ptz_->xaddr_.is_initialized())
        {
          LOG_GUI_WARNING_SOURCE(device_, "Invalid PTZ xaddr");
          return;
        }

        if (onvifptz_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), *getcapabilitiesresponse.capabilities_->ptz_->xaddr_, receiverusername, receiverpassword, 1, false, true))
        {
          LOG_GUI_WARNING_SOURCE(device_, "ONVIF PTZ service initialisation failed");

        }
      }

      if (onvifmedia_->IsInitialised())
      {
        onvifconnection_ = onvifmedia_->GetProfileCallback(profile, [this, profile](const onvif::media::GetProfileResponse& getprofileresponse)
        {
          if (getprofileresponse.Error())
          {
            LOG_GUI_WARNING_SOURCE(device_, QString("Failed to get profile: ") + QString::fromStdString(profile));
            return;
          }

          if (!getprofileresponse.profile_.is_initialized())
          {
            LOG_GUI_WARNING_SOURCE(device_, "Invalid profile");
            return;
          }

          if (onvifptz_->IsInitialised() && getprofileresponse.profile_->ptzconfiguration_.is_initialized() && getprofileresponse.profile_->ptzconfiguration_->defaultcontinuouspantiltvelocityspace_.is_initialized() && getprofileresponse.profile_->ptzconfiguration_->token_.is_initialized())
          {
            const std::string continuousvelocityspaceuri = *getprofileresponse.profile_->ptzconfiguration_->defaultcontinuouspantiltvelocityspace_;

            onvifconnection_ = onvifptz_->GetConfigurationOptionsCallback(*getprofileresponse.profile_->ptzconfiguration_->token_, [this, continuousvelocityspaceuri](const onvif::ptz::GetConfigurationOptionsResponse& getconfigurationoptionsresponse)
            {
              if (getconfigurationoptionsresponse.Error())
              {
                LOG_GUI_WARNING_SOURCE(device_, "Failed to retrieve PTZ configuration options");
                return;
              }

              if (getconfigurationoptionsresponse.ptzconfigurationoptions_.is_initialized() && getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_.is_initialized())
              {
                auto pantiltcontinuousvelocityspace = std::find_if(getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_->continuouspantiltvelocityspace_.cbegin(), getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_->continuouspantiltvelocityspace_.cend(), [continuousvelocityspaceuri](const onvif::Space2dDescription& continuouspantiltvelocityspace)
                {
                  return (continuouspantiltvelocityspace.uri_.is_initialized() &&
                          continuouspantiltvelocityspace.xrange_.is_initialized() &&
                          continuouspantiltvelocityspace.xrange_->min_.is_initialized() &&
                          continuouspantiltvelocityspace.xrange_->max_.is_initialized() &&
                          continuouspantiltvelocityspace.yrange_.is_initialized() &&
                          continuouspantiltvelocityspace.yrange_->min_.is_initialized() &&
                          continuouspantiltvelocityspace.yrange_->max_.is_initialized() &&
                          (*continuouspantiltvelocityspace.uri_ == continuousvelocityspaceuri));
                });
                if (pantiltcontinuousvelocityspace != getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_->continuouspantiltvelocityspace_.cend())
                {
                  ptzxmin_ = *pantiltcontinuousvelocityspace->xrange_->min_;
                  ptzxmax_ = *pantiltcontinuousvelocityspace->xrange_->max_;
                  ptzymin_ = *pantiltcontinuousvelocityspace->yrange_->min_;
                  ptzymax_ = *pantiltcontinuousvelocityspace->yrange_->max_;
                }

                auto zoomcontinuousvelocityspace = std::find_if(getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_->continuouszoomvelocityspace_.cbegin(), getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_->continuouszoomvelocityspace_.cend(), [continuousvelocityspaceuri](const onvif::Space1dDescription& continuouszoomvelocityspace)
                {
                  return (continuouszoomvelocityspace.uri_.is_initialized() &&
                          continuouszoomvelocityspace.xrange_.is_initialized() &&
                          continuouszoomvelocityspace.xrange_->min_.is_initialized() &&
                          continuouszoomvelocityspace.xrange_->max_.is_initialized() &&
                          (*continuouszoomvelocityspace.uri_ == continuousvelocityspaceuri));
                });
                if (zoomcontinuousvelocityspace != getconfigurationoptionsresponse.ptzconfigurationoptions_->spaces_->continuouszoomvelocityspace_.cend())
                {
                  zoommin_ = *zoomcontinuousvelocityspace->xrange_->min_;
                  zoommax_ = *zoomcontinuousvelocityspace->xrange_->max_;
                }
              }
            });
          }
        });
      }
    });
  });
}

void VideoView::AddCodecIndex(const uint32_t trackid, const monocle::CODECINDEX& codecindex)
{
  std::vector<std::string> parameterssplit;
  boost::split(parameterssplit, codecindex.parameters_, boost::is_any_of(";"), boost::algorithm::token_compress_on);

  if (codecindex.codec_ == monocle::Codec::MJPEG) // MJPEG sorts itself out
  {

    return;
  }
  else if (codecindex.codec_ == monocle::Codec::H265)
  {
    std::unique_ptr<H265Decoder> h265decoder = std::make_unique<H265Decoder>(trackid, codecindex.id_, device_->GetPublicKey());
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
    std::unique_ptr<H264Decoder> h264decoder = std::make_unique<H264Decoder>(trackid, codecindex.id_, device_->GetPublicKey(), cudacontext_);
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
    std::unique_ptr<MPEG4Decoder> mpeg4decoder = std::make_unique<MPEG4Decoder>(trackid, codecindex.id_, device_->GetPublicKey());
    const DECODERERROR error = mpeg4decoder->Init(parameterssplit);
    if (error)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, "MPEG4Decoder failed to initialise");
      return;
    }
    mpeg4decoders_.emplace_back(std::move(mpeg4decoder));
  }
}

void VideoView::DestroyDecoders()
{
  // We destroy the imagequeue preemptively here because we need the cuda context stored inside ffmpeg
  imagequeue_.consume_all([this](const ImageBuffer& imagebuffer) { const_cast<ImageBuffer&>(imagebuffer).Destroy(); });
  cache_.Clear();

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

void VideoView::Keepalive()
{
  std::shared_ptr<boost::asio::deadline_timer> timer = std::make_shared<boost::asio::deadline_timer>(MainWindow::Instance()->GetGUIIOService());
  timer->expires_from_now(boost::posix_time::seconds(15));
  QWeakPointer<VideoView> ptr = sharedFromThis().staticCast<VideoView>();
  timer->async_wait([ptr, timer](const boost::system::error_code& err)
  {
    QSharedPointer<VideoView> p = ptr.lock();
    if (p == nullptr)
    {

      return;
    }

    if (err)
    {

      return;
    }

    p->keepalive_ = p->connection_->Keepalive([ptr](const std::chrono::steady_clock::duration latency, const monocle::client::KEEPALIVERESPONSE& keepaliveresponse)
    {
      QSharedPointer<VideoView> p = ptr.lock();
      if (p == nullptr)
      {

        return;
      }

      if (keepaliveresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {

        return;
      }

      p->Keepalive();
    });
  });
}

ROTATION VideoView::GetActiveRotation() const
{
  const std::vector<ROTATION> rotations = recording_->GetActiveRotations(track_);
  if (rotations.empty())
  {

    return ROTATION::_0;
  }
  return rotations.front();
}

void VideoView::ResetDecoders()
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

uint64_t VideoView::GetNextMetadataPlayRequestIndex()
{
  videowidget_->makeCurrent();
  objects_.Clear();
  videowidget_->doneCurrent();
  return ++metadataplayrequestindex_;
}

void VideoView::AddMetadataTrack(const QSharedPointer<RecordingTrack>& metadatatrack)
{
  metadatacreatestreams_.push_back(metadataconnection_->CreateStream(recording_->GetToken(), metadatatrack->GetId(), [this, metadatatrack](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
  {
    if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Failed to create metadata stream: ") + QString::number(metadatatrack->GetId()));
      return;
    }
    metadatastreamtokens_.push_back(std::make_pair(metadatatrack, createstreamresponse.token_));

    // We kick off a live stream here because there can be no negative consequences aside from the additional bandwidth usage, and it might help a lot
    metadatacontrolstreams_.push_back(metadataconnection_->ControlStreamLive(createstreamresponse.token_, GetNextMetadataPlayRequestIndex(), [this, metadatatrack](const std::chrono::steady_clock::duration latency, const monocle::client::CONTROLSTREAMRESPONSE& controlstreamresponse)
    {
      if (controlstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Failed to control metadata live stream: ") + QString::number(metadatatrack->GetId()));
        return;
      }
    }));
  }, VideoView::ControlStreamEnd, nullptr, nullptr, VideoView::MetadataCallback, nullptr, nullptr, VideoView::ObjectDetectorCallback, nullptr, this));
}

void VideoView::DeviceStateChanged(const DEVICESTATE state, const QString&)
{
  if (state == DEVICESTATE::SUBSCRIBED)
  {
    Reconnect(false);

  }
}

void VideoView::Reconnect(bool)
{
  Connect();

}

void VideoView::Properties(bool)
{
  VideoViewPropertiesWindow(videowidget_, sharedFromThis().staticCast<VideoView>()).exec();

}

void VideoView::ChartView(bool)
{
  MainWindow::Instance()->GetVideoWidgetsMgr().CreateVideoChartView(device_, recording_, recording_->GetObjectDetectorTracks());

}

void VideoView::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  if (track->GetTrackType() == monocle::TrackType::Video)
  {
    if (track_ == nullptr) // If we weren't streaming before, we now have something to bite on, so lets go
    {
      track_ = track;
      Connect();
    }
    else
    {
      createstream_.push_back(connection_->CreateStream(recording_->GetToken(), track->GetId(), [this, track](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse)
      {
        if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Failed to create video stream: ") + QString::number(track->GetId()));
          return;
        }
        streamtokens_.push_back(std::make_pair(track->GetId(), createstreamresponse.token_));
      }, VideoView::ControlStreamEnd, VideoView::H265Callback, VideoView::H264Callback, nullptr, VideoView::JPEGCallback, VideoView::MPEG4Callback, VideoView::ObjectDetectorCallback, VideoView::NewCodecIndexCallback, this));
    }
  }
  else if ((track->GetTrackType() == monocle::TrackType::Metadata) || (track->GetTrackType() == monocle::TrackType::ObjectDetector))
  {
    AddMetadataTrack(track);

  }
}
//TODO make sure to test this...
void VideoView::TrackRemoved(const uint32_t trackid)
{
  std::vector< std::pair<uint32_t, uint64_t> >::const_iterator streamtoken = std::find_if(streamtokens_.cbegin(), streamtokens_.cend(), [trackid](const std::pair<uint32_t, uint64_t>& streamtoken) { return (streamtoken.first == trackid); });
  if (streamtoken == streamtokens_.cend())
  {
    LOG_GUI_THREAD_WARNING_SOURCE(device_, QString("Unable to retrieve stream token: ") + QString::number(trackid));
    return;
  }
  streamtokens_.erase(streamtoken);

  auto i = std::find_if(metadatastreamtokens_.begin(), metadatastreamtokens_.end(), [trackid](const std::pair<QSharedPointer<RecordingTrack>, uint64_t>& metadatatrack) { return (metadatatrack.second == trackid); });
  if (i != metadatastreamtokens_.end())
  {
    metadatastreamtokens_.erase(i);

  }

  if (activestreamtoken_.is_initialized() && (*activestreamtoken_ == trackid))
  {
    if (streamtokens_.size()) // Switch to another video track if one is available
    {
      
      //TODO pick another stream to stream from...


      track_ = recording_->GetVideoTracks().front(); //TODO make sure to check this
//TODO Stop()
    }
    else // Nothing left to do, just warn the user and turn off streaming
    {
      activestreamtoken_ .reset();
      track_.reset();
      //TODO SetMessage("Error no tracks")
    }
  }
}

void VideoView::ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob)
{
  rotation_ = GetActiveRotation();
  SetPosition(videowidget_, rect_.x(), rect_.y(), rect_.width(), rect_.height(), rotation_, mirror_, stretch_, true);
  Connect();
}

}
