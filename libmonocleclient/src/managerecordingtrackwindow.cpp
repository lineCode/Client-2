// managerecordingtrackwindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingtrackwindow.h"

#include <QMessageBox>
#include <QUrl>

#include "monocleclient/device.h"
#include "monocleclient/file.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingtrack.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingTrackWindow::ManageRecordingTrackWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track) :
  QDialog(parent),
  device_(device),
  recording_(recording),
  track_(track)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  connect(device_.get(), QOverload<QSharedPointer<client::File>&>::of(&Device::SignalFileAdded), this, &ManageRecordingTrackWindow::FileAdded);
  connect(device_.get(), QOverload<const uint64_t>::of(&Device::SignalFileRemoved), this, &ManageRecordingTrackWindow::FileRemoved);

  if (track)
  {
    ui_.combotracktype->addItem(monocle::EnumNameTrackType(static_cast<monocle::TrackType>(track->GetTrackType())), static_cast<int>(track->GetTrackType()));

  }
  else
  {
    for (int i = static_cast<int>(monocle::TrackType::MIN); i <= static_cast<int>(monocle::TrackType::MAX); ++i)
    {
      ui_.combotracktype->addItem(monocle::EnumNameTrackType(static_cast<monocle::TrackType>(i)), i);

    }
  }

  if (track)
  {
    connect(recording_.get(), QOverload<const QSharedPointer<client::RecordingTrack>&>::of(&Recording::TrackChanged), this, &ManageRecordingTrackWindow::TrackChanged);
    connect(recording_.get(), QOverload<const uint32_t>::of(&Recording::TrackRemoved), this, &ManageRecordingTrackWindow::TrackRemoved);

    Setup(track_);

    setWindowTitle("Edit Track");
  }
  else
  {
    for (const QSharedPointer<client::File>& file : device_->GetFiles())
    {
      AddFile(file);

    }
  }

  on_checkfixedfiles_toggled(true);
}

ManageRecordingTrackWindow::~ManageRecordingTrackWindow()
{
  trackconnection_.Close();

}

void ManageRecordingTrackWindow::AddFile(const QSharedPointer<client::File>& file)
{
  QListWidgetItem* item = new QListWidgetItem(file->GetPath());
  item->setData(Qt::UserRole, static_cast<qulonglong>(file->GetToken()));
  ui_.listavailablerecordings->addItem(item);
}

void ManageRecordingTrackWindow::SetEnabled(const bool enabled)
{
  ui_.editdescription->setEnabled(enabled);
  ui_.checkdigitalsigning->setEnabled(enabled);
  ui_.checkencrypt->setEnabled(enabled);
  ui_.checkfixedfiles->setEnabled(enabled);
  ui_.buttonright->setEnabled(enabled);
  ui_.buttonleft->setEnabled(enabled);
  ui_.listavailablerecordings->setEnabled(enabled);
  ui_.listselectedrecordings->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageRecordingTrackWindow::Setup(const QSharedPointer<client::RecordingTrack>& track)
{
  ui_.editdescription->setText(track->GetDescription());
  ui_.checkdigitalsigning->setChecked(track->GetDigitalSigning());
  ui_.checkencrypt->setChecked(track->GetEncrypt());
  ui_.spinflushfrequency->setValue(track->GetFlushFrequency());
  ui_.checkfixedfiles->setChecked(track->GetFixedFiles());
  ui_.listavailablerecordings->clear();
  ui_.listselectedrecordings->clear();
  for (const QSharedPointer<client::File>& file : device_->GetFiles())
  {
    QListWidgetItem* item = new QListWidgetItem(file->GetPath());
    item->setData(Qt::UserRole, static_cast<qulonglong>(file->GetToken()));
    if (track_->HasFile(file->GetToken()))
    {
      ui_.listselectedrecordings->addItem(item);

    }
    else
    {
      ui_.listselectedrecordings->addItem(item);

    }
  }
}

void ManageRecordingTrackWindow::FileAdded(const QSharedPointer<client::File>& file)
{
  AddFile(file);

}

void ManageRecordingTrackWindow::FileRemoved(const uint64_t token)
{
  for (int i = ui_.listavailablerecordings->count(); i >= 0; --i)
  {
    QListWidgetItem* item = ui_.listavailablerecordings->item(i);
    if (item->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.listavailablerecordings->removeItemWidget(item);

    }
  }

  for (int i = ui_.listselectedrecordings->count(); i >= 0; --i)
  {
    QListWidgetItem* item = ui_.listselectedrecordings->item(i);
    if (item->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.listselectedrecordings->removeItemWidget(item);

    }
  }
}

void ManageRecordingTrackWindow::TrackChanged(const QSharedPointer<client::RecordingTrack>& track)
{
  if (track_ == track)
  {
    if (QMessageBox::question(this, tr("Track Changed"), tr("Track has been changed, Would you like to update with new values?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
      Setup(track);

    }
  }
}

void ManageRecordingTrackWindow::TrackRemoved(const uint32_t id)
{
  if (track_ && (track_->GetId() == id))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Recording has been removed"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    reject();
  }
}

void ManageRecordingTrackWindow::on_checkfixedfiles_toggled(bool)
{
  ui_.listavailablerecordings->setEnabled(ui_.checkfixedfiles->isChecked());
  ui_.listselectedrecordings->setEnabled(ui_.checkfixedfiles->isChecked());
  ui_.buttonright->setEnabled(ui_.checkfixedfiles->isChecked());
  ui_.buttonleft->setEnabled(ui_.checkfixedfiles->isChecked());
}

void ManageRecordingTrackWindow::on_buttonright_clicked()
{
  auto items = ui_.listavailablerecordings->selectedItems();
  for (const QListWidgetItem* item : items)
  {
    ui_.listselectedrecordings->addItem(ui_.listavailablerecordings->takeItem(ui_.listavailablerecordings->row(item)));

  }
}

void ManageRecordingTrackWindow::on_buttonleft_clicked()
{
  auto items = ui_.listselectedrecordings->selectedItems();
  for (const QListWidgetItem* item : items)
  {
    ui_.listavailablerecordings->addItem(ui_.listselectedrecordings->takeItem(ui_.listselectedrecordings->row(item)));

  }
}

void ManageRecordingTrackWindow::on_buttonok_clicked()
{
  if (ui_.editdescription->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a name for this Track"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  std::vector<uint64_t> files;
  files.reserve(ui_.listselectedrecordings->count());
  for (int i = 0; i < ui_.listselectedrecordings->count(); ++i)
  {
    files.push_back(ui_.listselectedrecordings->item(i)->data(Qt::UserRole).toULongLong());

  }

  if (ui_.checkfixedfiles->isChecked() && files.empty())
  {
    if (QMessageBox::question(this, tr("Warning"), tr("Track will record to a fixed set of files but no files are currently selected, are you sure you want to continue?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {

      return;
    }
  }

  SetEnabled(false);
  if (track_)
  {
    trackconnection_ = device_->ChangeTrack(recording_->GetToken(), track_->GetId(), static_cast<monocle::TrackType>(ui_.combotracktype->currentData(Qt::UserRole).toInt()), ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), files, [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGETRACKRESPONSE& changetrackresponse)
    {
      SetEnabled(true);
      if (changetrackresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changetrackresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
  else
  {
    trackconnection_ = device_->AddTrack(recording_->GetToken(), static_cast<monocle::TrackType>(ui_.combotracktype->currentData(Qt::UserRole).toInt()), ui_.editdescription->text().toStdString(), ui_.checkfixedfiles->isChecked(), ui_.checkdigitalsigning->isChecked(), ui_.checkencrypt->isChecked(), ui_.spinflushfrequency->value(), files, [this](const std::chrono::nanoseconds latency, const monocle::client::ADDTRACKRESPONSE& addtrackresponse)
    {
      SetEnabled(true);
      if (addtrackresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(addtrackresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
