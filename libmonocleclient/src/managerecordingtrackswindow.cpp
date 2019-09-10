// managerecordingtrackswindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingtrackswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/managerecordingtrackwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingTracksWindow::ManageRecordingTracksWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording) :
  QDialog(parent),
  device_(device),
  recording_(recording)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tabletracks->hideColumn(0);
    ui_.tabletracks->hideColumn(1);
  }

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(recording.data(), QOverload<const QSharedPointer<client::RecordingTrack>&>::of(&Recording::TrackAdded), this, &ManageRecordingTracksWindow::TrackAdded);
  connect(recording.data(), QOverload<const QSharedPointer<client::RecordingTrack>&>::of(&Recording::TrackChanged), this, &ManageRecordingTracksWindow::TrackChanged);
  connect(recording.data(), QOverload<const uint32_t>::of(&Recording::TrackRemoved), this, &ManageRecordingTracksWindow::TrackRemoved);

  for (const QSharedPointer<client::RecordingTrack>& track : recording_->GetTracks())
  {
    AddTrack(track);

  }
}

ManageRecordingTracksWindow::~ManageRecordingTracksWindow()
{
  removetrackconnection_.Close();

}

void ManageRecordingTracksWindow::AddTrack(const QSharedPointer<client::RecordingTrack>& track)
{
  const int row = ui_.tabletracks->rowCount();
  ui_.tabletracks->insertRow(row);
  ui_.tabletracks->setItem(row, 0, new QTableWidgetItem(QString::number(track->GetId())));
  ui_.tabletracks->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(track->GetId()));
  ui_.tabletracks->setItem(row, 1, new QTableWidgetItem(track->GetToken()));
  ui_.tabletracks->setItem(row, 2, new QTableWidgetItem(monocle::EnumNameTrackType(track->GetTrackType())));
  ui_.tabletracks->setItem(row, 3, new QTableWidgetItem(track->GetDescription()));
  ui_.tabletracks->setItem(row, 4, new QTableWidgetItem(track->GetFixedFiles() ? "True" : "False"));
  ui_.tabletracks->setItem(row, 5, new QTableWidgetItem(track->GetDigitalSigning() ? "True" : "False"));
  ui_.tabletracks->setItem(row, 6, new QTableWidgetItem(track->GetEncrypt() ? "True" : "False"));
  ui_.tabletracks->setItem(row, 7, new QTableWidgetItem(QString::number(track->GetFlushFrequency())));
}

void ManageRecordingTracksWindow::TrackAdded(const QSharedPointer<client::RecordingTrack>& track)
{
  AddTrack(track);

}

void ManageRecordingTracksWindow::TrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  for (int row = 0; row < ui_.tabletracks->rowCount(); ++row)
  {
    if (ui_.tabletracks->item(row, 0)->data(Qt::UserRole).toUInt() == track->GetId())
    {
      ui_.tabletracks->item(row, 1)->setText(track->GetToken());
      ui_.tabletracks->item(row, 2)->setText(monocle::EnumNameTrackType(track->GetTrackType()));
      ui_.tabletracks->item(row, 3)->setText(track->GetDescription());
      ui_.tabletracks->item(row, 4)->setText(track->GetFixedFiles() ? "True" : "False");
      ui_.tabletracks->item(row, 5)->setText(track->GetDigitalSigning() ? "True" : "False");
      ui_.tabletracks->item(row, 6)->setText(track->GetEncrypt() ? "True" : "False");
      ui_.tabletracks->item(row, 7)->setText(QString::number(track->GetFlushFrequency()));
    }
  }
}

void ManageRecordingTracksWindow::TrackRemoved(const uint32_t id)
{
  for (int i = (ui_.tabletracks->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tabletracks->item(i, 0)->data(Qt::UserRole) == id)
    {
      ui_.tabletracks->removeRow(i);

    }
  }
}

void ManageRecordingTracksWindow::on_tabletracks_itemSelectionChanged()
{
  const QModelIndexList selectedrows = ui_.tabletracks->selectionModel()->selectedRows();
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

void ManageRecordingTracksWindow::on_buttonadd_clicked()
{
  ManageRecordingTrackWindow(this, device_, recording_, nullptr).exec();

}

void ManageRecordingTracksWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tabletracks->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const QSharedPointer<client::RecordingTrack> track = recording_->GetTrack(ui_.tabletracks->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toUInt());
  if (!track)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to retrieve track: ") + QString::number(ui_.tabletracks->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toUInt()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageRecordingTrackWindow(this, device_, recording_, track).exec();
}

void ManageRecordingTracksWindow::on_buttonremove_clicked()
{
  const int row = ui_.tabletracks->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove Recording"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const uint id = ui_.tabletracks->item(row, 0)->text().toUInt();
  removetrackconnection_ = device_->RemoveTrack(recording_->GetToken(), id, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVETRACKRESPONSE& removetrackresponse)
  {
    if (removetrackresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removetrackresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
