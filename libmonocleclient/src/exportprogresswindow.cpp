// exportprogresswindow.cpp
//

///// Includes /////

#include "monocleclient/exportprogresswindow.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <QDateTime>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

#include "monocleclient/device.h"
#include "monocleclient/h265decoder.h"
#include "monocleclient/h264decoder.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/mjpegdecoder.h"
#include "monocleclient/mpeg4decoder.h"
#include "monocleclient/recording.h"
#include "monocleclient/savevideo.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Globals /////

static const QString EXPORT_FORMAT_MONOCLE("Monocle");
static const QString EXPORT_FORMAT_AVI("AVI");

///// Functions /////

QString ToString(const EXPORTFORMAT exportformat)
{
  switch (exportformat)
  {
    case EXPORTFORMAT_MONOCLE:
    {

      return EXPORT_FORMAT_MONOCLE;
    }
    case EXPORTFORMAT_AVI:
    {

      return EXPORT_FORMAT_AVI;
    }
    default:
    {

      return QString();
    }
  }
}

boost::optional<EXPORTFORMAT> GetExportFormat(const QString& exportformat)
{
  if (QString::compare(exportformat, EXPORT_FORMAT_MONOCLE, Qt::CaseInsensitive) == 0)
  {

    return EXPORTFORMAT_MONOCLE;
  }
  else if (QString::compare(exportformat, EXPORT_FORMAT_AVI, Qt::CaseInsensitive) == 0)
  {

    return EXPORTFORMAT_AVI;
  }
  else
  {

    return boost::none;
  }
}

std::vector<uint8_t> Signature(const uint8_t* signature, const size_t signaturesize)
{
  std::vector<uint8_t> result;
  if (signature && signaturesize)
  {
    result = std::vector<uint8_t>(signature, signature + signaturesize);

  }
  return result;
}

///// Methods /////

ExportProgressWindow::ExportTrackConnection::ExportTrackConnection(const EXPORTFORMAT exportformat, const std::shared_ptr<std::recursive_mutex>& filewritermutex, const std::shared_ptr<file::FileWrite>& filewriter, const uint64_t starttime, const uint64_t endtime, const QDir& directory, const uint64_t deviceindex, const uint64_t recordingtoken, const QString& devicename, const QString& deviceaddress, const QString& recordingname, const QString& recordinglocation, const monocle::TrackType tracktype, const QString& trackdescription, const utility::PublicKey& publickey, const uint64_t trackid, const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port) :
  Connection(MainWindow::Instance()->GetIOServicePool().GetIoService(), proxyparams, address, port),
  state_(EXPORTTRACKSTATE_CONNECTING),
  exportformat_(exportformat),
  filewritermutex_(filewritermutex),
  filewriter_(filewriter),
  starttime_(starttime),
  endtime_(endtime),
  directory_(directory),
  deviceindex_(deviceindex),
  recordingtoken_(recordingtoken),
  devicename_(devicename),
  deviceaddress_(deviceaddress),
  recordingname_(recordingname),
  recordinglocation_(recordinglocation),
  tracktype_(tracktype),
  trackdescription_(trackdescription),
  publickey_(publickey),
  trackid_(trackid),
  currentprogress_(0.0f),
  totalsize_(0),
  currentspeed_(40),
  framerate_(25.0f),
  framecount_(0)
{

}

ExportProgressWindow::ExportTrackConnection::~ExportTrackConnection()
{
  getauthenticatenonce_.Close();
  authenticate_.Close();
  createstream_.Close();
  controlstream_.Close();
}

void ExportProgressWindow::ExportTrackConnection::AddFrame(const ImageBuffer& imagebuffer, const float progress, const uint64_t timestamp, const size_t size)
{
  if (!savevideo_)
  {
    const QDateTime start = QDateTime::fromMSecsSinceEpoch(starttime_, Qt::UTC);
    const QDateTime end = QDateTime::fromMSecsSinceEpoch(endtime_, Qt::UTC);
    static const QString format("ddMMyyyyhhmmss");
    const QString filename = recordingname_ + "-" + QString::number(trackid_) + "-" + start.toString(format) + "-" + end.toString(format) + ".avi";

    // Clear up anything in the way
    boost::system::error_code err;
    boost::filesystem::remove((directory_.path() + "/" + filename).toStdString(), err);

    // Initialise the file
    savevideo_ = std::make_unique<SaveVideo>();
    if (savevideo_->Init(directory_.path(), filename, imagebuffer.widths_[0], imagebuffer.heights_[0], framerate_))
    {
      audit_.push_back("ExportTrack SaveVideo::Init failed");
      state_ = EXPORTTRACKSTATE_ERROR;
      return;
    }
  }

  if (savevideo_->AddFrame(imagebuffer, framecount_++))
  {
    audit_.push_back("ExportTrack SaveVideo::AddFrame failed");

  }

  currentprogress_ = std::min(1.0f, progress);
  totalsize_ += size;
  currentspeed_.push_back(std::make_pair(std::chrono::steady_clock::now(), size));
}

uint64_t ExportProgressWindow::ExportTrackConnection::GetCurrentSpeed() const
{
  if (currentspeed_.empty())
  {

    return 0;
  }

  uint64_t totalsize = 0;
  for (const std::pair<std::chrono::steady_clock::time_point, size_t>& size : currentspeed_)
  {
    totalsize += (size.second / 1024);

  }
  const uint64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - currentspeed_.front().first).count();
  return ((totalsize * 1000) / (duration ? duration : 1));
}

ExportProgressWindow::ExportProgressWindow(QWidget* parent, const QString& directory, const EXPORTFORMAT exportformat, const uint64_t starttime, const uint64_t endtime, const std::vector< QSharedPointer<client::Recording> >& recordings) :
  QDialog(parent),
  directory_(directory),
  exportformat_(exportformat),
  filewritermutex_(std::make_shared<std::recursive_mutex>()),
  filewriter_(std::make_shared<file::FileWrite>())
{
  ui_.setupUi(this);

  ui_.labeltotal->setText(QString::number(recordings.size()));

  start_ = std::chrono::steady_clock::now();
  timer_ = startTimer(400);

  if (exportformat_ == EXPORTFORMAT_MONOCLE)
  {
    const QDateTime start = QDateTime::fromMSecsSinceEpoch(starttime, Qt::UTC);
    const QDateTime end = QDateTime::fromMSecsSinceEpoch(endtime, Qt::UTC);
    static const QString format("ddMMyyyyhhmmss");

    // Clear up anything in the way
    const std::string path = (directory_.path() + "/Recording-" + start.toString(format) + "-" + end.toString(format) + ".mon").toStdString();

    boost::system::error_code err;
    boost::filesystem::remove(path, err);

    if (filewriter_->Init(path))
    {
      QMessageBox(QMessageBox::Information, tr("Error"), "Unable to initialise file", QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    if (filewriter_->Open())
    {
      QMessageBox(QMessageBox::Information, tr("Error"), "Unable to open file", QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  }

  std::vector< boost::shared_ptr<Device> > devices;
  for (const QSharedPointer<client::Recording>& recording : recordings)
  {
    // Figure out a nice device index to use because the file requires it
    uint64_t deviceindex = 0;
    std::vector< boost::shared_ptr<Device> >::const_iterator i = std::find(devices.cbegin(), devices.cend(), recording->GetDevice());
    if (i == devices.cend())
    {
      devices.push_back(recording->GetDevice());
      i = devices.cend() - 1;
      deviceindex = devices.size() - 1;
    }
    else
    {
      deviceindex = std::distance(devices.cbegin(), i);

    }

    // Stream all the tracks from all the recordings
    for (const QSharedPointer<client::RecordingTrack>& track : recording->GetTracks())
    {
      boost::shared_ptr<ExportTrackConnection> exporttrackconnection = boost::make_shared<ExportTrackConnection>(exportformat_, filewritermutex_, filewriter_, starttime, endtime, directory_, deviceindex, recording->GetToken(), recording->GetDevice()->GetName(), recording->GetDevice()->GetAddress(), recording->GetName(), recording->GetLocation(), track->GetTrackType(), track->GetDescription(), recording->GetDevice()->GetPublicKey(), track->GetId(), recording->GetDevice()->GetProxyParams(), recording->GetDevice()->GetAddress(), recording->GetDevice()->GetPort());
      exporttrackconnection->state_ = EXPORTTRACKSTATE_CONNECTING;
      exporttrackconnection->connect_ = exporttrackconnection->Connect([this, exporttrackconnection, username = recording->GetDevice()->GetUsername(), password = recording->GetDevice()->GetPassword(), recordingname = recording->GetName(), recordingtoken = recording->GetToken(), trackid = track->GetId(), starttime, endtime](const boost::system::error_code& err) mutable
      {
        std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
        if (err)
        {
          exporttrackconnection->audit_.push_back("ExportTrack Failed Unable to connect: " + recordingname);
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }
        exporttrackconnection->state_ = EXPORTTRACKSTATE_CONNECTED;

        exporttrackconnection->getauthenticatenonce_ = exporttrackconnection->GetAuthenticationNonce([this, exporttrackconnection, username, password, recordingname, recordingtoken, trackid, starttime, endtime](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse) mutable
        {
          std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
          if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
          {
            exporttrackconnection->audit_.push_back("ExportTrack GetAuthenticateNonce failed : " + recordingname);
            exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
            return;
          }

          const std::string clientnonce = utility::GenerateRandomString(32);
          exporttrackconnection->authenticate_ = exporttrackconnection->Authenticate(username.toStdString(), clientnonce, monocle::AuthenticateDigest(username.toStdString(), password.toStdString(), getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this, exporttrackconnection, recordingname, recordingtoken, trackid, starttime, endtime](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse) mutable
          {
            std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
            if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
            {
              exporttrackconnection->audit_.push_back("ExportTrack Authenticate failed: " + recordingname);
              exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
              return;
            }

            exporttrackconnection->createstream_ = exporttrackconnection->CreateStream(recordingtoken, trackid, [this, exporttrackconnection, recordingname, recordingtoken, trackid, starttime, endtime](const std::chrono::steady_clock::duration latency, const monocle::client::CREATESTREAMRESPONSE& createstreamresponse) mutable
            {
              std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
              if (createstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
              {
                exporttrackconnection->audit_.push_back("ExportTrack CreateStream failed: " + recordingname);
                exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
                return;
              }
              exporttrackconnection->state_ = EXPORTTRACKSTATE_OPENING;
              exporttrackconnection->codecindices_ = createstreamresponse.codecindices_;
              exporttrackconnection->controlstream_ = exporttrackconnection->ControlStream(createstreamresponse.token_, 0, true, false, true, starttime, endtime, boost::none, false, [this, exporttrackconnection, recordingname](const std::chrono::steady_clock::duration latency, const monocle::client::CONTROLSTREAMRESPONSE& controlstreamresponse)
              {
                std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
                if (controlstreamresponse.GetErrorCode() != monocle::ErrorCode::Success)
                {
                  exporttrackconnection->audit_.push_back("ExportTrack ControlStream failed: " + recordingname);
                  exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
                  return;
                }
              });
              exporttrackconnection->state_ = EXPORTTRACKSTATE_STREAMING;
            }, ExportProgressWindow::ControlStreamEndCallback, ExportProgressWindow::H265Callback, ExportProgressWindow::H264Callback, ExportProgressWindow::MetadataCallback, ExportProgressWindow::JPEGCallback, ExportProgressWindow::MPEG4Callback, ExportProgressWindow::NewCodecIndexCallback, exporttrackconnection.get());
          });
        });
      });
      exporttrackconnections_.push_back(exporttrackconnection);
    }
  }
}

ExportProgressWindow::~ExportProgressWindow()
{
  for (boost::shared_ptr<ExportTrackConnection>& exporttrackconnection : exporttrackconnections_)
  {
    exporttrackconnection->Destroy();

  }
}

void ExportProgressWindow::timerEvent(QTimerEvent*)
{
  const uint64_t duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_).count();
  float totalprogress = 0.0f;
  uint64_t totalsize = 0;
  uint64_t currentspeed = 0;
  bool complete = true;
  int numcompleted = 0;
  for (boost::shared_ptr<ExportTrackConnection>& exporttrackconnection : exporttrackconnections_)
  {
    totalprogress += exporttrackconnection->currentprogress_;
    totalsize += exporttrackconnection->totalsize_;
    currentspeed += exporttrackconnection->GetCurrentSpeed();

    if (exporttrackconnection->state_ == EXPORTTRACKSTATE_SUCCESS)
    {
      ++numcompleted;

    }
    else if (exporttrackconnection->state_ == EXPORTTRACKSTATE_ERROR)
    {
      ++numcompleted;

    }
    else
    {
      complete = false;

    }
  }

  const uint64_t totalsizekb = totalsize / 1024;
  totalprogress /= static_cast<float>(exporttrackconnections_.size());
  ui_.progressexport->setValue(static_cast<int>(totalprogress * 100.0));
  ui_.labelcurrentspeed->setText(SpeedToString(currentspeed));
  ui_.labelaveragespeed->setText(SpeedToString(duration ? (totalsizekb / duration) : totalsizekb));
  ui_.labeltotalexported->setText(QString::number(totalsizekb) + "kB");
  ui_.labeltimeelapsed->setText(QString::number(duration) + "seconds");
  ui_.labelexported->setText(QString::number(numcompleted));

  if (complete)
  {
    switch (exportformat_)
    {
      case EXPORTFORMAT_MONOCLE:
      {
        std::lock_guard<std::recursive_mutex> lock(*filewritermutex_);
        QStringList messages;
        boost::container::flat_set<file::DEVICE> devices;
        for (boost::shared_ptr<ExportTrackConnection>& exporttrackconnection : exporttrackconnections_)
        {
          std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
          if (exporttrackconnection->state_ == EXPORTTRACKSTATE_SUCCESS)
          {
            messages.push_back(exporttrackconnection->recordingname_ + " " + exporttrackconnection->trackdescription_ + ": Success " + QString::number(exporttrackconnection->totalsize_ / 1024) + "kB " + QString::number(std::chrono::duration_cast<std::chrono::seconds>(exporttrackconnection->end_ - start_).count()) + "seconds");

          }
          else // (savevideo->GetState() == SAVEVIDEOSTATE_ERROR)
          {
            messages.push_back(exporttrackconnection->recordingname_ + " " + exporttrackconnection->trackdescription_ + ": Error ");

          }
          boost::container::flat_set<file::CODEC> codecsindices;
          for (const monocle::CODECINDEX& codecindex : exporttrackconnection->codecindices_)
          {
            codecsindices.insert(file::CODEC(codecindex.id_, static_cast<int>(codecindex.codec_), codecindex.parameters_));

          }

          boost::container::flat_set<file::DEVICE>::iterator device = std::find_if(devices.begin(), devices.end(), [exporttrackconnection](const file::DEVICE& device) { return (device.index_ == exporttrackconnection->deviceindex_); });
          if (device == devices.cend())
          {
            device = devices.insert(file::DEVICE(exporttrackconnection->deviceindex_, exporttrackconnection->devicename_.toStdString(), exporttrackconnection->deviceaddress_.toStdString(), exporttrackconnection->publickey_.GetString())).first;;

          }

          boost::container::flat_set<file::RECORDING>::iterator recording = std::find_if(device->recordings_.begin(), device->recordings_.end(), [exporttrackconnection](const file::RECORDING& recording) { return (recording.index_ == exporttrackconnection->recordingtoken_); });
          if (recording == device->recordings_.cend())
          {
            recording = device->recordings_.insert(file::RECORDING(exporttrackconnection->recordingtoken_, exporttrackconnection->recordingname_.toStdString(), exporttrackconnection->recordinglocation_.toStdString())).first;

          }

          if (exporttrackconnection->tracktype_ == monocle::TrackType::Video)
          {
            recording->videotracks_.insert(file::TRACK(exporttrackconnection->trackid_, exporttrackconnection->trackdescription_.toStdString(), codecsindices));

          }
          else if (exporttrackconnection->tracktype_ == monocle::TrackType::Audio)
          {
            recording->audiotracks_.insert(file::TRACK(exporttrackconnection->trackid_, exporttrackconnection->trackdescription_.toStdString(), codecsindices));

          }
          else if (exporttrackconnection->tracktype_ == monocle::TrackType::Metadata)
          {
            recording->metadatatracks_.insert(file::TRACK(exporttrackconnection->trackid_, exporttrackconnection->trackdescription_.toStdString(), codecsindices));

          }
          else if (exporttrackconnection->tracktype_ == monocle::TrackType::ObjectDetector)
          {
            recording->objectdetectortracks_.insert(file::TRACK(exporttrackconnection->trackid_, exporttrackconnection->trackdescription_.toStdString(), codecsindices));

          }
        }
        if (filewriter_->Close(file::FILE(devices)))
        {
          QMessageBox(QMessageBox::Information, tr("Error"), "Unable to close file", QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          return;
        }
        QMessageBox(QMessageBox::Information, tr("Complete"), messages.join("\r\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        break;
      }
      case EXPORTFORMAT_AVI:
      {
        QStringList messages;
        for (boost::shared_ptr<ExportTrackConnection>& exporttrackconnection : exporttrackconnections_)
        {
          std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
          if (exporttrackconnection->state_ == EXPORTTRACKSTATE_SUCCESS)
          {
            messages.push_back(exporttrackconnection->recordingname_ + " " + exporttrackconnection->trackdescription_ + ": Success " + QString::number(exporttrackconnection->totalsize_ / 1024) + "kB " + QString::number(std::chrono::duration_cast<std::chrono::seconds>(exporttrackconnection->end_ - start_).count()) + "seconds");

          }
          else // (savevideo->GetState() == SAVEVIDEOSTATE_ERROR)
          {
            messages.push_back(exporttrackconnection->recordingname_ + " " + exporttrackconnection->trackdescription_ + ": Error ");

          }
        }
        QMessageBox(QMessageBox::Information, tr("Complete"), messages.join("\r\n"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        break;
      }
    }
    ui_.buttonclose->setText("Close");
    killTimer(timer_);
    timer_ = -1;
  }
}

void ExportProgressWindow::ControlStreamEndCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  switch (exporttrackconnection->exportformat_)
  {
    case EXPORTFORMAT_MONOCLE:
    {
      exporttrackconnection->state_ = EXPORTTRACKSTATE_SUCCESS;
      break;
    }
    case EXPORTFORMAT_AVI:
    {
      std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
      if (!exporttrackconnection->savevideo_)
      {
        exporttrackconnection->audit_.push_back("ExportTrack SaveVideo not initialised failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }

      // Finish writing the video
      if (exporttrackconnection->savevideo_->Destroy())
      {
        exporttrackconnection->audit_.push_back("ExportTrack SaveVideo::Destroy failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }
      break;
    }
  }

  exporttrackconnection->currentprogress_ = 1.0f;
  exporttrackconnection->end_ = std::chrono::steady_clock::now();
  exporttrackconnection->state_ = EXPORTTRACKSTATE_SUCCESS;
}

void ExportProgressWindow::H265Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  switch (exporttrackconnection->exportformat_)
  {
    case EXPORTFORMAT_MONOCLE:
    {
      std::lock_guard<std::recursive_mutex> lock(*exporttrackconnection->filewritermutex_);
      if (exporttrackconnection->filewriter_->WriteH265Frame(exporttrackconnection->deviceindex_, exporttrackconnection->recordingtoken_, exporttrackconnection->trackid_, codecindex, reinterpret_cast<const uint8_t*>(signaturedata), signaturedatasize, marker, timestamp, Signature(signature, signaturesize), donlfield, std::vector<uint32_t>(offsets, offsets + numoffsets)))
      {
        exporttrackconnection->audit_.push_back("FileWrite::WriteFrame failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }
      exporttrackconnection->currentprogress_ = std::min(1.0f, progress);
      exporttrackconnection->totalsize_ += size;
      exporttrackconnection->currentspeed_.push_back(std::make_pair(std::chrono::steady_clock::now(), size));
      break;
    }
    case EXPORTFORMAT_AVI:
    {
      std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
      if (exporttrackconnection->state_ == EXPORTTRACKSTATE_ERROR)
      {

        return;
      }

      // Find the decoder
      std::vector< std::unique_ptr<H265Decoder> >::iterator h265decoder = std::find_if(exporttrackconnection->h265decoders_.begin(), exporttrackconnection->h265decoders_.end(), [codecindex](const std::unique_ptr<H265Decoder>& h265decoder) { return (h265decoder->GetId() == codecindex); });
      if (h265decoder == exporttrackconnection->h265decoders_.end())
      {
        std::vector<monocle::CODECINDEX>::const_iterator ci = std::find_if(exporttrackconnection->codecindices_.cbegin(), exporttrackconnection->codecindices_.cend(), [codecindex](const monocle::CODECINDEX& ci) { return ((ci.codec_ == monocle::Codec::H265) && (ci.id_ == codecindex)); });
        if (ci == exporttrackconnection->codecindices_.cend())
        {
          exporttrackconnection->audit_.push_back("ExportTrack invalid codec index");
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }

        // Create the decoder
        const std::string FRAMERATE = "framerate=";
        const std::vector<std::string> parameters = ci->GetParameters();
        std::vector<std::string>::const_iterator framerate = std::find_if(parameters.cbegin(), parameters.cend(), [&FRAMERATE](const std::string& parameter) { return (boost::istarts_with(parameter, FRAMERATE)); });
        if (framerate != parameters.cend())
        {
          try
          {
            exporttrackconnection->framerate_ = std::stod(framerate->substr(FRAMERATE.size(), std::string::npos));

          }
          catch (...)
          {

          }
        }

        std::unique_ptr<H265Decoder> decoder = std::make_unique<H265Decoder>(ci->id_, utility::PublicKey());
        if (decoder->Init(ci->GetParameters()))
        {
          exporttrackconnection->audit_.push_back(QString::fromStdString("ExportTrack H265Decoder::Init failed"));
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }
        exporttrackconnection->h265decoders_.emplace_back(std::move(decoder));
        h265decoder = exporttrackconnection->h265decoders_.end() - 1;
      }

      ImageBuffer imagebuffer = (*h265decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, static_cast<int>(signaturesize), signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &exporttrackconnection->freeframebuffer_);
      if (!imagebuffer.buffer_)
      {
        // Ignore this because an ImageBuffer is not always produced
        return;
      }

      exporttrackconnection->AddFrame(imagebuffer, progress, timestamp, size);
      exporttrackconnection->freeframebuffer_.AddFreeImage(imagebuffer);
      break;
    }
  }
}

void ExportProgressWindow::H264Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  switch (exporttrackconnection->exportformat_)
  {
    case EXPORTFORMAT_MONOCLE:
    {
      std::lock_guard<std::recursive_mutex> lock(*exporttrackconnection->filewritermutex_);
      if (exporttrackconnection->filewriter_->WriteH264Frame(exporttrackconnection->deviceindex_, exporttrackconnection->recordingtoken_, exporttrackconnection->trackid_, codecindex, reinterpret_cast<const uint8_t*>(signaturedata), signaturedatasize, marker, timestamp, Signature(signature, signaturesize), std::vector<uint32_t>(offsets, offsets + numoffsets)))
      {
        exporttrackconnection->audit_.push_back("FileWrite::WriteFrame failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }
      exporttrackconnection->currentprogress_ = std::min(1.0f, progress);
      exporttrackconnection->totalsize_ += size;
      exporttrackconnection->currentspeed_.push_back(std::make_pair(std::chrono::steady_clock::now(), size));
      break;
    }
    case EXPORTFORMAT_AVI:
    {
      std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
      if (exporttrackconnection->state_ == EXPORTTRACKSTATE_ERROR)
      {

        return;
      }

      // Find the decoder
      std::vector< std::unique_ptr<H264Decoder> >::iterator h264decoder = std::find_if(exporttrackconnection->h264decoders_.begin(), exporttrackconnection->h264decoders_.end(), [codecindex](const std::unique_ptr<H264Decoder>& h264decoder) { return (h264decoder->GetId() == codecindex); });
      if (h264decoder == exporttrackconnection->h264decoders_.end())
      {
        std::vector<monocle::CODECINDEX>::const_iterator ci = std::find_if(exporttrackconnection->codecindices_.cbegin(), exporttrackconnection->codecindices_.cend(), [codecindex](const monocle::CODECINDEX& ci) { return ((ci.codec_ == monocle::Codec::H264) && (ci.id_ == codecindex)); });
        if (ci == exporttrackconnection->codecindices_.cend())
        {
          exporttrackconnection->audit_.push_back("ExportTrack invalid codec index");
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }

        // Create the decoder
        const std::string FRAMERATE = "framerate=";
        const std::vector<std::string> parameters = ci->GetParameters();
        std::vector<std::string>::const_iterator framerate = std::find_if(parameters.cbegin(), parameters.cend(), [&FRAMERATE](const std::string& parameter) { return (boost::istarts_with(parameter, FRAMERATE)); });
        if (framerate != parameters.cend())
        {
          try
          {
            exporttrackconnection->framerate_ = std::stod(framerate->substr(FRAMERATE.size(), std::string::npos));

          }
          catch (...)
          {

          }
        }
        
        std::unique_ptr<H264Decoder> decoder = std::make_unique<H264Decoder>(ci->id_, utility::PublicKey(), nullptr);
        if (decoder->Init(ci->GetParameters()))
        {
          exporttrackconnection->audit_.push_back(QString::fromStdString("ExportTrack H264Decoder::Init failed"));
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }
        exporttrackconnection->h264decoders_.emplace_back(std::move(decoder));
        h264decoder = exporttrackconnection->h264decoders_.end() - 1;
      }

      ImageBuffer imagebuffer = (*h264decoder)->Decode(playrequestindex, marker, timestamp, sequencenum, signature, static_cast<int>(signaturesize), signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &exporttrackconnection->freeframebuffer_);
      if (!imagebuffer.buffer_)
      {
        // Ignore this because an ImageBuffer is not always produced
        return;
      }

      exporttrackconnection->AddFrame(imagebuffer, progress, timestamp, size);
      exporttrackconnection->freeframebuffer_.AddFreeImage(imagebuffer);
      break;
    }
  }
}

void ExportProgressWindow::MetadataCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  switch (exporttrackconnection->exportformat_)
  {
    case EXPORTFORMAT_MONOCLE:
    {
      std::lock_guard<std::recursive_mutex> lock(*exporttrackconnection->filewritermutex_);
      if (exporttrackconnection->filewriter_->WriteMetadataFrame(exporttrackconnection->deviceindex_, exporttrackconnection->recordingtoken_, exporttrackconnection->trackid_, codecindex, reinterpret_cast<const uint8_t*>(signaturedata), signaturedatasize, timestamp, static_cast<file::MetadataFrameType>(metadataframetype), Signature(signature, signaturesize)))
      {
        exporttrackconnection->audit_.push_back("FileWrite::WriteFrame failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }
      exporttrackconnection->currentprogress_ = std::min(1.0f, progress);
      exporttrackconnection->totalsize_ += size;
      exporttrackconnection->currentspeed_.push_back(std::make_pair(std::chrono::steady_clock::now(), size));
      break;
    }
  }
}

void ExportProgressWindow::JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  switch (exporttrackconnection->exportformat_)
  {
    case EXPORTFORMAT_MONOCLE:
    {
      std::lock_guard<std::recursive_mutex> lock(*exporttrackconnection->filewritermutex_);

      std::array<uint8_t, 64> lqtarr;
      if (lqt)
      {
        memcpy(lqtarr.data(), lqt, lqtarr.size());

      }
      else
      {
        lqtarr.fill(0);

      }

      std::array<uint8_t, 64> cqtarr;
      if (cqt)
      {
        memcpy(cqtarr.data(), cqt, cqtarr.size());

      }
      else
      {
        cqtarr.fill(0);

      }

      if (exporttrackconnection->filewriter_->WriteJPEGFrame(exporttrackconnection->deviceindex_, exporttrackconnection->recordingtoken_, exporttrackconnection->trackid_, codecindex, reinterpret_cast<const uint8_t*>(signaturedata), signaturedatasize, timestamp, Signature(signature, signaturesize), restartinterval, typespecificfragmentoffset, type, q, width, height, lqtarr, cqtarr))
      {
        exporttrackconnection->audit_.push_back("FileWrite::WriteFrame failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }
      exporttrackconnection->currentprogress_ = std::min(1.0f, progress);
      exporttrackconnection->totalsize_ += size;
      exporttrackconnection->currentspeed_.push_back(std::make_pair(std::chrono::steady_clock::now(), size));
      break;
    }
    case EXPORTFORMAT_AVI:
    {
      std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
      if (exporttrackconnection->state_ == EXPORTTRACKSTATE_ERROR)
      {

        return;
      }

      if (!exporttrackconnection->mjpegdecoder_)
      {
        exporttrackconnection->mjpegdecoder_ = std::make_unique<MJpegDecoder>(codecindex, utility::PublicKey());
        if (exporttrackconnection->mjpegdecoder_->Init())
        {
          exporttrackconnection->audit_.push_back(QString::fromStdString("ExportTrack MJpegDecoder::Init failed"));
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        }
      }

      ImageBuffer imagebuffer = exporttrackconnection->mjpegdecoder_->Decode(playrequestindex, timestamp, sequencenum, signature, static_cast<int>(signaturesize), signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &exporttrackconnection->freeframebuffer_);
      if (!imagebuffer.buffer_)
      {
        // Ignore this because an ImageBuffer is not always produced
        return;
      }

      exporttrackconnection->AddFrame(imagebuffer, progress, timestamp, size);
      exporttrackconnection->freeframebuffer_.AddFreeImage(imagebuffer);
      break;
    }
  }
}

void ExportProgressWindow::MPEG4Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  switch (exporttrackconnection->exportformat_)
  {
    case EXPORTFORMAT_MONOCLE:
    {
      std::lock_guard<std::recursive_mutex> lock(*exporttrackconnection->filewritermutex_);
      if (exporttrackconnection->filewriter_->WriteMPEG4Frame(exporttrackconnection->deviceindex_, exporttrackconnection->recordingtoken_, exporttrackconnection->trackid_, codecindex, reinterpret_cast<const uint8_t*>(signaturedata), signaturedatasize, marker, timestamp, Signature(signature, signaturesize)))
      {
        exporttrackconnection->audit_.push_back("FileWrite::WriteFrame failed");
        exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
        return;
      }
      exporttrackconnection->currentprogress_ = std::min(1.0f, progress);
      exporttrackconnection->totalsize_ += size;
      exporttrackconnection->currentspeed_.push_back(std::make_pair(std::chrono::steady_clock::now(), size));
      break;
    }
    case EXPORTFORMAT_AVI:
    {
      std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
      if (exporttrackconnection->state_ == EXPORTTRACKSTATE_ERROR)
      {

        return;
      }

      // Find the decoder
      std::vector< std::unique_ptr<MPEG4Decoder> >::iterator mpeg4decoder = std::find_if(exporttrackconnection->mpeg4decoders_.begin(), exporttrackconnection->mpeg4decoders_.end(), [codecindex](const std::unique_ptr<MPEG4Decoder>& mpeg4decoder) { return (mpeg4decoder->GetId() == codecindex); });
      if (mpeg4decoder == exporttrackconnection->mpeg4decoders_.end())
      {
        std::vector<monocle::CODECINDEX>::const_iterator ci = std::find_if(exporttrackconnection->codecindices_.cbegin(), exporttrackconnection->codecindices_.cend(), [codecindex](const monocle::CODECINDEX& ci) { return ((ci.codec_ == monocle::Codec::MPEG4) && (ci.id_ == codecindex)); });
        if (ci == exporttrackconnection->codecindices_.cend())
        {
          exporttrackconnection->audit_.push_back("ExportTrack invalid codec index");
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }

        // Create the decoder
        const std::string FRAMERATE = "framerate=";
        const std::vector<std::string> parameters = ci->GetParameters();
        std::vector<std::string>::const_iterator framerate = std::find_if(parameters.cbegin(), parameters.cend(), [&FRAMERATE](const std::string& parameter) { return (boost::istarts_with(parameter, FRAMERATE)); });
        if (framerate != parameters.cend())
        {
          try
          {
            exporttrackconnection->framerate_ = std::stod(framerate->substr(FRAMERATE.size(), std::string::npos));

          }
          catch (...)
          {

          }
        }

        utility::PublicKey publickey;
        std::unique_ptr<MPEG4Decoder> decoder = std::make_unique<MPEG4Decoder>(ci->id_, publickey);
        if (decoder->Init(ci->GetParameters()))
        {
          exporttrackconnection->audit_.push_back(QString::fromStdString("ExportTrack MPEG4Decoder::Init failed"));
          exporttrackconnection->state_ = EXPORTTRACKSTATE_ERROR;
          return;
        }
        exporttrackconnection->mpeg4decoders_.emplace_back(std::move(decoder));
        mpeg4decoder = exporttrackconnection->mpeg4decoders_.end() - 1;
      }

      ImageBuffer imagebuffer = (*mpeg4decoder)->Decode(playrequestindex, timestamp, sequencenum, signature, static_cast<int>(signaturesize), signaturedata, signaturedatasize, reinterpret_cast<const uint8_t*>(framedata), static_cast<unsigned int>(size), &exporttrackconnection->freeframebuffer_);
      if (!imagebuffer.buffer_)
      {
        // Ignore this because an ImageBuffer is not always produced
        return;
      }

      exporttrackconnection->AddFrame(imagebuffer, progress, timestamp, size);
      exporttrackconnection->freeframebuffer_.AddFreeImage(imagebuffer);
      break;
    }
  }
}

void ExportProgressWindow::NewCodecIndexCallback(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata)
{
  ExportTrackConnection* exporttrackconnection = reinterpret_cast<ExportTrackConnection*>(callbackdata);
  std::lock_guard<std::recursive_mutex> lock(exporttrackconnection->mutex_);
  exporttrackconnection->codecindices_.push_back(monocle::CODECINDEX(id, codec, parameters, timestamp));
}

void ExportProgressWindow::on_buttonclose_clicked()
{
  accept();

}

void ExportProgressWindow::on_buttonopen_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(directory_.path()));

}

QString ExportProgressWindow::SpeedToString(const uint64_t speedkbps) const
{
  if (speedkbps == 0)
  {

    return QString("0kB/s");
  }

  if (speedkbps > 1000)
  {

    return QString::number(speedkbps / 1024) + "." + QString::number(speedkbps % 1024) + "MB/s";
  }
  else
  {

    return QString::number(speedkbps) + "kB/s";
  }
}

}

QT_END_NAMESPACE
