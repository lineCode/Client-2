// managerecordingjobsourceobjectdetectorwindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingjobsourceobjectdetectorwindow.h"

#include <monocleprotocol/streamingprotocol_generated.h>
#include <network/uri.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>
#include <rtsp/client/client.hpp>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingJobSourceObjectDetectorWindow::ManageRecordingJobSourceObjectDetectorWindow(QWidget* parent,
                                                                                           const boost::shared_ptr<Device>& device,
                                                                                           const QSharedPointer<client::Recording>& recording,
                                                                                           const QSharedPointer<client::RecordingJob>& recordingjob,
                                                                                           const bool editing,
                                                                                           const bool enabled,
                                                                                           const uint32_t metadatatrackid,
                                                                                           const QString& mediauri,
                                                                                           const int accuracy,
                                                                                           const bool humans,
                                                                                           const bool bicycles,
                                                                                           const bool cars,
                                                                                           const bool motorbikes,
                                                                                           const bool buses,
                                                                                           const bool trucks,
                                                                                           const bool backpacks,
                                                                                           const bool umbrellas,
                                                                                           const bool handbags,
                                                                                           const bool suitcases,
                                                                                           const bool cats,
                                                                                           const bool dogs,
                                                                                           const bool aeroplanes,
                                                                                           const bool trains,
                                                                                           const bool boats,
                                                                                           const bool horses,
                                                                                           const double humanssensitivity,
                                                                                           const double bicyclessensitivity,
                                                                                           const double carssensitivity,
                                                                                           const double motorbikessensitivity,
                                                                                           const double busessensitivity,
                                                                                           const double truckssensitivity,
                                                                                           const double backpackssensitivity,
                                                                                           const double umbrellassensitivity,
                                                                                           const double handbagssensitivity,
                                                                                           const double suitcasessensitivity,
                                                                                           const double catssensitivity,
                                                                                           const double dogssensitivity,
                                                                                           const double aeroplanessensitivity,
                                                                                           const double trainssensitivity,
                                                                                           const double boatssensitivity,
                                                                                           const double horsessensitivity,
                                                                                           const std::vector<uint32_t>& reservedtrackids) :
  QDialog(parent),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob),
  enabled_(enabled),
  metadatatrackid_(metadatatrackid),
  uri_(mediauri),
  accuracy_(accuracy),
  humans_(humans),
  bicycles_(bicycles),
  cars_(cars),
  motorbikes_(motorbikes),
  buses_(buses),
  trucks_(trucks),
  backpacks_(backpacks),
  umbrellas_(umbrellas),
  handbags_(handbags),
  suitcases_(suitcases),
  cats_(cats),
  dogs_(dogs),
  aeroplanes_(aeroplanes),
  trains_(trains),
  boats_(boats),
  horses_(horses),
  humanssensitivity_(humanssensitivity),
  bicyclessensitivity_(bicyclessensitivity),
  carssensitivity_(carssensitivity),
  motorbikessensitivity_(motorbikessensitivity),
  busessensitivity_(busessensitivity),
  truckssensitivity_(truckssensitivity),
  backpackssensitivity_(backpackssensitivity),
  umbrellassensitivity_(umbrellassensitivity),
  handbagssensitivity_(handbagssensitivity),
  suitcasessensitivity_(suitcasessensitivity),
  catssensitivity_(catssensitivity),
  dogssensitivity_(dogssensitivity),
  aeroplanessensitivity_(aeroplanessensitivity),
  trainssensitivity_(trainssensitivity),
  boatssensitivity_(boatssensitivity),
  horsessensitivity_(horsessensitivity)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  connect(device_.get(), &Device::SignalRecordingJobSourceTrackRemoved, this, &ManageRecordingJobSourceObjectDetectorWindow::RecordingJobSourceTrackRemoved, Qt::QueuedConnection);
  connect(recording_.data(), &Recording::TrackChanged, this, &ManageRecordingJobSourceObjectDetectorWindow::TrackChanged, Qt::QueuedConnection);
  connect(recording_.data(), &Recording::TrackRemoved, this, &ManageRecordingJobSourceObjectDetectorWindow::TrackRemoved, Qt::QueuedConnection);

  ui_.checkenabled->setChecked(enabled);

  // Video
  for (const QSharedPointer<client::RecordingTrack>& videotrack : recording_->GetVideoTracks())
  {
    ui_.combovideotrack->addItem(videotrack->GetDescription(), videotrack->GetId());

  }

  if (ui_.combovideotrack->count() == 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No video tracks available for this recording, please add a video track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    QTimer::singleShot(1, this, [this]() { reject(); });
  }

  // Metadata
  if (editing)
  {
    setWindowTitle("Edit Object Detector Source");

    const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(metadatatrackid_);
    if (track == nullptr)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(metadatatrackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    
    }
    else
    {
      if (track->GetTrackType() != monocle::TrackType::Metadata)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid track type: ") + QString::number(metadatatrackid_) + " " + monocle::EnumNameTrackType(track->GetTrackType()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();

      }
      else
      {
        ui_.combometadatatrack->addItem(track->GetDescription(), track->GetId());

      }
    }
  }
  else
  {
    connect(recording_.data(), &Recording::TrackAdded, this, &ManageRecordingJobSourceObjectDetectorWindow::TrackAdded, Qt::QueuedConnection); // We don't want to add new tracks if we are editing

    for (const QSharedPointer<client::RecordingTrack>& metadatatrack : recording_->GetMetadataTracks())
    {
      if (metadatatrack->GetId() != metadatatrackid_) // We can select our current track
      {
        if (std::find_if(reservedtrackids.cbegin(), reservedtrackids.cend(), [id = metadatatrack->GetId()](const uint64_t trackid){ return (trackid == id); }) != reservedtrackids.cend()) // Check to see if this is already in use by another source
        {
          
          continue;
        }
      }
      ui_.combometadatatrack->addItem(metadatatrack->GetDescription(), metadatatrack->GetId());
    }
  }

  ui_.checkhumans->setChecked(humans);
  ui_.checkbicycles->setChecked(bicycles);
  ui_.checkcars->setChecked(cars);
  ui_.checkmotorbikes->setChecked(motorbikes);
  ui_.checkbuses->setChecked(buses);
  ui_.checktrucks->setChecked(trucks);
  ui_.checkbackpacks->setChecked(backpacks);
  ui_.checkumbrellas->setChecked(umbrellas);
  ui_.checkhandbags->setChecked(handbags);
  ui_.checksuitcases->setChecked(suitcases);
  ui_.checkcats->setChecked(cats);
  ui_.checkdogs->setChecked(dogs);
  ui_.checkaeroplanes->setChecked(aeroplanes);
  ui_.checktrains->setChecked(trains);
  ui_.checkboats->setChecked(boats);
  ui_.checkhorses->setChecked(horses);
  ui_.sliderhumans->setValue((1.0 - humanssensitivity) * static_cast<double>(ui_.sliderhumans->maximum()));
  ui_.sliderbicycles->setValue((1.0 - bicyclessensitivity) * static_cast<double>(ui_.sliderbicycles->maximum()));
  ui_.slidercars->setValue((1.0 - carssensitivity) * static_cast<double>(ui_.slidercars->maximum()));
  ui_.slidermotorbikes->setValue((1.0 - motorbikessensitivity) * static_cast<double>(ui_.slidermotorbikes->maximum()));
  ui_.sliderbuses->setValue((1.0 - busessensitivity) * static_cast<double>(ui_.sliderbuses->maximum()));
  ui_.slidertrucks->setValue((1.0 - truckssensitivity) * static_cast<double>(ui_.slidertrucks->maximum()));
  ui_.sliderbackpacks->setValue((1.0 - backpackssensitivity) * static_cast<double>(ui_.sliderbackpacks->maximum()));
  ui_.sliderumbrellas->setValue((1.0 - umbrellassensitivity) * static_cast<double>(ui_.sliderumbrellas->maximum()));
  ui_.sliderhandbags->setValue((1.0 - handbagssensitivity) * static_cast<double>(ui_.sliderhandbags->maximum()));
  ui_.slidersuitcases->setValue((1.0 - suitcasessensitivity) * static_cast<double>(ui_.slidersuitcases->maximum()));
  ui_.slidercats->setValue((1.0 - catssensitivity) * static_cast<double>(ui_.slidercats->maximum()));
  ui_.sliderdogs->setValue((1.0 - dogssensitivity) * static_cast<double>(ui_.sliderdogs->maximum()));
  ui_.slideraeroplanes->setValue((1.0 - aeroplanessensitivity) * static_cast<double>(ui_.slideraeroplanes->maximum()));
  ui_.slidertrains->setValue((1.0 - trainssensitivity) * static_cast<double>(ui_.slidertrains->maximum()));
  ui_.sliderboats->setValue((1.0 - boatssensitivity) * static_cast<double>(ui_.sliderboats->maximum()));
  ui_.sliderhorses->setValue((1.0 - horsessensitivity) * static_cast<double>(ui_.sliderhorses->maximum()));

  // Select current video track
  if (!uri_.isEmpty())
  {
    try
    {
      const network::uri uri(uri_.toStdString());
      const uint32_t videotrackid = boost::lexical_cast<uint32_t>(uri.host().to_string());
      for (int i = 0; i < ui_.combovideotrack->count(); ++i)
      {
        if (ui_.combovideotrack->itemData(i) == videotrackid)
        {
          ui_.combovideotrack->setCurrentIndex(i);
          break;
        }
      }
    }
    catch (...)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to parse video track id: ") + uri_, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();

    }
  }

  // Select current metadata track
  for (int i = 0; i < ui_.combometadatatrack->count(); ++i)
  {
    if (ui_.combometadatatrack->itemData(i) == metadatatrackid_)
    {
      ui_.combometadatatrack->setCurrentIndex(i);
      break;
    }
  }

  // Select current accuracy
  accuracy_ = std::min(ui_.comboaccuracy->count() - 1, accuracy_);
  accuracy_ = std::max(0, accuracy);
  ui_.comboaccuracy->setCurrentIndex(accuracy_);
}

ManageRecordingJobSourceObjectDetectorWindow::~ManageRecordingJobSourceObjectDetectorWindow()
{

}

void ManageRecordingJobSourceObjectDetectorWindow::RecordingJobSourceTrackRemoved(const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token)
{
  QMessageBox(QMessageBox::Warning, tr("Error"), tr("Recording Job Source has been removed"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
  reject();
}

void ManageRecordingJobSourceObjectDetectorWindow::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  if (track->GetTrackType() == monocle::TrackType::Video)
  {
    ui_.combovideotrack->addItem(track->GetDescription(), track->GetId());

  }
  else if (track->GetTrackType() == monocle::TrackType::Metadata)
  {
    ui_.combometadatatrack->addItem(track->GetDescription(), track->GetId());

  }
}

void ManageRecordingJobSourceObjectDetectorWindow::TrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  if (track->GetTrackType() == monocle::TrackType::Video)
  {
    const int row = ui_.combovideotrack->findData(track->GetId());
    if (row == -1)
    {
    
      return;
    }
    ui_.combovideotrack->setItemText(0, track->GetDescription());
  }
  else if (track->GetTrackType() == monocle::TrackType::Metadata)
  {
    const int row = ui_.combometadatatrack->findData(track->GetId());
    if (row == -1)
    {

      return;
    }
    ui_.combometadatatrack->setItemText(0, track->GetDescription());
  }
}

void ManageRecordingJobSourceObjectDetectorWindow::TrackRemoved(const uint32_t id)
{
  int row = ui_.combovideotrack->findData(id);
  if (row != -1)
  {
    ui_.combovideotrack->removeItem(row);
  
  }

  row = ui_.combometadatatrack->findData(id);
  if (row != -1)
  {
    ui_.combometadatatrack->removeItem(row);

  }
}

void ManageRecordingJobSourceObjectDetectorWindow::on_buttonok_clicked()
{
  if (ui_.combometadatatrack->count() == 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No metadata tracks available for this recording, please add a metadata track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (ui_.combovideotrack->count() == 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No video tracks available for this recording, please add a video track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  enabled_ = ui_.checkenabled->isChecked();
  metadatatrackid_ = ui_.combometadatatrack->currentData().toUInt();
  accuracy_ = ui_.comboaccuracy->currentIndex();
  uri_ = QString("objectdetector://") + QString::number(ui_.combovideotrack->currentData().toUInt());
  humans_ = ui_.checkhumans->isChecked();
  bicycles_ = ui_.checkbicycles->isChecked();
  cars_ = ui_.checkcars->isChecked();
  motorbikes_ = ui_.checkmotorbikes->isChecked();
  buses_ = ui_.checkbuses->isChecked();
  trucks_ = ui_.checktrucks->isChecked();
  backpacks_ = ui_.checkbackpacks->isChecked();
  umbrellas_ = ui_.checkumbrellas->isChecked();
  handbags_ = ui_.checkhandbags->isChecked();
  suitcases_ = ui_.checksuitcases->isChecked();
  cats_ = ui_.checkcats->isChecked();
  dogs_ = ui_.checkdogs->isChecked();
  aeroplanes_ = ui_.checkaeroplanes->isChecked();
  trains_ = ui_.checktrains->isChecked();
  boats_ = ui_.checkboats->isChecked();
  horses_ = ui_.checkhorses->isChecked();
  humanssensitivity_ = (static_cast<double>(ui_.sliderhumans->maximum() - ui_.sliderhumans->value()) / static_cast<double>(ui_.sliderhumans->maximum()));
  bicyclessensitivity_ = (static_cast<double>(ui_.sliderbicycles->maximum() - ui_.sliderbicycles->value()) / static_cast<double>(ui_.sliderbicycles->maximum()));
  carssensitivity_ = (static_cast<double>(ui_.slidercars->maximum() - ui_.slidercars->value()) / static_cast<double>(ui_.slidercars->maximum()));
  motorbikessensitivity_ = (static_cast<double>(ui_.slidermotorbikes->maximum() - ui_.slidermotorbikes->value()) / static_cast<double>(ui_.slidermotorbikes->maximum()));
  busessensitivity_ = (static_cast<double>(ui_.sliderbuses->maximum() - ui_.sliderbuses->value()) / static_cast<double>(ui_.sliderbuses->maximum()));
  truckssensitivity_ = (static_cast<double>(ui_.slidertrucks->maximum() - ui_.slidertrucks->value()) / static_cast<double>(ui_.slidertrucks->maximum()));
  backpackssensitivity_ = (static_cast<double>(ui_.sliderbackpacks->maximum() - ui_.sliderbackpacks->value()) / static_cast<double>(ui_.sliderbackpacks->maximum()));
  umbrellassensitivity_ = (static_cast<double>(ui_.sliderumbrellas->maximum() - ui_.sliderumbrellas->value()) / static_cast<double>(ui_.sliderumbrellas->maximum()));
  handbagssensitivity_ = (static_cast<double>(ui_.sliderhandbags->maximum() - ui_.sliderhandbags->value()) / static_cast<double>(ui_.sliderhandbags->maximum()));
  suitcasessensitivity_ = (static_cast<double>(ui_.slidersuitcases->maximum() - ui_.slidersuitcases->value()) / static_cast<double>(ui_.slidersuitcases->maximum()));
  catssensitivity_ = (static_cast<double>(ui_.slidercats->maximum() - ui_.slidercats->value()) / static_cast<double>(ui_.slidercats->maximum()));
  dogssensitivity_ = (static_cast<double>(ui_.sliderdogs->maximum() - ui_.sliderdogs->value()) / static_cast<double>(ui_.sliderdogs->maximum()));
  aeroplanessensitivity_ = (static_cast<double>(ui_.slideraeroplanes->maximum() - ui_.slideraeroplanes->value()) / static_cast<double>(ui_.slideraeroplanes->maximum()));
  trainssensitivity_ = (static_cast<double>(ui_.slidertrains->maximum() - ui_.slidertrains->value()) / static_cast<double>(ui_.slidertrains->maximum()));
  boatssensitivity_ = (static_cast<double>(ui_.sliderboats->maximum() - ui_.sliderboats->value()) / static_cast<double>(ui_.sliderboats->maximum()));
  horsessensitivity_ = (static_cast<double>(ui_.sliderhorses->maximum() - ui_.sliderhorses->value()) / static_cast<double>(ui_.sliderhorses->maximum()));
  accept();
}

}

QT_END_NAMESPACE
