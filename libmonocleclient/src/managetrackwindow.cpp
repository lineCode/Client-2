// managetrackwindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackwindow.h"

#include <monocleprotocol/streamingprotocol_generated.h>
#include <monocleprotocol/tracktype_generated.h>
#include <network/uri.hpp>
#include <QString>

#include "monocleclient/device.h"
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
//TODO I don't think we pass in the tracktype anymore, because metadata will be a checkbox within
  //TODO so we just pass in the recordingjobsourcetrack and track
    //TODO maybe we just pass in the "recordingjobsource", this way if there is 
  //TODO how do we link the metadata tracks in like this?
    //TODO is it by ip address or hostname?
      //TODO this doesn't work because we may use the same ip address or hostname multiple times...
    //TODO is it by recordingjobsource/receiver?
      //TODO I think this is the ONLY way?
ManageTrackWindow::ManageTrackWindow(QWidget* parent, boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& recordingtrack) :
  QDialog(parent),
  device_(device),
  recording_(recording),
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
  //TODO connect to the track and rjst getting removed so that we can close this dialog if we are editing

  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::TCPInterleaved)), static_cast<int>(monocle::StreamingProtocol::TCPInterleaved));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPUnicast)), static_cast<int>(monocle::StreamingProtocol::UDPUnicast));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPMulticast)), static_cast<int>(monocle::StreamingProtocol::UDPMulticast));

  ui_.comborotation->addItem("0", "0");
  ui_.comborotation->addItem("90", "90");
  ui_.comborotation->addItem("180", "180");
  ui_.comborotation->addItem("270", "270");

  // Find a job to get started with, it may not always be available
  QSharedPointer<RecordingJob> job = GetJob();

  if (recordingjobsourcetrack_ && recordingtrack_)
  {
    //TODO is this correct?
    if (job)
    {
      for (const QSharedPointer<RecordingJobSource>& source : job->GetSources())
      {
        // Check to see whether this source is an object detector pointing to the original track
        QSharedPointer<Receiver> receiver = device_->GetReceiver(source->GetReceiverToken());
        if (!receiver)
        {

          continue;
        }

        try
        {
          network::uri uri(receiver->GetMediaUri().toStdString());
          if (!uri.has_scheme() || !uri.has_path())
          {

            continue;
          }

          if (uri.scheme().compare("objectdetector"))
          {

            continue;
          }
          
          if (uri.path().compare(std::to_string(recordingtrack_->GetId())))//TODO not sure this works, be careful?
          {

            continue;
          }
        }
        catch (...)
        {

          continue;
        }

        for (const QSharedPointer<RecordingJobSourceTrack>& sourcetrack : source->GetTracks())
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

  //TODO select and fill in the things if there is a stuff editing to put in

  if (device_->GetNumCudaDevices() == 0)
  {
    ui_.checkobjectdetector->setDisabled(true);
    ui_.checkobjectdetector->setStatusTip("No CUDA devices found on server");//TODO does this work
    ui_.buttonobjectdetectorsettings->setDisabled(true);
    ui_.buttonobjectdetectorsettings->setStatusTip("No CUDA devices found on server");//TODO does this work
  }

  on_edituri_textChanged(QString());
  on_checkfixedfiles_stateChanged(0);
  on_checkobjectdetector_stateChanged(0);
}

ManageTrackWindow::~ManageTrackWindow()
{
  addtrack2connection_.Close();

}

QSharedPointer<RecordingJob> ManageTrackWindow::GetJob() const
{
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

void ManageTrackWindow::on_edituri_textChanged(const QString& text)
{
  try
  {
    const network::uri uri(text.toStdString());
    if (!uri.has_scheme())
    {
      DisableSource();
      //TODO disable test
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
      ui_.checkobjectdetector->setEnabled(true);
      ui_.buttonobjectdetectorsettings->setEnabled(true);
      //TODO enable test
    }
    else if ((uri.scheme().compare("http") == 0) && uri.has_path() && (uri.path().compare("/onvif/device_service") == 0))//TODO check it works
    {
      ui_.editprofiletoken->setEnabled(true);
      ui_.editsourcetag->setEnabled(true);
      ui_.comboprotocol->setEnabled(true);
      ui_.editusername->setEnabled(true);
      ui_.editpassword->setEnabled(true);
      ui_.comborotation->setEnabled(true);
      ui_.checkobjectdetector->setEnabled(true);
      ui_.buttonobjectdetectorsettings->setEnabled(true);
      //TODO enable test
    }
    else
    {
      DisableSource();
      //TODO disable test

    }
  }
  catch (...)
  {
    DisableSource();
    //TODO disable test

  }
}

void ManageTrackWindow::on_checkfixedfiles_stateChanged(int)
{
  ui_.buttonfiles->setEnabled(ui_.checkfixedfiles->isChecked());

}

void ManageTrackWindow::on_checkobjectdetector_stateChanged(int)
{
  ui_.buttonobjectdetectorsettings->setEnabled(ui_.checkobjectdetector->isChecked());

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

void ManageTrackWindow::on_buttonfindonvifdevice_clicked()
{
  //TODO open the find a onvif device window...
}
//TODO on find onvif device
void ManageTrackWindow::on_buttonok_clicked()
{

  //TODO make sure uri is valid
  //TODO check uri looks like rtsp or http://1.2.3.4:99/onvif/device_service

  // Find or create a recording job
  QSharedPointer<RecordingJob> job = GetJob();
  uint64_t recordingjobtoken = 0;
  if (job)
  {
    recordingjobtoken = job->GetToken();

  }
  else
  {
    //TODO remove the job from sqlite and try this
    int i = 0;//TODO remove
    //TODO now just find the highest priority one...
      //TODO if we don't find one then we need to create a job

  }

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

  if (!ui_.comborotation->currentData(ROTATION_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((ROTATION_PARAMETER_NAME + "=" + ui_.comborotation->currentData(ROTATION_ROLE).toString()).toStdString());

  }

  // Object detector
  const bool enableobjectdetector = ui_.checkobjectdetector->isChecked();
  std::vector<std::string> objectdetectorsourcetrackparameters;
  if (enableobjectdetector)
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

  //TODO disable buttons

  //TODO files needs to be sorted
  if (recordingjobsourcetrack_ && recordingtrack_)
  {
    //TODO edit

  }
  else
  {
    addtrack2connection_ = device_->AddTrack2(recording_->GetToken(), recordingjobtoken, monocle::TrackType::Video, ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), {}, ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), receiverparameters, recordingjobsourcetrackparameters, objectdetectorsourcetrackparameters, [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDTRACK2RESPONSE& addtrack2response)
    {

      //TODO enable buttons... again
        //TODO just create a method for tidy up all the enabled/disabled

      if (addtrack2response.GetErrorCode() != monocle::ErrorCode::Success)
      {
        //TODO QMessageBox
        return;
      }

      accept();
    });
  }
}

}

QT_END_NAMESPACE
