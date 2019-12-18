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

ManageTrackWindow::ManageTrackWindow(QWidget* parent, const monocle::TrackType tracktype, boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording) :
  QDialog(parent),
  tracktype_(tracktype),
  device_(device),
  recording_(recording),
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

  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::TCPInterleaved)), static_cast<int>(monocle::StreamingProtocol::TCPInterleaved));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPUnicast)), static_cast<int>(monocle::StreamingProtocol::UDPUnicast));
  ui_.comboprotocol->addItem(QString(monocle::EnumNameStreamingProtocol(monocle::StreamingProtocol::UDPMulticast)), static_cast<int>(monocle::StreamingProtocol::UDPMulticast));

  ui_.comborotation->addItem("0", "0");
  ui_.comborotation->addItem("90", "90");
  ui_.comborotation->addItem("180", "180");
  ui_.comborotation->addItem("270", "270");
  
  if (tracktype == monocle::TrackType::Video)
  {
    //TODO this shit is ugly here... would be nice to sort out...
    QSharedPointer<RecordingJob> job = recording_->GetActiveJob();
    if (!job)
    {
      if (recording_->GetJobs().size())
      {
        job = recording_->GetJobs().front();

      }
    }

    if (job)
    {
      for (const QSharedPointer<RecordingJobSource>& source : job->GetSources())
      {
        for (const QSharedPointer<RecordingJobSourceTrack>& sourcetrack : source->GetTracks())
        {
          QSharedPointer<RecordingTrack> track = sourcetrack->GetTrack();
          if (track && (track->GetTrackType() == monocle::TrackType::ObjectDetector))
          {
            ui_.checkobjectdetector->setChecked(true);
            sourcetrack->GetParameters();//TODO set all the object detector details(anything not mentioned should be disabled imo?)
            //TODO fill it in and deal with it

          }
        }
      }
    }
  }
  else if (tracktype == monocle::TrackType::Metadata)
  {
    ui_.checkobjectdetector->setVisible(false);
    ui_.buttonobjectdetectorsettings->setVisible(false);
  }
  else
  {
    //TODO error and reject in 1ms

  }

  //TODO select and fill in the things if there is a stuff editing to put in

  on_checkfixedfiles_stateChanged(0);
  on_checkobjectdetector_stateChanged(0);
}

ManageTrackWindow::~ManageTrackWindow()
{
  addtrack2connection_.Close();

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
  QSharedPointer<RecordingJob> activerecordingjob = recording_->GetActiveJob();
  uint64_t recordingjobtoken = 0;
  if (activerecordingjob)
  {
    recordingjobtoken = activerecordingjob->GetToken();

  }
  else
  {
    int i = 0;//TODO remove
    //TODO now just find the highest priority one...
      //TODO if we don't find one then we need to create a job

  }

  std::vector<std::string> receiverparameters;
  if (!ui_.comboprotocol->currentData(STREAMING_PROTOCOL_ROLE).toString().isEmpty())
  {
    receiverparameters.push_back((STREAMING_PROTOCOL_PARAMETER_NAME + "=" + ui_.comboprotocol->currentData(STREAMING_PROTOCOL_ROLE).toString()).toStdString());

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

  //TODO files needs to be sorted
  addtrack2connection_ = device_->AddTrack2(recording_->GetToken(), recordingjobtoken, tracktype_, ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), {}, ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), receiverparameters, recordingjobsourcetrackparameters, objectdetectorsourcetrackparameters, [this](const std::chrono::steady_clock::duration latency, const monocle::client::ADDTRACK2RESPONSE& addtrack2response)
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

QT_END_NAMESPACE
