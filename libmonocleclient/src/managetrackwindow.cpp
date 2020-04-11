// managetrackwindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackwindow.h"

#include <boost/lexical_cast.hpp>
#include <monocleprotocol/streamingprotocol_generated.h>
#include <monocleprotocol/tracktype_generated.h>
#include <network/uri.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <QString>
#include <QTimer>

#include "monocleclient/mainwindow.h"
#include "monocleclient/device.h"
#include "monocleclient/managetrackfileswindow.h"
#include "monocleclient/managetrackfindonvifdevicewindow.h"
#include "monocleclient/managetrackobjectdetectorwindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Globals /////

const int STREAMING_PROTOCOL_ROLE = Qt::UserRole;
const int ROTATION_ROLE = Qt::UserRole + 1;

///// Methods /////

ManageTrackWindow::ManageTrackWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingJob>& recordingjob, const QSharedPointer<RecordingJobSource>& recordingjobsource, const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& recordingtrack, const QString& uri) :
  QDialog(parent),
  mutex_(boost::make_shared<std::recursive_mutex>()),
  profilemodel_(new QStringListModel(this)),
  sourcetagmodel_(new QStringListModel(this)),
  profilecompleter_(new QCompleter(this)),
  sourcetagcompleter_(new QCompleter(this)),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob),
  recordingjobsource_(recordingjobsource),
  recordingjobsourcetrack_(recordingjobsourcetrack),
  recordingtrack_(recordingtrack),
  accuracy_(1),
  humans_(true),
  bicycles_(true),
  cars_(true),
  motorbikes_(true),
  buses_(true),
  trucks_(true),
  backpacks_(true),
  umbrellas_(true),
  handbags_(true),
  suitcases_(true),
  cats_(true),
  dogs_(true),
  aeroplanes_(true),
  trains_(true),
  boats_(true),
  horses_(true),
  humanssensitivity_(0.5),
  bicyclessensitivity_(0.5),
  carssensitivity_(0.5),
  motorbikessensitivity_(0.5),
  busessensitivity_(0.5),
  truckssensitivity_(0.5),
  backpackssensitivity_(0.5),
  umbrellassensitivity_(0.5),
  handbagssensitivity_(0.5),
  suitcasessensitivity_(0.5),
  catssensitivity_(0.5),
  dogssensitivity_(0.5),
  aeroplanessensitivity_(0.5),
  trainssensitivity_(0.5),
  boatssensitivity_(0.5),
  horsessensitivity_(0.5)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &ManageTrackWindow::reject);

  if (device_)
  {
    ui_.labeldevice->setVisible(false);
    ui_.combodevice->setVisible(false);
  }
  else
  {
    for (const boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
    {
      //TODO in the future we can look at the address ranges this device is on, and compare that with the address range of the uri, and then put them at the top if they share possible connectivity
      ui_.combodevice->addItem(device->GetName() + "(" + device->GetAddress() + ")", device->GetIdentifier());

    }

    if (ui_.combodevice->count() == 0)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("No valid devices"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      QTimer::singleShot(std::chrono::milliseconds(1), this, [this]() { reject(); });
      return;
    }

    ui_.combodevice->setCurrentIndex(0);

    boost::shared_ptr<Device> currentdevice = MainWindow::Instance()->GetDeviceMgr().GetDevice(ui_.combodevice->currentData().toULongLong());
    if (!currentdevice)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find device: ") + QString::number(ui_.combodevice->currentData().toULongLong()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      QTimer::singleShot(std::chrono::milliseconds(1), this, [this]() { reject(); });
      return;
    }

    device_ = currentdevice;
  }

  if (recording_ && recordingtrack)
  {
    connect(recording_.data(), &Recording::TrackRemoved, this, &ManageTrackWindow::TrackRemoved, Qt::QueuedConnection);

  }

  ui_.edituri->setText(uri);

  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::TCPInterleaved)), static_cast<int>(monocle::StreamingProtocol::TCPInterleaved));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPUnicast)), static_cast<int>(monocle::StreamingProtocol::UDPUnicast));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPMulticast)), static_cast<int>(monocle::StreamingProtocol::UDPMulticast));

  ui_.comborotation->addItem("0", "0");
  ui_.comborotation->addItem("90", "90");
  ui_.comborotation->addItem("180", "180");
  ui_.comborotation->addItem("270", "270");

  profilecompleter_->setModel(profilemodel_);
  profilecompleter_->setCompletionMode(QCompleter::PopupCompletion);
  profilecompleter_->setMaxVisibleItems(16);
  ui_.editprofiletoken->setCompleter(profilecompleter_);

  sourcetagcompleter_->setModel(sourcetagmodel_);
  sourcetagcompleter_->setCompletionMode(QCompleter::PopupCompletion);
  sourcetagcompleter_->setMaxVisibleItems(16);
  ui_.editsourcetag->setCompleter(sourcetagcompleter_);

  // Find a job to get started with, it may not always be available
  if (recordingjob_ && recordingjobsource_ && recordingjobsourcetrack_ && recordingtrack_)
  {
    ui_.editdescription->setText(recordingtrack_->GetDescription());
    ui_.spinflushfrequency->setValue(recordingtrack_->GetFlushFrequency());
    ui_.checkdigitalsigning->setChecked(recordingtrack_->GetDigitalSigning());
    ui_.checkencrypt->setChecked(recordingtrack_->GetEncrypt());
    ui_.checkfixedfiles->setChecked(recordingtrack_->GetFixedFiles());

    if (recordingtrack_->GetFixedFiles())
    {
      for (const uint64_t filetoken : recordingtrack_->GetFiles())
      {
        QSharedPointer<File> file = device_->GetFile(filetoken);
        if (file)
        {
          files_.push_back(file);

        }
      }
    }
    else
    {
      files_ = device_->GetFiles();

    }

    QSharedPointer<Receiver> receiver = device_->GetReceiver(recordingjobsource_->GetReceiverToken());
    if (receiver)
    {
      ui_.edituri->setText(receiver->GetMediaUri());

      boost::optional<monocle::StreamingProtocol> streamingprotocol = receiver->GetStreamingProtocol();
      if (streamingprotocol.is_initialized())
      {
        int i = ui_.comboprotocol->findData(static_cast<int>(*streamingprotocol));
        if (i != -1)
        {
          ui_.comboprotocol->setCurrentIndex(i);

        }
      }

      ui_.editusername->setText(receiver->GetUsername());
      ui_.editpassword->setText(receiver->GetPassword());
    }

    const boost::optional<QString> profiletoken = recordingjobsourcetrack_->GetProfileToken();
    if (profiletoken.is_initialized())
    {
      ui_.editprofiletoken->setText(*profiletoken);

    }

    const boost::optional<QString> sourcetag = recordingjobsourcetrack_->GetSourceTag();
    if (sourcetag.is_initialized())
    {
      ui_.editsourcetag->setText(*sourcetag);

    }
    
    boost::optional<ROTATION> rotation = recordingjobsourcetrack_->GetRotation();
    if (rotation.is_initialized())
    {
      const int i = ui_.comborotation->findData(QString::number(static_cast<int>(*rotation)));
      if (i != -1)
      {
        ui_.comborotation->setCurrentIndex(i);

      }
    }

    // Object detector
    QSharedPointer<RecordingJob> job = GetJob();
    if (job)
    {
      const std::vector< QSharedPointer<RecordingJobSource> > sources = job->GetObjectDetectors(recordingtrack_->GetId());
      if (sources.size())
      {
        for (const QSharedPointer<RecordingJobSourceTrack>& sourcetrack : sources.front()->GetTracks())
        {
          QSharedPointer<RecordingTrack> track = sourcetrack->GetTrack();
          if (track && (track->GetTrackType() == monocle::TrackType::ObjectDetector)) // This should always be true after looking above at the receiver, but lets just check just in case...
          {
            ui_.checkobjectdetector->setChecked(true);
            accuracy_ = sourcetrack->GetObjectDetectorAccuracy().toInt();
            humans_ = sourcetrack->GetObjectDetectorHumansEnabled().toBool();
            bicycles_ = sourcetrack->GetObjectDetectorBicyclesEnabled().toBool();
            cars_ = sourcetrack->GetObjectDetectorCarsEnabled().toBool();
            motorbikes_ = sourcetrack->GetObjectDetectorMotorbikesEnabled().toBool();
            buses_ = sourcetrack->GetObjectDetectorBusesEnabled().toBool();
            trucks_ = sourcetrack->GetObjectDetectorTrucksEnabled().toBool();
            backpacks_ = sourcetrack->GetObjectDetectorBackpacksEnabled().toBool();
            umbrellas_ = sourcetrack->GetObjectDetectorUmbrellasEnabled().toBool();
            handbags_ = sourcetrack->GetObjectDetectorHandbagsEnabled().toBool();
            suitcases_ = sourcetrack->GetObjectDetectorSuitcasesEnabled().toBool();
            cats_ = sourcetrack->GetObjectDetectorCatsEnabled().toBool();
            dogs_ = sourcetrack->GetObjectDetectorDogsEnabled().toBool();
            aeroplanes_ = sourcetrack->GetObjectDetectorAeroplanesEnabled().toBool();
            trains_ = sourcetrack->GetObjectDetectorTrainsEnabled().toBool();
            boats_ = sourcetrack->GetObjectDetectorBoatsEnabled().toBool();
            horses_ = sourcetrack->GetObjectDetectorHorsesEnabled().toBool();
            humanssensitivity_ = sourcetrack->GetObjectDetectorHumansSensitivity().toDouble();
            bicyclessensitivity_ = sourcetrack->GetObjectDetectorBicyclesSensitivity().toDouble();
            carssensitivity_ = sourcetrack->GetObjectDetectorCarsSensitivity().toDouble();
            motorbikessensitivity_ = sourcetrack->GetObjectDetectorMotorbikesSensitivity().toDouble();
            busessensitivity_ = sourcetrack->GetObjectDetectorBusesSensitivity().toDouble();
            truckssensitivity_ = sourcetrack->GetObjectDetectorTrucksSensitivity().toDouble();
            backpackssensitivity_ = sourcetrack->GetObjectDetectorBackpacksSensitivity().toDouble();
            umbrellassensitivity_ = sourcetrack->GetObjectDetectorUmbrellasSensitivity().toDouble();
            handbagssensitivity_ = sourcetrack->GetObjectDetectorHandbagsSensitivity().toDouble();
            suitcasessensitivity_ = sourcetrack->GetObjectDetectorSuitcasesSensitivity().toDouble();
            catssensitivity_ = sourcetrack->GetObjectDetectorCatsSensitivity().toDouble();
            dogssensitivity_ = sourcetrack->GetObjectDetectorDogsSensitivity().toDouble();
            aeroplanessensitivity_ = sourcetrack->GetObjectDetectorAeroplanesSensitivity().toDouble();
            trainssensitivity_ = sourcetrack->GetObjectDetectorTrainsSensitivity().toDouble();
            boatssensitivity_ = sourcetrack->GetObjectDetectorBoatsSensitivity().toDouble();
            horsessensitivity_ = sourcetrack->GetObjectDetectorHorsesSensitivity().toDouble();
            break;
          }
        }
      }
    }
  }

  if (device_->GetNumCudaDevices() == 0)
  {
    ui_.checkobjectdetector->setDisabled(true);
    ui_.checkobjectdetector->setToolTip("No CUDA devices found on server");
    ui_.buttonobjectdetectorsettings->setDisabled(true);
    ui_.buttonobjectdetectorsettings->setToolTip("No CUDA devices found on server");
  }

  startTimer(std::chrono::milliseconds(50));

  on_edituri_textChanged(QString());
  on_checkfixedfiles_stateChanged(0);
  on_checkobjectdetector_stateChanged(0);
}

ManageTrackWindow::~ManageTrackWindow()
{
  addtrack2connection_.Close();

  if (mediaclient_)
  {
    mediaclient_->Destroy();
    mediaclient_.reset();
  }

  if (deviceclient_)
  {
    deviceclient_->Destroy();
    deviceclient_.reset();
  }

  rtspconnectconnection_.Close();
  rtspconnection_.Close();
  if (rtspclient_)
  {
    rtspclient_->Destroy();
    rtspclient_.reset();
  }
}

void ManageTrackWindow::timerEvent(QTimerEvent*)
{
  if (deviceclient_ && deviceclient_->IsInitialised())
  {
    deviceclient_->Update();

  }

  if (mediaclient_ && mediaclient_->IsInitialised())
  {
    mediaclient_->Update();

  }
}

QSharedPointer<RecordingJob> ManageTrackWindow::GetJob() const
{
  if (!recording_)
  {

    return nullptr;
  }

  if (recordingjob_)
  {

    return recordingjob_;
  }

  QSharedPointer<RecordingJob> job = recording_->GetActiveJob();
  if (job)
  {

    return job;
  }

  if (recording_->GetJobs().size())
  {
    return recording_->GetJobs().front();

  }

  return nullptr;
}

void ManageTrackWindow::DisableSource()
{
  ui_.editprofiletoken->setEnabled(false);
  ui_.editsourcetag->setEnabled(false);
  ui_.comboprotocol->setEnabled(false);
  ui_.editusername->setEnabled(false);
  ui_.editpassword->setEnabled(false);
  ui_.comborotation->setEnabled(false);
  ui_.checkobjectdetector->setEnabled(false);
  ui_.buttonobjectdetectorsettings->setEnabled(false);
}

void ManageTrackWindow::AddProfile(const onvif::Profile& profile)
{
  QTreeWidgetItem* top = new QTreeWidgetItem({ "Profile" });
  ui_.treedetails->addTopLevelItem(top);
  top->setExpanded(true);

  top->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.token_) }));

  if (profile.videosourceconfiguration_.is_initialized() && profile.videosourceconfiguration_->name_.is_initialized())
  {
    top->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.name_) }));

  }

  if (profile.videosourceconfiguration_.is_initialized())
  {
    if (profile.videosourceconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* videosourceconfiguration = new QTreeWidgetItem({ "Video Source" });
      top->addChild(videosourceconfiguration);
      videosourceconfiguration->setExpanded(true);

      videosourceconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.videosourceconfiguration_->token_) }));

      if (profile.videosourceconfiguration_->name_.is_initialized())
      {
        videosourceconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.videosourceconfiguration_->name_) }));

      }

      if (profile.videosourceconfiguration_->bounds_.is_initialized() && profile.videosourceconfiguration_->bounds_->x_.is_initialized() && profile.videosourceconfiguration_->bounds_->y_.is_initialized() && profile.videosourceconfiguration_->bounds_->width_.is_initialized() && profile.videosourceconfiguration_->bounds_->height_.is_initialized())
      {
        QTreeWidgetItem* bounds = new QTreeWidgetItem({ "Bounds" });
        videosourceconfiguration->addChild(bounds);
        bounds->setExpanded(true);

        bounds->addChild(new QTreeWidgetItem({ "x: " + QString::number(*profile.videosourceconfiguration_->bounds_->x_) }));
        bounds->addChild(new QTreeWidgetItem({ "y: " + QString::number(*profile.videosourceconfiguration_->bounds_->y_) }));
        bounds->addChild(new QTreeWidgetItem({ "width: " + QString::number(*profile.videosourceconfiguration_->bounds_->width_) }));
        bounds->addChild(new QTreeWidgetItem({ "height: " + QString::number(*profile.videosourceconfiguration_->bounds_->height_) }));
      }

      if (profile.videosourceconfiguration_->rotate_.is_initialized() && profile.videosourceconfiguration_->rotate_->rotatemode_.is_initialized())
      {
        QTreeWidgetItem* rotation = new QTreeWidgetItem({ "Rotation" });
        videosourceconfiguration->addChild(rotation);
        rotation->setExpanded(true);

        rotation->addChild(new QTreeWidgetItem({ "Mode: " + QString::fromStdString(onvif::ToString(*profile.videosourceconfiguration_->rotate_->rotatemode_)) }));
        if (profile.videosourceconfiguration_->rotate_->degree_.is_initialized())
        {
          rotation->addChild(new QTreeWidgetItem({ "Degree: " + QString::number(*profile.videosourceconfiguration_->rotate_->degree_) }));

        }
        else
        {
          rotation->addChild(new QTreeWidgetItem({ "Degree: 180" })); // Default according to ONVIF

        }
      }
    }
  }

  if (profile.audiosourceconfiguration_.is_initialized())
  {
    if (profile.audiosourceconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* audiosourceconfiguration = new QTreeWidgetItem({ "Audio Source" });
      top->addChild(audiosourceconfiguration);
      audiosourceconfiguration->setExpanded(true);

      audiosourceconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.audiosourceconfiguration_->token_) }));

      if (profile.audiosourceconfiguration_->name_.is_initialized())
      {
        audiosourceconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.audiosourceconfiguration_->name_) }));

      }

      if (profile.audiosourceconfiguration_->sourcetoken_.is_initialized())
      {
        audiosourceconfiguration->addChild(new QTreeWidgetItem({ "Source Token: " + QString::fromStdString(*profile.audiosourceconfiguration_->sourcetoken_) }));

      }
    }
  }

  if (profile.videoencoderconfiguration_.is_initialized())
  {
    if (profile.videoencoderconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* videoencoderconfiguration = new QTreeWidgetItem({ "Video Encoder" });
      top->addChild(videoencoderconfiguration);
      videoencoderconfiguration->setExpanded(true);

      videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.videoencoderconfiguration_->token_) }));

      if (profile.videoencoderconfiguration_->name_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.videoencoderconfiguration_->name_) }));

      }

      if (profile.videoencoderconfiguration_->encoding_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Encoding: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->encoding_)) }));

        if ((*profile.videoencoderconfiguration_->encoding_ == onvif::VIDEOENCODING_MPEG4) && profile.videoencoderconfiguration_->mpeg4_.is_initialized() && profile.videoencoderconfiguration_->mpeg4_->govlength_.is_initialized() && profile.videoencoderconfiguration_->mpeg4_->mpeg4profile_.is_initialized())
        {
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "GovLength: " + QString::number(*profile.videoencoderconfiguration_->mpeg4_->govlength_) }));
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Profile: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->mpeg4_->mpeg4profile_)) }));
        }

        if ((*profile.videoencoderconfiguration_->encoding_ == onvif::VIDEOENCODING_H264) && profile.videoencoderconfiguration_->h264_.is_initialized() && profile.videoencoderconfiguration_->h264_->govlength_.is_initialized() && profile.videoencoderconfiguration_->h264_->h264profile_.is_initialized())
        {
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "GovLength: " + QString::number(*profile.videoencoderconfiguration_->h264_->govlength_) }));
          videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Profile: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->h264_->h264profile_)) }));
        }
      }

      if (profile.videoencoderconfiguration_->resolution_.is_initialized() && profile.videoencoderconfiguration_->resolution_->width_.is_initialized() && profile.videoencoderconfiguration_->resolution_->height_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Resolution: " + QString::number(*profile.videoencoderconfiguration_->resolution_->width_) + "x" + QString::number(*profile.videoencoderconfiguration_->resolution_->height_) }));

      }

      if (profile.videoencoderconfiguration_->quality_.is_initialized())
      {
        videoencoderconfiguration->addChild(new QTreeWidgetItem({ "Quality: " + QString::number(*profile.videoencoderconfiguration_->quality_) }));

      }
    }
  }

  if (profile.audioencoderconfiguration_.is_initialized())
  {
    if (profile.audioencoderconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* audioencoderconfiguration = new QTreeWidgetItem({ "Audio Encoder" });
      top->addChild(audioencoderconfiguration);
      audioencoderconfiguration->setExpanded(true);

      audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.audioencoderconfiguration_->token_) }));

      if (profile.audioencoderconfiguration_->name_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.audioencoderconfiguration_->name_) }));

      }

      if (profile.audioencoderconfiguration_->encoding_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Encoding: " + QString::fromStdString(onvif::ToString(*profile.audioencoderconfiguration_->encoding_)) }));

      }

      if (profile.audioencoderconfiguration_->bitrate_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Bitrate: " + QString::number(*profile.audioencoderconfiguration_->bitrate_) }));

      }

      if (profile.audioencoderconfiguration_->samplerate_.is_initialized())
      {
        audioencoderconfiguration->addChild(new QTreeWidgetItem({ "Samplerate: " + QString::number(*profile.audioencoderconfiguration_->samplerate_) }));

      }
    }
  }

  if (profile.ptzconfiguration_.is_initialized())
  {
    if (profile.ptzconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* ptzconfiguration = new QTreeWidgetItem({ "PTZ" });
      top->addChild(ptzconfiguration);
      ptzconfiguration->setExpanded(true);

      ptzconfiguration->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.ptzconfiguration_->token_) }));

      if (profile.ptzconfiguration_->name_.is_initialized())
      {
        ptzconfiguration->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.ptzconfiguration_->name_) }));

      }

      if (profile.ptzconfiguration_->nodetoken_.is_initialized())
      {
        ptzconfiguration->addChild(new QTreeWidgetItem({ "Node: " + QString::fromStdString(*profile.ptzconfiguration_->nodetoken_) }));

      }
    }
  }

  if (profile.metadataconfiguration_.is_initialized())
  {
    if (profile.metadataconfiguration_->token_.is_initialized())
    {
      QTreeWidgetItem* metadataconfiguration_ = new QTreeWidgetItem({ "PTZ" });
      top->addChild(metadataconfiguration_);
      metadataconfiguration_->setExpanded(true);

      metadataconfiguration_->addChild(new QTreeWidgetItem({ "Token: " + QString::fromStdString(*profile.metadataconfiguration_->token_) }));

      if (profile.metadataconfiguration_->name_.is_initialized())
      {
        metadataconfiguration_->addChild(new QTreeWidgetItem({ "Name: " + QString::fromStdString(*profile.metadataconfiguration_->name_) }));

      }

      if (profile.metadataconfiguration_->compressiontype_.is_initialized())
      {
        metadataconfiguration_->addChild(new QTreeWidgetItem({ "Compression: " + QString::fromStdString(onvif::ToString(*profile.metadataconfiguration_->compressiontype_)) }));

      }
    }
  }
}

void ManageTrackWindow::AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription)
{
  const std::vector<std::string> tmp = mediadescription.ToString();
  if (tmp.empty())
  {
    // Shouldn't be possible so just ignore it I guess?
    return;
  }

  QTreeWidgetItem* top = new QTreeWidgetItem({ QString::fromStdString(tmp.front()) });
  ui_.treedetails->addTopLevelItem(top);
  for (std::vector<std::string>::const_iterator i = tmp.cbegin() + 1; i != tmp.cend(); ++i)
  {
    if (i->empty())
    {

      continue;
    }
    top->addChild(new QTreeWidgetItem({ QString::fromStdString(*i) }));
  }
  top->setExpanded(true);
}

void ManageTrackWindow::GetProfileCallback(const onvif::Profile& profile)
{
  if (!profile.token_.is_initialized())
  {
    ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid Profile token</font><br/>");
    return;
  }

  onvif::StreamSetup streamsetup;
  const monocle::StreamingProtocol streamingprotocol = monocle::EnumValuesStreamingProtocol()[ui_.comboprotocol->currentData().toInt()];
  if (streamingprotocol == monocle::StreamingProtocol::UDPUnicast)
  {
    streamsetup = onvif::StreamSetup(onvif::STREAM_RTPUNICAST, onvif::Transport(onvif::TRANSPORTPROTOCOL_UDP));

  }
  else if (streamingprotocol == monocle::StreamingProtocol::TCPInterleaved)
  {
    streamsetup = onvif::StreamSetup(onvif::STREAM_RTPUNICAST, onvif::Transport(onvif::TRANSPORTPROTOCOL_RTSP));

  }
  else if (streamingprotocol == monocle::StreamingProtocol::UDPMulticast)
  {
    streamsetup = onvif::StreamSetup(onvif::STREAM_RTPMULTICAST, onvif::Transport(onvif::TRANSPORTPROTOCOL_UDP));

  }
  else
  {
    ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid protocol</font><br/>");
    return;
  }

  AddProfile(profile);

  ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Retrieving Stream URI</font><br/>");
  onvifconnection_ = mediaclient_->GetStreamUriCallback(streamsetup, *profile.token_, [this, profile](const onvif::media::GetStreamUriResponse& getstreamuriresponse)
  {
    if (getstreamuriresponse.Error())
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetStreamUri failed: " + QString::fromStdString(getstreamuriresponse.Message()) + "</font><br/>");
      return;
    }

    if (!getstreamuriresponse.mediauri_.is_initialized() || !getstreamuriresponse.mediauri_->uri_.is_initialized())
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetStreamUri Invalid Media URI</font><br/>");
      return;
    }

    network::uri uri;
    try
    {
      uri = network::uri(*getstreamuriresponse.mediauri_->uri_);

    }
    catch (...)
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid RTSP URI: " + QString::fromStdString(*getstreamuriresponse.mediauri_->uri_) + "</font><br/>");
      return;
    }

    if (!uri.has_scheme())
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid RTSP URI: " + QString::fromStdString(*getstreamuriresponse.mediauri_->uri_) + " no schema present</font><br/>");
      return;
    }

    if (!boost::algorithm::iequals(uri.scheme().to_string(), "rtsp"))
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid RTSP URI schema: " + QString::fromStdString(uri.scheme().to_string()) + "</font><br/>");
      return;
    }

    if (!uri.has_host())
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid RTSP URI: " + QString::fromStdString(*getstreamuriresponse.mediauri_->uri_) + " no host present</font><br/>");
      return;
    }

    uint16_t port = 554;
    if (uri.has_port())
    {
      try
      {
        port = boost::lexical_cast<uint16_t>(uri.port().to_string());

      }
      catch (...)
      {
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid RTSP URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
        return;
      }
    }

    RTSPCallback(*getstreamuriresponse.mediauri_->uri_, uri.host().to_string(), port);
  });
}

void ManageTrackWindow::RTSPCallback(const std::string& uri, const std::string& host, const uint16_t port)
{
  rtspclient_ = boost::make_shared< rtsp::Client<ManageTrackWindow> >(shared_from_this(), mutex_, MainWindow::Instance()->GetGUIIOService(), boost::posix_time::seconds(10), boost::posix_time::seconds(60));
  rtspclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), host, port, ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

  ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">RTSP Connecting</font><br/>");
  rtspconnectconnection_ = rtspclient_->Connect([this, uri](const boost::system::error_code err)
  {
    if (err)
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Failed to connect</font><br/>");
      return;
    }

    ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Connected</font><br/><font color=\"green\">Requesting Options</font><br/>");
    rtspconnection_ = rtspclient_->OptionsCallback(uri, [this, uri](const boost::system::error_code err, const rtsp::OptionsResponse& optionsresponse) mutable
    {
      if (err)
      {
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">OPTIONS failed</font><br/>");
        return;
      }

      if (optionsresponse.options_.find(rtsp::headers::REQUESTTYPE_DESCRIBE) == optionsresponse.options_.end())
      {
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Describe option not found</font><br/>");
        return;
      }

      if (optionsresponse.options_.find(rtsp::headers::REQUESTTYPE_SETUP) == optionsresponse.options_.end())
      {
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Setup option not found</font><br/>");
        return;
      }

      if (optionsresponse.options_.find(rtsp::headers::REQUESTTYPE_PLAY) == optionsresponse.options_.end())
      {
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Play option not found</font><br/>");
        return;
      }

      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Options received</font><br/><font color=\"green\">Requesting Describe</font><br/>");
      rtspconnection_ = rtspclient_->DescribeCallback(uri, [this, uri](const boost::system::error_code err, const rtsp::DescribeResponse& describeresponse) mutable
      {
        if (err)
        {
          ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">DESCRIBE failed</font><br/>");
          return;
        }
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Describe received</font><br/>");

        const std::vector<rtsp::sdp::MediaDescription> mediadescriptions = describeresponse.sdp_.GetMediaDescriptions(rtsp::sdp::MEDIATYPE_VIDEO);
        if (mediadescriptions.empty())
        {
          ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Video media description type not found</font><br/>");
          return;
        }

        if (ui_.editsourcetag->text().isEmpty())
        {
          if (mediadescriptions.size() > 1)
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"orange\">Media ambiguity; set the source tag to remove this warning</font><br/>");

          }

          ui_.treedetails->addTopLevelItem(new QTreeWidgetItem({ "RTSP Uri: " + QString::fromStdString(uri) }));

          for (const rtsp::sdp::MediaDescription& mediadescription : mediadescriptions)
          {
            if (!mediadescription.media_.is_initialized())
            {

              continue;
            }

            // Set the auto completer for the source tags
            QStringList sourcetags;
            for (const unsigned int format : mediadescription.media_->formats_)
            {
              sourcetags.push_back("codec=" + QString::number(format));

            }
            sourcetagmodel_->setStringList(sourcetags);

            AddMediaDescription(mediadescription);
          }
        }
        else
        {
          static const boost::regex sourcetagregex("([A-Za-z]+)=([A-Za-z0-9]+)");
          boost::smatch match;
          const std::string sourcetag = ui_.editsourcetag->text().toStdString();
          if (!boost::regex_match(sourcetag, match, sourcetagregex))
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid source tag: " + ui_.editsourcetag->text() + "</font><br/>");
            return;
          }

          if (!boost::algorithm::iequals(match[1].str(), "codec"))
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid source tag: " + ui_.editsourcetag->text() + "</font><br/>");
            return;
          }

          int codec = 0;
          try
          {
            codec = boost::lexical_cast<decltype(codec)>(match[2].str());

          }
          catch (...)
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid source tag: " + ui_.editsourcetag->text() + "</font><br/>");
            return;
          }

          auto mediadescription = std::find_if(mediadescriptions.cbegin(), mediadescriptions.cend(), [codec](const rtsp::sdp::MediaDescription& mediadescription) { return (mediadescription.media_.is_initialized() && utility::Contains(mediadescription.media_->formats_, codec)); });
          if (mediadescription == mediadescriptions.end())
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Source tag: " + ui_.editsourcetag->text() + " not found</font><br/>");
            return;
          }

          // Set the auto completer for the source tags
          QStringList sourcetags;
          for (const rtsp::sdp::MediaDescription& mediadescription : mediadescriptions)
          {
            if (!mediadescription.media_.is_initialized())
            {

              continue;
            }

            for (const unsigned int format : mediadescription.media_->formats_)
            {
              sourcetags.push_back("codec=" + QString::number(format));

            }
          }
          sourcetagmodel_->setStringList(sourcetags);

          ui_.treedetails->addTopLevelItem(new QTreeWidgetItem({ "RTSP Uri: " + QString::fromStdString(uri) }));

          AddMediaDescription(*mediadescription);
        }
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Success</font><br/>");
      });
    });
  }, []()
  {
    // Do nothing

  });
}

void ManageTrackWindow::SetTrack(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t objectdetectortrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken)
{
  std::vector<std::string> receiverparameters;
  if (!ui_.comboprotocol->currentData(STREAMING_PROTOCOL_ROLE).toString().isEmpty())
  {
    receiverparameters.push_back((STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(static_cast<monocle::StreamingProtocol>(ui_.comboprotocol->currentData(STREAMING_PROTOCOL_ROLE).toInt()))).toStdString());

  }

  std::vector<std::string> recordingjobsourcetrackparameters;
  if (!ui_.editsourcetag->text().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((SOURCE_TAG_PARAMETER_NAME + "=" + ui_.editsourcetag->text()).toStdString());

  }

  if (!ui_.editprofiletoken->text().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((PROFILE_TOKEN_PARAMETER_NAME + "=" + ui_.editprofiletoken->text()).toStdString());

  }

  if (!ui_.comborotation->currentData().toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((ROTATION_PARAMETER_NAME + "=" + ui_.comborotation->currentData().toString()).toStdString());

  }

  // Object detector
  std::vector<std::string> objectdetectorsourcetrackparameters;
  if (ui_.checkobjectdetector->isChecked())
  {
    objectdetectorsourcetrackparameters.reserve(33);
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_ACCURACY_PARAMETER_NAME + "=" + QString::number(accuracy_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_HUMANS_ENABLED_PARAMETER_NAME + "=" + QString::number(humans_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_HUMANS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(humanssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BICYCLES_ENABLED_PARAMETER_NAME + "=" + QString::number(bicycles_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BICYCLES_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(bicyclessensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_CARS_ENABLED_PARAMETER_NAME + "=" + QString::number(cars_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_CARS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(carssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_MOTORBIKES_ENABLED_PARAMETER_NAME + "=" + QString::number(motorbikes_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(motorbikessensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BUSES_ENABLED_PARAMETER_NAME + "=" + QString::number(buses_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BUSES_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(busessensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_TRUCKS_ENABLED_PARAMETER_NAME + "=" + QString::number(trucks_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_TRUCKS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(truckssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BACKPACKS_ENABLED_PARAMETER_NAME + "=" + QString::number(backpacks_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(backpackssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_UMBRELLAS_ENABLED_PARAMETER_NAME + "=" + QString::number(umbrellas_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(umbrellassensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_HANDBAGS_ENABLED_PARAMETER_NAME + "=" + QString::number(handbags_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(handbagssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_SUITCASES_ENABLED_PARAMETER_NAME + "=" + QString::number(suitcases_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_SUITCASES_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(suitcasessensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_CATS_ENABLED_PARAMETER_NAME + "=" + QString::number(cats_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_CATS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(catssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_DOGS_ENABLED_PARAMETER_NAME + "=" + QString::number(dogs_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_DOGS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(dogssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_AEROPLANES_ENABLED_PARAMETER_NAME + "=" + QString::number(aeroplanes_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(aeroplanessensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_TRAINS_ENABLED_PARAMETER_NAME + "=" + QString::number(trains_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_TRAINS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(trainssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BOATS_ENABLED_PARAMETER_NAME + "=" + QString::number(boats_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_BOATS_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(boatssensitivity_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_HORSES_ENABLED_PARAMETER_NAME + "=" + QString::number(horses_)).toStdString());
    objectdetectorsourcetrackparameters.push_back((OBJECT_DETECTOR_HORSES_SENSITIVITY_PARAMETER_NAME + "=" + QString::number(horsessensitivity_)).toStdString());
  }

  std::vector<uint64_t> filetokens;
  if (ui_.checkfixedfiles->isChecked())
  {
    filetokens.reserve(files_.size());
    for (const QSharedPointer<File>& file : files_)
    {
      filetokens.push_back(file->GetToken());

    }
  }

  SetEnabled(false);

  if (recording_ && recordingjob_ && recordingjobsource_ && recordingjobsourcetrack_ && recordingtrack_)
  {
    addtrack2connection_ = device_->ChangeTrack2(recordingtoken, recordingtrack_->GetId(), recordingjobtoken, recordingjobsource_->GetToken(), recordingjobsourcetrack_->GetToken(), objectdetectortrackid, objectdetectorrecordingjobsourcetoken, objectdetectorrecordingjobsourcetracktoken, ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), filetokens, ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), receiverparameters, recordingjobsourcetrackparameters, objectdetectorsourcetrackparameters, [this](const std::chrono::steady_clock::duration latency, const monocle::client::CHANGETRACK2RESPONSE& changetrack2response)
    {
      if (changetrack2response.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("ChangeTrack failed: ") + QString::fromStdString(changetrack2response.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        SetEnabled(true);
        return;
      }

      accept();
    });
  }
  else
  {
    addtrack2connection_ = device_->AddTrack2(recordingtoken, recordingjobtoken, monocle::TrackType::Video, ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), filetokens, ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), receiverparameters, recordingjobsourcetrackparameters, objectdetectorsourcetrackparameters, [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDTRACK2RESPONSE& addtrack2response)
    {
      if (addtrack2response.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("ChangeTrack failed: ") + QString::fromStdString(addtrack2response.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        SetEnabled(true);
        return;
      }

      accept();
    });
  }
}

void ManageTrackWindow::SetEnabled(const bool enabled)
{
  ui_.editdescription->setEnabled(enabled);
  ui_.spinflushfrequency->setEnabled(enabled);
  ui_.checkdigitalsigning->setEnabled(enabled);
  ui_.checkencrypt->setEnabled(enabled);
  ui_.checkfixedfiles->setEnabled(enabled);
  ui_.buttonfiles->setEnabled(enabled);
  ui_.edituri->setEnabled(enabled);
  ui_.buttonfindonvifdevice->setEnabled(enabled);

  if (enabled)
  {
    on_edituri_textChanged(QString());
    on_checkfixedfiles_stateChanged(0);
    on_checkobjectdetector_stateChanged(0);
  }
  else
  {
    ui_.editprofiletoken->setEnabled(false);
    ui_.editsourcetag->setEnabled(false);
    ui_.comboprotocol->setEnabled(false);
    ui_.editusername->setEnabled(false);
    ui_.editpassword->setEnabled(false);
    ui_.comborotation->setEnabled(false);
    ui_.checkobjectdetector->setEnabled(false);
    ui_.buttonobjectdetectorsettings->setEnabled(false);
    ui_.buttontest->setEnabled(false);
    ui_.buttonok->setEnabled(false);
  }
}

void ManageTrackWindow::TrackRemoved(const uint32_t id)
{
  if (recordingtrack_ && (recordingtrack_->GetId() == id))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Track has been removed"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    reject();
  }
}

void ManageTrackWindow::on_combodevice_currentIndexChanged(const QString&)
{
  boost::shared_ptr<Device> currentdevice = MainWindow::Instance()->GetDeviceMgr().GetDevice(ui_.combodevice->currentData().toULongLong());
  if (!currentdevice)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find device: ") + QString::number(ui_.combodevice->currentData().toULongLong()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    reject();
    return;
  }
  device_ = currentdevice;
  on_edituri_textChanged(QString()); // Update all the buttons in here
}

void ManageTrackWindow::on_edituri_textChanged(const QString&)
{
  try
  {
    const bool enableobjectdetector = device_->GetNumCudaDevices() ? true : false;
    const network::uri uri(ui_.edituri->text().toStdString());
    if (!uri.has_scheme())
    {
      DisableSource();
      ui_.buttontest->setEnabled(false);
      return;
    }

    if (uri.scheme().compare("rtsp") == 0)
    {
      ui_.editprofiletoken->setEnabled(false);
      ui_.editsourcetag->setEnabled(true);
      ui_.comboprotocol->setEnabled(true);
      ui_.editusername->setEnabled(true);
      ui_.editpassword->setEnabled(true);
      ui_.comborotation->setEnabled(true);
      ui_.checkobjectdetector->setEnabled(enableobjectdetector);
      ui_.buttonobjectdetectorsettings->setEnabled(enableobjectdetector);
      ui_.buttontest->setEnabled(true);
    }
    else if (((uri.scheme().compare("http") == 0) || (uri.scheme().compare("https") == 0)) && uri.has_path() && (uri.path().compare("/onvif/device_service") == 0))
    {
      ui_.editprofiletoken->setEnabled(true);
      ui_.editsourcetag->setEnabled(true);
      ui_.comboprotocol->setEnabled(true);
      ui_.editusername->setEnabled(true);
      ui_.editpassword->setEnabled(true);
      ui_.comborotation->setEnabled(true);
      ui_.checkobjectdetector->setEnabled(enableobjectdetector);
      ui_.buttonobjectdetectorsettings->setEnabled(enableobjectdetector);
      ui_.buttontest->setEnabled(true);
    }
    else
    {
      DisableSource();
      ui_.buttontest->setEnabled(false);
    }
  }
  catch (...)
  {
    DisableSource();
    ui_.buttontest->setEnabled(false);
  }
}

void ManageTrackWindow::on_checkfixedfiles_stateChanged(int)
{
  ui_.buttonfiles->setEnabled(ui_.checkfixedfiles->isChecked());
  
}

void ManageTrackWindow::on_checkobjectdetector_stateChanged(int)
{
  if (device_->GetNumCudaDevices())
  {
    ui_.buttonobjectdetectorsettings->setEnabled(ui_.checkobjectdetector->isChecked());

  }
}

void ManageTrackWindow::on_buttonobjectdetectorsettings_clicked()
{
  ManageTrackObjectDetectorWindow managetrackobjectdetectorwindow(this, accuracy_, humans_, bicycles_, cars_, motorbikes_, buses_, trucks_, backpacks_, umbrellas_, handbags_, suitcases_, cats_, dogs_, aeroplanes_, trains_, boats_, horses_, humanssensitivity_, bicyclessensitivity_, carssensitivity_, motorbikessensitivity_, busessensitivity_, truckssensitivity_, backpackssensitivity_, umbrellassensitivity_, handbagssensitivity_, suitcasessensitivity_, catssensitivity_, dogssensitivity_, aeroplanessensitivity_, trainssensitivity_, boatssensitivity_, horsessensitivity_);
  if (managetrackobjectdetectorwindow.exec() == QDialog::Accepted)
  {
    accuracy_ = managetrackobjectdetectorwindow.accuracy_;
    humans_ = managetrackobjectdetectorwindow.humans_;
    bicycles_ = managetrackobjectdetectorwindow.bicycles_;
    cars_ = managetrackobjectdetectorwindow.cars_;
    motorbikes_ = managetrackobjectdetectorwindow.motorbikes_;
    buses_ = managetrackobjectdetectorwindow.buses_;
    trucks_ = managetrackobjectdetectorwindow.trucks_;
    backpacks_ = managetrackobjectdetectorwindow.backpacks_;
    umbrellas_ = managetrackobjectdetectorwindow.umbrellas_;
    handbags_ = managetrackobjectdetectorwindow.handbags_;
    suitcases_ = managetrackobjectdetectorwindow.suitcases_;
    cats_ = managetrackobjectdetectorwindow.cats_;
    dogs_ = managetrackobjectdetectorwindow.dogs_;
    aeroplanes_ = managetrackobjectdetectorwindow.aeroplanes_;
    trains_ = managetrackobjectdetectorwindow.trains_;
    boats_ = managetrackobjectdetectorwindow.boats_;
    horses_ = managetrackobjectdetectorwindow.horses_;
    humanssensitivity_ = managetrackobjectdetectorwindow.humanssensitivity_;
    bicyclessensitivity_ = managetrackobjectdetectorwindow.bicyclessensitivity_;
    carssensitivity_ = managetrackobjectdetectorwindow.carssensitivity_;
    motorbikessensitivity_ = managetrackobjectdetectorwindow.motorbikessensitivity_;
    busessensitivity_ = managetrackobjectdetectorwindow.busessensitivity_;
    truckssensitivity_ = managetrackobjectdetectorwindow.truckssensitivity_;
    backpackssensitivity_ = managetrackobjectdetectorwindow.backpackssensitivity_;
    umbrellassensitivity_ = managetrackobjectdetectorwindow.umbrellassensitivity_;
    handbagssensitivity_ = managetrackobjectdetectorwindow.handbagssensitivity_;
    suitcasessensitivity_ = managetrackobjectdetectorwindow.suitcasessensitivity_;
    catssensitivity_ = managetrackobjectdetectorwindow.catssensitivity_;
    dogssensitivity_ = managetrackobjectdetectorwindow.dogssensitivity_;
    aeroplanessensitivity_ = managetrackobjectdetectorwindow.aeroplanessensitivity_;
    trainssensitivity_ = managetrackobjectdetectorwindow.trainssensitivity_;
    boatssensitivity_ = managetrackobjectdetectorwindow.boatssensitivity_;
    horsessensitivity_ = managetrackobjectdetectorwindow.horsessensitivity_;
  }
}

void ManageTrackWindow::on_buttonfiles_clicked()
{
  ManageTrackFilesWindow managetrackfileswindow(this, device_, files_);
  if (managetrackfileswindow.exec() == QDialog::Accepted)
  {
    files_ = managetrackfileswindow.files_;

  }
}

void ManageTrackWindow::on_buttonfindonvifdevice_clicked()
{
  ManageTrackFindONVIFDeviceWindow managetrackfindonvifdevicewindow(this, device_, ui_.edituri->text(), ui_.editusername->text(), ui_.editpassword->text());
  if (managetrackfindonvifdevicewindow.exec() == QDialog::Accepted)
  {
    ui_.edituri->setText(managetrackfindonvifdevicewindow.uri_);
    ui_.editusername->setText(managetrackfindonvifdevicewindow.username_);
    ui_.editpassword->setText(managetrackfindonvifdevicewindow.password_);
  }
}

void ManageTrackWindow::on_buttontest_clicked()
{
  profilemodel_->setStringList(QStringList());
  sourcetagmodel_->setStringList(QStringList());

  ui_.labeltestresult->clear();
  ui_.treedetails->clear();

  rtspconnectconnection_.Close();
  rtspconnection_.Close();
  rtspclient_.reset();
  onvifconnection_.Close();
  if (mediaclient_)
  {
    mediaclient_->Destroy();
    mediaclient_.reset();
  }
  if (deviceclient_)
  {
    deviceclient_->Destroy();
    deviceclient_.reset();
  }

  if (ui_.edituri->text().isEmpty())
  {
    ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Empty URI</font><br/>");
    return;
  }

  try
  {
    const network::uri uri(ui_.edituri->text().toStdString());
    if (!uri.has_scheme())
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no scheme present</font><br/>");
      return;
    }

    if (!uri.has_host())
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no host present</font><br/>");
      return;
    }

    if (uri.scheme().compare("rtsp") == 0)
    {
      uint16_t port = 554;
      if (uri.has_port())
      {
        try
        {
          port = boost::lexical_cast<uint16_t>(uri.port().to_string());

        }
        catch (...)
        {
          ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
          return;
        }
      }

      RTSPCallback(ui_.edituri->text().toStdString(), uri.host().to_string(), port);
    }
    else if (((uri.scheme().compare("http") == 0) || (uri.scheme().compare("https") == 0)) && uri.has_path() && (uri.path().compare("/onvif/device_service") == 0))
    {
      uint16_t port = 80;
      if (uri.has_port())
      {
        try
        {
          port = boost::lexical_cast<uint16_t>(uri.port().to_string());

        }
        catch (...)
        {
          ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
          return;
        }
      }

      deviceclient_ = boost::make_shared<onvif::device::DeviceClient>(mutex_);
      mediaclient_ = boost::make_shared<onvif::media::MediaClient>(mutex_);
      if (deviceclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), 1, false, true))
      {
        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Failed to initialise ONVIF Device Client</font><br/>");
        return;
      }

      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Retrieving System Date and Time</font><br/>");
      onvifconnection_ = deviceclient_->GetSystemDateAndTimeCallback([this](const onvif::device::GetSystemDateAndTimeResponse& getsystemdateandtimeresponse)
      {
        if (getsystemdateandtimeresponse.Message() == onvif::UNABLETOCONNECT)
        {
          ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetSystemDateAndTime failed: " + QString::fromStdString(getsystemdateandtimeresponse.Message()) + "</font><br/>");
          return;
        }
        else if (getsystemdateandtimeresponse.Error())
        {
          ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"orange\">GetSystemDateAndTime failed: " + QString::fromStdString(getsystemdateandtimeresponse.Message()) + "</font><br/>");

        }

        if (getsystemdateandtimeresponse.systemdatetime_.is_initialized())
        {
          deviceclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
          mediaclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
        }

        ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Retrieving Capabilities</font><br/>");
        onvifconnection_ = deviceclient_->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY::CAPABILITYCATEGORY_ALL, [this](const onvif::device::GetCapabilitiesResponse& getcapabilitiesresponse)
        {
          if (getcapabilitiesresponse.Error())
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetCapabilities failed: " + QString::fromStdString(getcapabilitiesresponse.Message()) + "</font><br/>");
            return;
          }

          if (!getcapabilitiesresponse.capabilities_.is_initialized())
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetCapabilities invalid response no capabilities found</font><br/>");
            return;
          }

          if (!getcapabilitiesresponse.capabilities_->media_.is_initialized())
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetCapabilities invalid response no media capabilities</font><br/>");
            return;
          }

          const std::string mediaxaddr = getcapabilitiesresponse.capabilities_->media_->GetXAddr();
          if (mediaxaddr.empty())
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetCapabilities invalid response no media XADDR</font><br/>");
            return;
          }

          if (mediaclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), mediaxaddr, deviceclient_->GetUsername(), deviceclient_->GetPassword(), 1, false, true))
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Failed to initialise Media client</font><br/>");
            return;
          }

          if (ui_.editprofiletoken->text().isEmpty())
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Retrieving Profiles</font><br/>");
            onvifconnection_ = mediaclient_->GetProfilesCallback([this](const onvif::media::GetProfilesResponse& getprofilesresponse)
            {
              if (getprofilesresponse.Error())
              {
                ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetProfiles failed: " + QString::fromStdString(getprofilesresponse.Message()) + "</font><br/>");
                return;
              }

              if (getprofilesresponse.profiles_.empty())
              {
                ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">No profiles found</font><br/>");
                return;
              }

              // Add the profile tokens to the auto completer for the edit profile
              QStringList profiletokens;
              for (const onvif::Profile& profile : getprofilesresponse.profiles_)
              {
                if (profile.token_.is_initialized())
                {
                  profiletokens.push_back(QString::fromStdString(*profile.token_));

                }
              }
              profilemodel_->setStringList(profiletokens);

              GetProfileCallback(getprofilesresponse.profiles_.front());
            });
          }
          else
          {
            ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"green\">Retrieving Profile</font><br/>");
            onvifconnection_ = mediaclient_->GetProfileCallback(ui_.editprofiletoken->text().toStdString(), [this](const onvif::media::GetProfileResponse& getprofileresponse)
            {
              if (getprofileresponse.Error())
              {
                ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">GetProfile failed: " + QString::fromStdString(getprofileresponse.Message()) + "</font><br/>");
                return;
              }

              if (!getprofileresponse.profile_.is_initialized())
              {
                ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Profile not initialised</font><br/>");
                return;
              }

              // Add the profile token to the auto completer for the edit profile
              QStringList profiletokens = profilemodel_->stringList();
              if (getprofileresponse.profile_->token_.is_initialized())
              {
                profiletokens.push_back(QString::fromStdString(*getprofileresponse.profile_->token_));

              }
              profilemodel_->setStringList(profiletokens);

              GetProfileCallback(*getprofileresponse.profile_);
            });
          }
        });
      });
    }
    else
    {
      ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " invalid scheme</font><br/>");

    }
  }
  catch (...)
  {
    ui_.labeltestresult->setText(ui_.labeltestresult->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " invalid uri</font><br/>");

  }
}

void ManageTrackWindow::on_buttonok_clicked()
{
  if (ui_.editdescription->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Description empty, please provide a track description"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  // Make sure the URI looks reasonable
  const std::string mediauri = ui_.edituri->text().toStdString();
  if (mediauri.size()) // Empty URI is satisfactory, it just remains idle on the server
  {
    const network::uri uri(mediauri);
    if (!uri.has_scheme())
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid URI: ") + ui_.edituri->text() + " no scheme present", QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    if (!uri.has_host())
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid URI: ") + ui_.edituri->text() + " no host present", QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    if (uri.scheme().compare("rtsp") == 0)
    {
      // Looks good

    }
    else if ((uri.scheme().compare("http") == 0) || (uri.scheme().compare("https") == 0))
    {
      if (uri.has_path() && (uri.path().compare("/onvif/device_service/") == 0))
      {
        if (QMessageBox::question(this, tr("Warning"), tr("The uri aooears invalid(trailing slash '/'), are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        {

          return;
        }
      }
      else if (uri.has_path() && (uri.path().compare("/onvif/device_service") == 0))
      {
        // Looks good

      }
      else
      {
        if (QMessageBox::question(this, tr("Warning"), tr("The uri looks invalid, are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        {

          return;
        }
      }
    }
    else
    {
      if (QMessageBox::question(this, tr("Warning"), tr("The uri looks invalid, are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
      {

        return;
      }
    }
  }

  QSharedPointer<RecordingJob> job = GetJob();
  if (job)
  {
    uint32_t objectdetectortrackid = 0;
    uint64_t objectdetectorrecordingjobsourcetoken = 0;
    uint64_t objectdetectorrecordingjobsourcetracktoken = 0;
    if (recordingjobsource_ && recordingjobsourcetrack_ && recordingtrack_)
    {
      const std::vector< QSharedPointer<RecordingJobSource> > sources = job->GetObjectDetectors(recordingtrack_->GetId());
      if (sources.size())
      {
        std::vector< QSharedPointer<RecordingJobSourceTrack> > sourcetracks = sources.front()->GetTracks(monocle::TrackType::ObjectDetector);
        if (sourcetracks.size())
        {
          if (sourcetracks.front()->GetTrack())
          {
            objectdetectortrackid = sourcetracks.front()->GetTrack()->GetId();
            objectdetectorrecordingjobsourcetoken = sources.front()->GetToken();
            objectdetectorrecordingjobsourcetracktoken = sourcetracks.front()->GetToken();
          }
        }
      }
    }

    SetTrack(recording_->GetToken(), job->GetToken(), objectdetectortrackid, objectdetectorrecordingjobsourcetoken, objectdetectorrecordingjobsourcetracktoken);
  }
  else
  {
    SetEnabled(false);

    if (recording_)
    {
      addtrack2connection_ = device_->AddRecordingJob(recording_->GetToken(), "Job", true, 0, std::vector<monocle::ADDRECORDINGJOBSOURCE>(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDRECORDINGJOBRESPONSE& addrecordingjobresponse)
      {
        if (addrecordingjobresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          QMessageBox(QMessageBox::Warning, tr("Error"), tr("ChangeTrack failed: ") + QString::fromStdString(addrecordingjobresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
          SetEnabled(true);
          return;
        }

        uint32_t objectdetectortrackid = 0;
        const std::vector< QSharedPointer<RecordingTrack> > objectdetectortracks = recording_->GetObjectDetectorTracks();
        if (objectdetectortracks.size())
        {
          objectdetectortrackid = objectdetectortracks.front()->GetId();

        }

        SetTrack(recording_->GetToken(), addrecordingjobresponse.recordingjobtoken_, objectdetectortrackid, 0, 0);
      });
    }
    else
    {
      SetTrack(0, 0, 0, 0, 0); // Need to create the recording along with everything else

    }
  }
}

}

QT_END_NAMESPACE
