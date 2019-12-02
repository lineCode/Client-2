// managerecordingjobwindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingjobwindow.h"

#include <monocleprotocol/streamingprotocol_generated.h>
#include <network/uri.hpp>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>

#include "monocleclient/device.h"
#include "monocleclient/file.h"
#include "monocleclient/managerecordingjobsourceobjectdetectorwindow.h"
#include "monocleclient/managerecordingjobsourceonvifwindow.h"
#include "monocleclient/managerecordingjobsourcertspwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/receiver.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"
#include "monocleclient/recordingjobsource.h"
#include "monocleclient/recordingjobsourcetrack.h"
#include "monocleclient/view.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Globals /////

const int SOURCE_TAG_ROLE = Qt::UserRole;
const int PROFILE_ROLE = Qt::UserRole + 1;
const int STREAMING_PROTOCOL_ROLE = Qt::UserRole + 2;
const int ROTATION_ROLE = Qt::UserRole + 3;
const int OBJECT_DETECTOR_ACCURACY_ROLE = Qt::UserRole + 4;
const int OBJECT_DETECTOR_HUMANS_ENABLED_ROLE = Qt::UserRole + 5;
const int OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE = Qt::UserRole + 6;
const int OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE = Qt::UserRole + 7;
const int OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE = Qt::UserRole + 8;
const int OBJECT_DETECTOR_CARS_ENABLED_ROLE = Qt::UserRole + 9;
const int OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE = Qt::UserRole + 10;
const int OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE = Qt::UserRole + 11;
const int OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE = Qt::UserRole + 12;
const int OBJECT_DETECTOR_BUSES_ENABLED_ROLE = Qt::UserRole + 13;
const int OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE = Qt::UserRole + 14;
const int OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE = Qt::UserRole + 15;
const int OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE = Qt::UserRole + 16;
const int OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE = Qt::UserRole + 17;
const int OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE = Qt::UserRole + 18;
const int OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE = Qt::UserRole + 19;
const int OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE = Qt::UserRole + 20;
const int OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE = Qt::UserRole + 21;
const int OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE = Qt::UserRole + 22;
const int OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE = Qt::UserRole + 23;
const int OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE = Qt::UserRole + 24;
const int OBJECT_DETECTOR_CATS_ENABLED_ROLE = Qt::UserRole + 25;
const int OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE = Qt::UserRole + 26;
const int OBJECT_DETECTOR_DOGS_ENABLED_ROLE = Qt::UserRole + 27;
const int OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE = Qt::UserRole + 28;
const int OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE = Qt::UserRole + 29;
const int OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE = Qt::UserRole + 30;
const int OBJECT_DETECTOR_TRAINS_ENABLED_ROLE = Qt::UserRole + 31;
const int OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE = Qt::UserRole + 32;
const int OBJECT_DETECTOR_BOATS_ENABLED_ROLE = Qt::UserRole + 33;
const int OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE = Qt::UserRole + 34;
const int OBJECT_DETECTOR_HORSES_ENABLED_ROLE = Qt::UserRole + 35;
const int OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE = Qt::UserRole + 36;

///// Methods /////

ManageRecordingJobWindow::ManageRecordingJobWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack) :
  QDialog(parent),
  device_(device),
  recording_(recording),
  recordingjob_(recordingjob)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablerecordingjobsources->hideColumn(0);

  }

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  if (recordingjob_)
  {
    Setup(recordingjob_);

    connect(recording.data(), QOverload<const uint64_t>::of(&Recording::JobRemoved), this, &ManageRecordingJobWindow::RecordingJobRemoved);

    setWindowTitle("Edit Recording Job");
  }
  else
  {
    ui_.checkenabled->setChecked(true);

  }

  on_tablerecordingjobsources_itemSelectionChanged();

  if (recordingjobsourcetrack)
  {
    if (!SelectItem(recordingjobsourcetrack->GetToken()))
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Can not find Source: ") + QString::number(recordingjobsourcetrack->GetToken()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  }
}

ManageRecordingJobWindow::~ManageRecordingJobWindow()
{
  recordingjobconnection_.Close();

}

void ManageRecordingJobWindow::SetEnabled(const bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.spinpriority->setEnabled(enabled);
  ui_.checkenabled->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
  if (enabled)
  {
    on_tablerecordingjobsources_itemSelectionChanged();

  }
}

void ManageRecordingJobWindow::Setup(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  ui_.checkenabled->setChecked(recordingjob->GetEnabled());
  ui_.editname->setText(recordingjob->GetName());
  ui_.spinpriority->setValue(recordingjob->GetPriority());

  for (const QSharedPointer<client::RecordingJobSource>& recordingjobsource : recordingjob->GetSources())
  {
    for (const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack : recordingjobsource->GetTracks())
    {
      AddRecordingJobSourceTrack(recordingjobsource, recordingjobsourcetrack);

    }
  }
}

void ManageRecordingJobWindow::AddRecordingJobSourceTrack(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack)
{
  const QSharedPointer<client::Receiver> receiver = device_->GetReceiver(recordingjobsource->GetReceiverToken());
  if (!receiver)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Can not find Receiver: ") + QString::number(recordingjobsource->GetReceiverToken()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const int row = ui_.tablerecordingjobsources->rowCount();
  ui_.tablerecordingjobsources->insertRow(row);
  ui_.tablerecordingjobsources->setItem(row, 0, new QTableWidgetItem(QString::number(recordingjobsourcetrack->GetToken())));
  ui_.tablerecordingjobsources->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(recordingjobsourcetrack->GetToken()));
  ui_.tablerecordingjobsources->setItem(row, 1, new QTableWidgetItem((receiver->GetReceiverMode() != monocle::ReceiverMode::NeverConnect) ? "True" : "False" ));
  ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(receiver->GetReceiverMode() != monocle::ReceiverMode::NeverConnect));
  ui_.tablerecordingjobsources->setItem(row, 2, new QTableWidgetItem(recordingjobsourcetrack->GetTrack()->GetDescription() + " (" + monocle::EnumNameTrackType(recordingjobsourcetrack->GetTrack()->GetTrackType()) + ")"));
  ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, recordingjobsourcetrack->GetTrack()->GetId());
  ui_.tablerecordingjobsources->setItem(row, 3, new QTableWidgetItem(receiver->GetMediaUri()));
  ui_.tablerecordingjobsources->setItem(row, 4, new QTableWidgetItem(receiver->GetUsername()));
  ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, receiver->GetPassword());
  QStringList parameters;
  parameters.reserve(static_cast<int>(receiver->GetParameters().size() + recordingjobsourcetrack->GetParameters().size()));
  for (const QString& parameter : receiver->GetParameters())
  {
    parameters.push_back(parameter);

  }
  for (const QString& parameter : recordingjobsourcetrack->GetParameters())
  {
    parameters.push_back(parameter);

  }

  ui_.tablerecordingjobsources->setItem(row, 5, new QTableWidgetItem(parameters.join(":")));
  const boost::optional<QString> sourcetag = recordingjobsourcetrack->GetSourceTag();
  ui_.tablerecordingjobsources->item(row, 5)->setData(SOURCE_TAG_ROLE, sourcetag.is_initialized() ? *sourcetag : QVariant());
  const boost::optional<QString> profiletoken = recordingjobsourcetrack->GetProfileToken();
  ui_.tablerecordingjobsources->item(row, 5)->setData(PROFILE_ROLE, profiletoken.is_initialized() ? *profiletoken : QVariant());
  const boost::optional<monocle::StreamingProtocol> streamingprotocol = receiver->GetStreamingProtocol();
  ui_.tablerecordingjobsources->item(row, 5)->setData(STREAMING_PROTOCOL_ROLE, (streamingprotocol.is_initialized() ? QVariant(static_cast<int>(*streamingprotocol)) : QVariant()));
  const boost::optional<ROTATION> rotation = recordingjobsourcetrack->GetRotation();
  ui_.tablerecordingjobsources->item(row, 5)->setData(ROTATION_ROLE, rotation.is_initialized() ? QVariant(ToString(*rotation)) : QVariant());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_ACCURACY_ROLE, recordingjobsourcetrack->GetObjectDetectorAccuracy());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HUMANS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorHumansEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorHumansSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorBicyclesEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorBicyclesSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CARS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorCarsEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorCarsSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorMotorbikesEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorMotorbikesSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BUSES_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorBusesEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorBusesSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorTrucksEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorTrucksSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorBackpacksEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorBackpacksSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorUmbrellasEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorUmbrellasSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorHandbagsEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorHandbagsSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorSuitcasesEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorSuitcasesSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CATS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorCatsEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorCatsSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_DOGS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorDogsEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorDogsSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorAeroplanesEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorAeroplanesSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRAINS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorTrainsEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorTrainsSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BOATS_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorBoatsEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorBoatsSensitivity());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HORSES_ENABLED_ROLE, recordingjobsourcetrack->GetObjectDetectorHorsesEnabled());
  ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE, recordingjobsourcetrack->GetObjectDetectorHorsesSensitivity());
}

std::vector<uint32_t> ManageRecordingJobWindow::GetTrackIds() const
{
  std::vector<uint32_t> trackids;
  trackids.reserve(ui_.tablerecordingjobsources->rowCount());
  for (int i = 0; i < ui_.tablerecordingjobsources->rowCount(); ++i)
  {
    trackids.push_back(ui_.tablerecordingjobsources->item(i, 2)->data(Qt::UserRole).toUInt());

  }
  return trackids;
}

bool ManageRecordingJobWindow::SelectItem(const uint64_t token)
{
  for (int i = 0; i < ui_.tablerecordingjobsources->rowCount(); ++i)
  {
    QTableWidgetItem* item = ui_.tablerecordingjobsources->item(i, 0);
    if (item->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.tablerecordingjobsources->clearSelection();
      ui_.tablerecordingjobsources->selectRow(i);
      return true;
    }
  }
  return false;
}

std::vector<std::string> ManageRecordingJobWindow::GetParameters(const int row) const
{
  std::vector<std::string> recordingjobsourcetrackparameters;
  if (!ui_.tablerecordingjobsources->item(row, 5)->data(SOURCE_TAG_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((SOURCE_TAG_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(SOURCE_TAG_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(PROFILE_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((PROFILE_TOKEN_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(PROFILE_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(ROTATION_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((ROTATION_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(ROTATION_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_ACCURACY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_ACCURACY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_ACCURACY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HUMANS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_HUMANS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HUMANS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_HUMANS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BICYCLES_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BICYCLES_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CARS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_CARS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CARS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_CARS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_MOTORBIKES_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BUSES_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BUSES_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BUSES_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BUSES_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_TRUCKS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_TRUCKS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BACKPACKS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_UMBRELLAS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_HANDBAGS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_SUITCASES_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_SUITCASES_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CATS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_CATS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CATS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_CATS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_DOGS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_DOGS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_DOGS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_DOGS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_AEROPLANES_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRAINS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_TRAINS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRAINS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_TRAINS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BOATS_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BOATS_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BOATS_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_BOATS_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE).toString()).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HORSES_ENABLED_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_HORSES_ENABLED_PARAMETER_NAME + "=" + (ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HORSES_ENABLED_ROLE).toBool() ? "1" : "0")).toStdString());

  }

  if (!ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE).toString().isEmpty())
  {
    recordingjobsourcetrackparameters.push_back((OBJECT_DETECTOR_HORSES_SENSITIVITY_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE).toString()).toStdString());

  }
  return recordingjobsourcetrackparameters;
}

void ManageRecordingJobWindow::RecordingJobRemoved(const uint64_t token)
{
  QMessageBox(QMessageBox::Warning, tr("Error"), tr("Recording Job has been removed"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
  reject();
}

void ManageRecordingJobWindow::on_tablerecordingjobsources_itemSelectionChanged()
{
  const QModelIndexList selectedrows = ui_.tablerecordingjobsources->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    ui_.buttonedit->setEnabled(false);
    ui_.buttonremove->setEnabled(false);
  }
  else
  {
    ui_.buttonedit->setEnabled(true);
    ui_.buttonremove->setEnabled(true);
  }
}

void ManageRecordingJobWindow::on_buttonaddrtspsource_clicked()
{
  ManageRecordingJobSourceRTSPWindow dialog(this, device_, recording_, recordingjob_, false, true, 0, QString(), QString(), monocle::StreamingProtocol::TCPInterleaved, QString(), QString(), QString(), GetTrackIds());
  if (dialog.exec() == QDialog::Accepted)
  {
    const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(dialog.trackid_);
    if (!track)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(dialog.trackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    QStringList parameters;
    if (!dialog.sourcetag_.isEmpty())
    {
      parameters.push_back(SOURCE_TAG_PARAMETER_NAME + "=" + dialog.sourcetag_);

    }

    parameters.push_back(STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(dialog.streamingprotocol_));

    if (!dialog.rotation_.isEmpty())
    {
      parameters.push_back(ROTATION_PARAMETER_NAME + "=" + dialog.rotation_);

    }

    const int row = ui_.tablerecordingjobsources->rowCount();
    ui_.tablerecordingjobsources->insertRow(row);

    ui_.tablerecordingjobsources->setItem(row, 1, new QTableWidgetItem(dialog.enabled_ ? "True" : "False"));
    ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(dialog.enabled_));
    ui_.tablerecordingjobsources->setItem(row, 2, new QTableWidgetItem(track->GetDescription() + " (" + monocle::EnumNameTrackType(track->GetTrackType()) + ")"));
    ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, dialog.trackid_);
    ui_.tablerecordingjobsources->setItem(row, 3, new QTableWidgetItem(dialog.uri_));
    ui_.tablerecordingjobsources->item(row, 3)->setText(dialog.uri_);
    ui_.tablerecordingjobsources->setItem(row, 4, new QTableWidgetItem(dialog.username_));
    ui_.tablerecordingjobsources->item(row, 4)->setText(dialog.username_);
    ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, dialog.password_);
    ui_.tablerecordingjobsources->setItem(row, 5, new QTableWidgetItem(parameters.join(":")));
    ui_.tablerecordingjobsources->item(row, 5)->setData(SOURCE_TAG_ROLE, dialog.sourcetag_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(PROFILE_ROLE, QString());
    ui_.tablerecordingjobsources->item(row, 5)->setData(STREAMING_PROTOCOL_ROLE, static_cast<int>(dialog.streamingprotocol_));
    ui_.tablerecordingjobsources->item(row, 5)->setData(ROTATION_ROLE, dialog.rotation_);
  }
}

void ManageRecordingJobWindow::on_buttonaddonvifsource_clicked()
{
  ManageRecordingJobSourceONVIFWindow dialog(this,
                                             device_,
                                             recording_,
                                             recordingjob_,
                                             false,
                                             true,
                                             0,
                                             QString(),
                                             QString(),
                                             QString(),
                                             monocle::StreamingProtocol::TCPInterleaved,
                                             QString(),
                                             QString(),
                                             QString(),
                                             GetTrackIds());
  if (dialog.exec() == QDialog::Accepted)
  {
    const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(dialog.trackid_);
    if (!track)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(dialog.trackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    QStringList parameters;
    if (!dialog.sourcetag_.isEmpty())
    {
      parameters.push_back(SOURCE_TAG_PARAMETER_NAME + "=" + dialog.sourcetag_);

    }

    if (!dialog.profiletoken_.isEmpty())
    {
      parameters.push_back(PROFILE_TOKEN_PARAMETER_NAME + "=" + dialog.profiletoken_);

    }

    parameters.push_back(STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(dialog.streamingprotocol_));

    if (!dialog.rotation_.isEmpty())
    {
      parameters.push_back(ROTATION_PARAMETER_NAME + "=" + dialog.rotation_);

    }

    const int row = ui_.tablerecordingjobsources->rowCount();
    ui_.tablerecordingjobsources->insertRow(row);

    ui_.tablerecordingjobsources->setItem(row, 1, new QTableWidgetItem(dialog.enabled_ ? "True" : "False"));
    ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(dialog.enabled_));
    ui_.tablerecordingjobsources->setItem(row, 2, new QTableWidgetItem(track->GetDescription() + " (" + monocle::EnumNameTrackType(track->GetTrackType()) + ")"));
    ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, dialog.trackid_);
    ui_.tablerecordingjobsources->setItem(row, 3, new QTableWidgetItem(dialog.uri_));
    ui_.tablerecordingjobsources->item(row, 3)->setText(dialog.uri_);
    ui_.tablerecordingjobsources->setItem(row, 4, new QTableWidgetItem(dialog.username_));
    ui_.tablerecordingjobsources->item(row, 4)->setText(dialog.username_);
    ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, dialog.password_);
    ui_.tablerecordingjobsources->setItem(row, 5, new QTableWidgetItem(parameters.join(":")));
    ui_.tablerecordingjobsources->item(row, 5)->setData(SOURCE_TAG_ROLE, dialog.sourcetag_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(PROFILE_ROLE, dialog.profiletoken_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(STREAMING_PROTOCOL_ROLE, static_cast<int>(dialog.streamingprotocol_));
    ui_.tablerecordingjobsources->item(row, 5)->setData(ROTATION_ROLE, dialog.rotation_);
  }
}

void ManageRecordingJobWindow::on_buttonaddobjectdetector_clicked()
{
  if (!device_->SupportsObjectDetection())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Server does not support object detection, please upgrade server to the latest version"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (device_->GetNumCudaDevices() <= 0)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Server does not have any CUDA Devices, please install a CUDA device"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (device_->GetNumObjectDetectors() >= device_->GetMaxObjectDetectors())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Max object detectors reached"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (recording_->GetMetadataTracks().empty())//TODO this is going in the bin, but definitely doesn't work now
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No metadata tracks available for this recording, please add a metadata track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (recording_->GetVideoTracks().empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No video tracks available for this recording, please add a video track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageRecordingJobSourceObjectDetectorWindow dialog(this,
                                                      device_,
                                                      recording_,
                                                      recordingjob_,
                                                      false,
                                                      true,
                                                      0,
                                                      QString(),
                                                      1,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      true,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      0.5,
                                                      GetTrackIds());
  if (dialog.exec() == QDialog::Accepted)
  {
    const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(dialog.metadatatrackid_);//TODO rename if we even keep this window... which we won't...
    if (!track)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find metadata track: ") + QString::number(dialog.metadatatrackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    if (track->GetTrackType() != monocle::TrackType::ObjectDetector)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Metadata track is not of metadata type: ") + QString::number(dialog.metadatatrackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    const QStringList parameters =
    {
       QString("accuracy=") + QString::number(dialog.accuracy_),
       QString("humans=") + (dialog.humans_ ? "1" : "0"), QString("humanssensitivity=") + QString::number(dialog.humanssensitivity_),
       QString("bicycles=") + (dialog.bicycles_ ? "1" : "0"), QString("bicyclessensitivity=") + QString::number(dialog.bicyclessensitivity_),
       QString("cars=") + (dialog.cars_ ? "1" : "0"), QString("carssensitivity=") + QString::number(dialog.carssensitivity_),
       QString("motorbikes=") + (dialog.motorbikes_ ? "1" : "0"), QString("motorbikessensitivity=") + QString::number(dialog.motorbikessensitivity_),
       QString("buses=") + (dialog.buses_ ? "1" : "0"), QString("busessensitivity=") + QString::number(dialog.busessensitivity_),
       QString("trucks=") + (dialog.trucks_ ? "1" : "0"), QString("truckssensitivity=") + QString::number(dialog.truckssensitivity_),
       QString("backpacks=") + (dialog.backpacks_ ? "1" : "0"), QString("backpackssensitivity=") + QString::number(dialog.backpackssensitivity_),
       QString("umbrellas=") + (dialog.umbrellas_ ? "1" : "0"), QString("umbrellassensitivity=") + QString::number(dialog.umbrellassensitivity_),
       QString("handbags=") + (dialog.handbags_ ? "1" : "0"), QString("handbagssensitivity=") + QString::number(dialog.handbagssensitivity_),
       QString("suitcases=") + (dialog.suitcases_ ? "1" : "0"), QString("suitcasessensitivity=") + QString::number(dialog.suitcasessensitivity_),
       QString("cats=") + (dialog.cats_ ? "1" : "0"), QString("catssensitivity=") + QString::number(dialog.catssensitivity_),
       QString("dogs=") + (dialog.dogs_ ? "1" : "0"), QString("dogssensitivity=") + QString::number(dialog.dogssensitivity_),
       QString("aeroplanes=") + (dialog.aeroplanes_ ? "1" : "0"), QString("aeroplanessensitivity=") + QString::number(dialog.aeroplanessensitivity_),
       QString("trains=") + (dialog.trains_ ? "1" : "0"), QString("trainssensitivity=") + QString::number(dialog.trainssensitivity_),
       QString("boats=") + (dialog.boats_ ? "1" : "0"), QString("boatssensitivity=") + QString::number(dialog.boatssensitivity_),
       QString("horses=") + (dialog.horses_ ? "1" : "0"), QString("horsessensitivity=") + QString::number(dialog.horsessensitivity_)
    };

    const int row = ui_.tablerecordingjobsources->rowCount();
    ui_.tablerecordingjobsources->insertRow(row);
    
    ui_.tablerecordingjobsources->setItem(row, 1, new QTableWidgetItem(dialog.enabled_ ? "True" : "False"));
    ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(dialog.enabled_));
    ui_.tablerecordingjobsources->setItem(row, 2, new QTableWidgetItem(track->GetDescription() + " (" + monocle::EnumNameTrackType(track->GetTrackType()) + ")"));
    ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, dialog.metadatatrackid_);
    ui_.tablerecordingjobsources->setItem(row, 3, new QTableWidgetItem(dialog.uri_));
    ui_.tablerecordingjobsources->item(row, 3)->setText(dialog.uri_);
    ui_.tablerecordingjobsources->setItem(row, 4, new QTableWidgetItem());
    ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, QString());
    ui_.tablerecordingjobsources->setItem(row, 5, new QTableWidgetItem(parameters.join(":")));
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_ACCURACY_ROLE, dialog.accuracy_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HUMANS_ENABLED_ROLE, dialog.humans_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE, dialog.humanssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE, dialog.bicycles_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE, dialog.bicyclessensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CARS_ENABLED_ROLE, dialog.cars_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE, dialog.carssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE, dialog.motorbikes_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE, dialog.motorbikessensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BUSES_ENABLED_ROLE, dialog.buses_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE, dialog.busessensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE, dialog.trucks_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE, dialog.truckssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE, dialog.backpacks_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE, dialog.backpackssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE, dialog.umbrellas_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE, dialog.umbrellassensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE, dialog.handbags_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE, dialog.handbagssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE, dialog.suitcases_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE, dialog.suitcasessensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CATS_ENABLED_ROLE, dialog.cats_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE, dialog.catssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_DOGS_ENABLED_ROLE, dialog.dogs_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE, dialog.dogssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE, dialog.aeroplanes_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE, dialog.aeroplanessensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRAINS_ENABLED_ROLE, dialog.trains_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE, dialog.trainssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BOATS_ENABLED_ROLE, dialog.boats_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE, dialog.boatssensitivity_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HORSES_ENABLED_ROLE, dialog.horses_);
    ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE, dialog.horsessensitivity_);
  }
}

void ManageRecordingJobWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tablerecordingjobsources->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording job source selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  const int row = selectedrows.at(0).row();

  const QString mediauri = ui_.tablerecordingjobsources->item(row, 3)->text();
  network::uri uri;
  try
  {
    uri = network::uri(mediauri.toStdString());

  }
  catch (...)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid URI: ") + mediauri, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!uri.has_scheme())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No schema found: ") + mediauri, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (uri.scheme().to_string() == "objectdetector")
  {
    ManageRecordingJobSourceObjectDetectorWindow dialog(this,
                                                        device_,
                                                        recording_,
                                                        recordingjob_,
                                                        true,
                                                        ui_.tablerecordingjobsources->item(row, 1)->data(Qt::UserRole).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 2)->data(Qt::UserRole).toUInt(),
                                                        ui_.tablerecordingjobsources->item(row, 3)->text(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_ACCURACY_ROLE).toUInt(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HUMANS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CARS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BUSES_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CATS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_DOGS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRAINS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BOATS_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HORSES_ENABLED_ROLE).toBool(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE).toDouble(),
                                                        ui_.tablerecordingjobsources->item(row, 5)->data(OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE).toDouble(),
                                                        GetTrackIds());
    if (dialog.exec() == QDialog::Accepted)
    {
      const QSharedPointer<client::RecordingTrack> metadatatrack = recording_->GetTrack(dialog.metadatatrackid_);
      if (!metadatatrack)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(dialog.metadatatrackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }

      const QStringList parameters =
      {
         QString("accuracy=") + QString::number(dialog.accuracy_),
         QString("humans=") + (dialog.humans_ ? "1" : "0"), QString("humanssensitivity=") + QString::number(dialog.humanssensitivity_),
         QString("bicycles=") + (dialog.bicycles_ ? "1" : "0"), QString("bicyclessensitivity=") + QString::number(dialog.bicyclessensitivity_),
         QString("cars=") + (dialog.cars_ ? "1" : "0"), QString("carssensitivity=") + QString::number(dialog.carssensitivity_),
         QString("motorbikes=") + (dialog.motorbikes_ ? "1" : "0"), QString("motorbikessensitivity=") + QString::number(dialog.motorbikessensitivity_),
         QString("buses=") + (dialog.buses_ ? "1" : "0"), QString("busessensitivity=") + QString::number(dialog.busessensitivity_),
         QString("trucks=") + (dialog.trucks_ ? "1" : "0"), QString("truckssensitivity=") + QString::number(dialog.truckssensitivity_),
         QString("backpacks=") + (dialog.backpacks_ ? "1" : "0"), QString("backpackssensitivity=") + QString::number(dialog.backpackssensitivity_),
         QString("umbrellas=") + (dialog.umbrellas_ ? "1" : "0"), QString("umbrellassensitivity=") + QString::number(dialog.umbrellassensitivity_),
         QString("handbags=") + (dialog.handbags_ ? "1" : "0"), QString("handbagssensitivity=") + QString::number(dialog.handbagssensitivity_),
         QString("suitcases=") + (dialog.suitcases_ ? "1" : "0"), QString("suitcasessensitivity=") + QString::number(dialog.suitcasessensitivity_),
         QString("cats=") + (dialog.cats_ ? "1" : "0"), QString("catssensitivity=") + QString::number(dialog.catssensitivity_),
         QString("dogs=") + (dialog.dogs_ ? "1" : "0"), QString("dogssensitivity=") + QString::number(dialog.dogssensitivity_),
         QString("aeroplanes=") + (dialog.aeroplanes_ ? "1" : "0"), QString("aeroplanessensitivity=") + QString::number(dialog.aeroplanessensitivity_),
         QString("trains=") + (dialog.trains_ ? "1" : "0"), QString("trainssensitivity=") + QString::number(dialog.trainssensitivity_),
         QString("boats=") + (dialog.boats_ ? "1" : "0"), QString("boatssensitivity=") + QString::number(dialog.boatssensitivity_),
         QString("horses=") + (dialog.horses_ ? "1" : "0"), QString("horsessensitivity=") + QString::number(dialog.horsessensitivity_)
      };

      ui_.tablerecordingjobsources->setItem(row, 1, new QTableWidgetItem(dialog.enabled_ ? "True" : "False"));
      ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(dialog.enabled_));
      ui_.tablerecordingjobsources->setItem(row, 2, new QTableWidgetItem(metadatatrack->GetDescription() + " (" + monocle::EnumNameTrackType(metadatatrack->GetTrackType()) + ")"));
      ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, dialog.metadatatrackid_);
      ui_.tablerecordingjobsources->setItem(row, 3, new QTableWidgetItem(dialog.uri_));
      ui_.tablerecordingjobsources->item(row, 3)->setText(dialog.uri_);
      ui_.tablerecordingjobsources->setItem(row, 4, new QTableWidgetItem());
      ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, QString());
      ui_.tablerecordingjobsources->setItem(row, 5, new QTableWidgetItem(parameters.join(":")));
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_ACCURACY_ROLE, dialog.accuracy_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HUMANS_ENABLED_ROLE, dialog.humans_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HUMANS_SENSITIVITY_ROLE, dialog.humanssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BICYCLES_ENABLED_ROLE, dialog.bicycles_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BICYCLES_SENSITIVITY_ROLE, dialog.bicyclessensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CARS_ENABLED_ROLE, dialog.cars_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CARS_SENSITIVITY_ROLE, dialog.carssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_MOTORBIKES_ENABLED_ROLE, dialog.motorbikes_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_MOTORBIKES_SENSITIVITY_ROLE, dialog.motorbikessensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BUSES_ENABLED_ROLE, dialog.buses_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BUSES_SENSITIVITY_ROLE, dialog.busessensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRUCKS_ENABLED_ROLE, dialog.trucks_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRUCKS_SENSITIVITY_ROLE, dialog.truckssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BACKPACKS_ENABLED_ROLE, dialog.backpacks_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BACKPACKS_SENSITIVITY_ROLE, dialog.backpackssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_UMBRELLAS_ENABLED_ROLE, dialog.umbrellas_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_UMBRELLAS_SENSITIVITY_ROLE, dialog.umbrellassensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HANDBAGS_ENABLED_ROLE, dialog.handbags_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HANDBAGS_SENSITIVITY_ROLE, dialog.handbagssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_SUITCASES_ENABLED_ROLE, dialog.suitcases_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_SUITCASES_SENSITIVITY_ROLE, dialog.suitcasessensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CATS_ENABLED_ROLE, dialog.cats_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_CATS_SENSITIVITY_ROLE, dialog.catssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_DOGS_ENABLED_ROLE, dialog.dogs_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_DOGS_SENSITIVITY_ROLE, dialog.dogssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_AEROPLANES_ENABLED_ROLE, dialog.aeroplanes_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_AEROPLANES_SENSITIVITY_ROLE, dialog.aeroplanessensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRAINS_ENABLED_ROLE, dialog.trains_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_TRAINS_SENSITIVITY_ROLE, dialog.trainssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BOATS_ENABLED_ROLE, dialog.boats_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_BOATS_SENSITIVITY_ROLE, dialog.boatssensitivity_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HORSES_ENABLED_ROLE, dialog.horses_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(OBJECT_DETECTOR_HORSES_SENSITIVITY_ROLE, dialog.horsessensitivity_);
    }
  }
  else if (uri.scheme().to_string() == "http")
  {
    ManageRecordingJobSourceONVIFWindow dialog(this,
                                               device_,
                                               recording_,
                                               recordingjob_,
                                               true,
                                               ui_.tablerecordingjobsources->item(row, 1)->data(Qt::UserRole).toBool(),
                                               ui_.tablerecordingjobsources->item(row, 2)->data(Qt::UserRole).toUInt(),
                                               ui_.tablerecordingjobsources->item(row, 3)->text(),
                                               ui_.tablerecordingjobsources->item(row, 5)->data(PROFILE_ROLE).toString(),
                                               ui_.tablerecordingjobsources->item(row, 5)->data(Qt::UserRole).toString(),
                                               static_cast<monocle::StreamingProtocol>(ui_.tablerecordingjobsources->item(row, 5)->data(STREAMING_PROTOCOL_ROLE).toInt()),
                                               ui_.tablerecordingjobsources->item(row, 4)->text(),
                                               ui_.tablerecordingjobsources->item(row, 4)->data(Qt::UserRole).toString(),
                                               ui_.tablerecordingjobsources->item(row, 5)->data(ROTATION_ROLE).toString(),
                                               GetTrackIds());
    if (dialog.exec() == QDialog::Accepted)
    {
      const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(dialog.trackid_);
      if (!track)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(dialog.trackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }

      QStringList parameters;
      if (!dialog.sourcetag_.isEmpty())
      {
        parameters.push_back(SOURCE_TAG_PARAMETER_NAME + "=" + dialog.sourcetag_);

      }

      if (!dialog.profiletoken_.isEmpty())
      {
        parameters.push_back(PROFILE_TOKEN_PARAMETER_NAME + "=" + dialog.profiletoken_);

      }

      parameters.push_back(STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(dialog.streamingprotocol_));

      if (!dialog.rotation_.isEmpty())
      {
        parameters.push_back(ROTATION_PARAMETER_NAME + "=" + dialog.rotation_);

      }

      ui_.tablerecordingjobsources->item(row, 1)->setText(dialog.enabled_ ? "True" : "False");
      ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(dialog.enabled_));
      ui_.tablerecordingjobsources->item(row, 2)->setText(track->GetDescription() + " (" + monocle::EnumNameTrackType(track->GetTrackType()) + ")");
      ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, dialog.trackid_);
      ui_.tablerecordingjobsources->item(row, 3)->setText(dialog.uri_);
      ui_.tablerecordingjobsources->item(row, 4)->setText(dialog.username_);
      ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, dialog.password_);
      ui_.tablerecordingjobsources->item(row, 5)->setText(parameters.join(":"));
      ui_.tablerecordingjobsources->item(row, 5)->setData(SOURCE_TAG_ROLE, dialog.sourcetag_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(PROFILE_ROLE, dialog.profiletoken_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(STREAMING_PROTOCOL_ROLE, static_cast<int>(dialog.streamingprotocol_));
      ui_.tablerecordingjobsources->item(row, 5)->setData(ROTATION_ROLE, dialog.rotation_);
    }
  }
  else if (uri.scheme().to_string() == "rtsp")
  {
    ManageRecordingJobSourceRTSPWindow dialog(this,
                                              device_,
                                              recording_,
                                              recordingjob_,
                                              true,
                                              ui_.tablerecordingjobsources->item(row, 1)->data(Qt::UserRole).toBool(),
                                              ui_.tablerecordingjobsources->item(row, 2)->data(Qt::UserRole).toUInt(),
                                              ui_.tablerecordingjobsources->item(row, 3)->text(),
                                              ui_.tablerecordingjobsources->item(row, 5)->data(SOURCE_TAG_ROLE).toString(),
                                              static_cast<monocle::StreamingProtocol>(ui_.tablerecordingjobsources->item(row, 5)->data(STREAMING_PROTOCOL_ROLE).toInt()),
                                              ui_.tablerecordingjobsources->item(row, 4)->text(),
                                              ui_.tablerecordingjobsources->item(row, 4)->data(Qt::UserRole).toString(),
                                              ui_.tablerecordingjobsources->item(row, 5)->data(ROTATION_ROLE).toString(),
                                              GetTrackIds());
    if (dialog.exec() == QDialog::Accepted)
    {
      const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(dialog.trackid_);
      if (!track)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to find track: ") + QString::number(dialog.trackid_), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }

      QString sourcetagparameter;
      if (!dialog.sourcetag_.isEmpty())
      {
        sourcetagparameter = SOURCE_TAG_PARAMETER_NAME + "=" + dialog.sourcetag_ + ":";

      }

      QString rotationparameter;
      if (!dialog.rotation_.isEmpty())
      {
        rotationparameter = ":" + ROTATION_PARAMETER_NAME + "=" + dialog.rotation_;

      }

      ui_.tablerecordingjobsources->item(row, 1)->setText(dialog.enabled_ ? "True" : "False");
      ui_.tablerecordingjobsources->item(row, 1)->setData(Qt::UserRole, static_cast<bool>(dialog.enabled_));
      ui_.tablerecordingjobsources->item(row, 2)->setText(track->GetDescription() + " (" + monocle::EnumNameTrackType(track->GetTrackType()) + ")");
      ui_.tablerecordingjobsources->item(row, 2)->setData(Qt::UserRole, dialog.trackid_);
      ui_.tablerecordingjobsources->item(row, 3)->setText(dialog.uri_);
      ui_.tablerecordingjobsources->item(row, 4)->setText(dialog.username_);
      ui_.tablerecordingjobsources->item(row, 4)->setData(Qt::UserRole, dialog.password_);
      ui_.tablerecordingjobsources->item(row, 5)->setText(sourcetagparameter + STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(dialog.streamingprotocol_) + rotationparameter);
      ui_.tablerecordingjobsources->item(row, 5)->setData(SOURCE_TAG_ROLE, dialog.sourcetag_);
      ui_.tablerecordingjobsources->item(row, 5)->setData(PROFILE_ROLE, QVariant());
      ui_.tablerecordingjobsources->item(row, 5)->setData(STREAMING_PROTOCOL_ROLE, static_cast<int>(dialog.streamingprotocol_));
      ui_.tablerecordingjobsources->item(row, 5)->setData(ROTATION_ROLE, dialog.rotation_);
    }
  }
  else
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unknown schema: ") + QString::fromStdString(uri.scheme().to_string()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
}

void ManageRecordingJobWindow::on_buttonremove_clicked()
{
  const QModelIndexList selectedrows = ui_.tablerecordingjobsources->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording job selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  const int row = selectedrows.at(0).row();

  if (QMessageBox::question(this, tr("Remove Recording Job"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  ui_.tablerecordingjobsources->removeRow(row);
}

void ManageRecordingJobWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a name for this Recording Job"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  SetEnabled(false);
  if (recordingjob_) // Edit
  {
    std::vector<monocle::CHANGERECORDINGJOBSOURCE> sources;
    sources.reserve(ui_.tablerecordingjobsources->rowCount());
    for (int i = 0; i < ui_.tablerecordingjobsources->rowCount(); ++i)
    {
      std::vector<std::string> sourceparameters;
      if (!ui_.tablerecordingjobsources->item(i, 5)->data(STREAMING_PROTOCOL_ROLE).isNull())
      {
        sourceparameters.push_back((STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(static_cast<monocle::StreamingProtocol>(ui_.tablerecordingjobsources->item(i, 5)->data(STREAMING_PROTOCOL_ROLE).toInt()))).toStdString());

      }

      sources.emplace_back(ui_.tablerecordingjobsources->item(i, 2)->data(Qt::UserRole).toUInt(),
                            ui_.tablerecordingjobsources->item(i, 1)->data(Qt::UserRole).toBool() ? monocle::ReceiverMode::AutoConnect : monocle::ReceiverMode::NeverConnect,
                            sourceparameters,
                            ui_.tablerecordingjobsources->item(i, 3)->text().toStdString(),
                            ui_.tablerecordingjobsources->item(i, 4)->text().toStdString(),
                            ui_.tablerecordingjobsources->item(i, 4)->data(Qt::UserRole).toString().toStdString(),
                            GetParameters(i));
    }

    recordingjobconnection_ = device_->ChangeRecordingJob(recording_->GetToken(), recordingjob_->GetToken(), ui_.editname->text().toStdString(), ui_.checkenabled->isChecked(), ui_.spinpriority->value(), sources, [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGERECORDINGJOBRESPONSE& changerecordingjobresponse)
    {
      SetEnabled(true);
      if (changerecordingjobresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changerecordingjobresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
  else // Add
  {
    std::vector<monocle::ADDRECORDINGJOBSOURCE> sources;
    sources.reserve(ui_.tablerecordingjobsources->rowCount());
    for (int i = 0; i < ui_.tablerecordingjobsources->rowCount(); ++i)
    {
      std::vector<std::string> sourceparameters;
      if (!ui_.tablerecordingjobsources->item(i, 5)->data(STREAMING_PROTOCOL_ROLE).isNull())
      {
        sourceparameters.push_back((STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(static_cast<monocle::StreamingProtocol>(ui_.tablerecordingjobsources->item(i, 5)->data(STREAMING_PROTOCOL_ROLE).toInt()))).toStdString());

      }

      sources.emplace_back(ui_.tablerecordingjobsources->item(i, 2)->data(Qt::UserRole).toUInt(),
                           ui_.tablerecordingjobsources->item(i, 1)->data(Qt::UserRole).toBool() ? monocle::ReceiverMode::AutoConnect : monocle::ReceiverMode::NeverConnect,
                           sourceparameters,
                           ui_.tablerecordingjobsources->item(i, 3)->text().toStdString(),
                           ui_.tablerecordingjobsources->item(i, 4)->text().toStdString(),
                           ui_.tablerecordingjobsources->item(i, 4)->data(Qt::UserRole).toString().toStdString(),
                           GetParameters(i));
    }

    recordingjobconnection_ = device_->AddRecordingJob(recording_->GetToken(), ui_.editname->text().toStdString(), ui_.checkenabled->isChecked(), ui_.spinpriority->value(), sources, [this](const std::chrono::nanoseconds latency, const monocle::client::ADDRECORDINGJOBRESPONSE& addrecordingjobresponse)
    {
      SetEnabled(true);
      if (addrecordingjobresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(addrecordingjobresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
