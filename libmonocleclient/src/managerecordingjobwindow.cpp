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
  ui_.tablerecordingjobsources->item(row, 5)->setData(SOURCE_TAG_ROLE, sourcetag.is_initialized() ? *sourcetag : QString());
  const boost::optional<QString> profiletoken = recordingjobsourcetrack->GetProfileToken();
  ui_.tablerecordingjobsources->item(row, 5)->setData(PROFILE_ROLE, profiletoken.is_initialized() ? *profiletoken : QString());
  const boost::optional<monocle::StreamingProtocol> streamingprotocol = receiver->GetStreamingProtocol();
  ui_.tablerecordingjobsources->item(row, 5)->setData(STREAMING_PROTOCOL_ROLE, static_cast<int>(streamingprotocol.is_initialized() ? *streamingprotocol : monocle::StreamingProtocol::TCPInterleaved));
  const boost::optional<ROTATION> rotation = recordingjobsourcetrack->GetRotation();
  ui_.tablerecordingjobsources->item(row, 5)->setData(ROTATION_ROLE, ToString(rotation.is_initialized() ? *rotation : ROTATION::_0));
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

  if (uri.scheme().to_string() == "http")
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
  if (recordingjob_)
  {
    std::vector<monocle::CHANGERECORDINGJOBSOURCE> sources;
    sources.reserve(ui_.tablerecordingjobsources->rowCount());
    for (int i = 0; i < ui_.tablerecordingjobsources->rowCount(); ++i)
    {
      std::vector<std::string> recordingjobsourcetrackparameters;
      if (!ui_.tablerecordingjobsources->item(i, 5)->data(SOURCE_TAG_ROLE).toString().isEmpty())
      {
        recordingjobsourcetrackparameters.push_back((SOURCE_TAG_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(i, 5)->data(SOURCE_TAG_ROLE).toString()).toStdString());

      }

      if (!ui_.tablerecordingjobsources->item(i, 5)->data(PROFILE_ROLE).toString().isEmpty())
      {
        recordingjobsourcetrackparameters.push_back((PROFILE_TOKEN_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(i, 5)->data(PROFILE_ROLE).toString()).toStdString());

      }

      if (!ui_.tablerecordingjobsources->item(i, 5)->data(ROTATION_ROLE).toString().isEmpty())
      {
        recordingjobsourcetrackparameters.push_back((ROTATION_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(i, 5)->data(ROTATION_ROLE).toString()).toStdString());

      }

      sources.emplace_back(ui_.tablerecordingjobsources->item(i, 2)->data(Qt::UserRole).toUInt(),
                            ui_.tablerecordingjobsources->item(i, 1)->data(Qt::UserRole).toBool() ? monocle::ReceiverMode::AutoConnect : monocle::ReceiverMode::NeverConnect,
                            std::vector<std::string>({ (STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(static_cast<monocle::StreamingProtocol>(ui_.tablerecordingjobsources->item(i, 5)->data(STREAMING_PROTOCOL_ROLE).toInt()))).toStdString() }),
                            ui_.tablerecordingjobsources->item(i, 3)->text().toStdString(),
                            ui_.tablerecordingjobsources->item(i, 4)->text().toStdString(),
                            ui_.tablerecordingjobsources->item(i, 4)->data(Qt::UserRole).toString().toStdString(),
                            recordingjobsourcetrackparameters);
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
  else
  {
    std::vector<monocle::ADDRECORDINGJOBSOURCE> sources;
    sources.reserve(ui_.tablerecordingjobsources->rowCount());
    for (int i = 0; i < ui_.tablerecordingjobsources->rowCount(); ++i)
    {
      std::vector<std::string> recordingjobsourcetrackparameters;
      if (!ui_.tablerecordingjobsources->item(i, 5)->data(SOURCE_TAG_ROLE).toString().isEmpty())
      {
        recordingjobsourcetrackparameters.push_back((SOURCE_TAG_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(i, 5)->data(SOURCE_TAG_ROLE).toString()).toStdString());

      }

      if (!ui_.tablerecordingjobsources->item(i, 5)->data(PROFILE_ROLE).toString().isEmpty())
      {
        recordingjobsourcetrackparameters.push_back((PROFILE_TOKEN_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(i, 5)->data(PROFILE_ROLE).toString()).toStdString());

      }

      if (!ui_.tablerecordingjobsources->item(i, 5)->data(ROTATION_ROLE).toString().isEmpty())
      {
        recordingjobsourcetrackparameters.push_back((ROTATION_PARAMETER_NAME + "=" + ui_.tablerecordingjobsources->item(i, 5)->data(ROTATION_ROLE).toString()).toStdString());

      }

      sources.emplace_back(ui_.tablerecordingjobsources->item(i, 2)->data(Qt::UserRole).toUInt(),
                           ui_.tablerecordingjobsources->item(i, 1)->data(Qt::UserRole).toBool() ? monocle::ReceiverMode::AutoConnect : monocle::ReceiverMode::NeverConnect,
                           std::vector<std::string>({ (STREAMING_PROTOCOL_PARAMETER_NAME + "=" + monocle::EnumNameStreamingProtocol(static_cast<monocle::StreamingProtocol>(ui_.tablerecordingjobsources->item(i, 5)->data(STREAMING_PROTOCOL_ROLE).toInt()))).toStdString() }),
                           ui_.tablerecordingjobsources->item(i, 3)->text().toStdString(),
                           ui_.tablerecordingjobsources->item(i, 4)->text().toStdString(),
                           ui_.tablerecordingjobsources->item(i, 4)->data(Qt::UserRole).toString().toStdString(),
                           recordingjobsourcetrackparameters);
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
