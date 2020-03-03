// managerecordingswindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/managerecordingwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/recording.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingsWindow::ManageRecordingsWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablerecordings->hideColumn(0);

  }

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(device.get(), QOverload<QSharedPointer<client::Recording>&>::of(&Device::SignalRecordingAdded), this, &ManageRecordingsWindow::RecordingAdded);
  connect(device.get(), QOverload<QSharedPointer<client::Recording>&>::of(&Device::SignalRecordingChanged), this, &ManageRecordingsWindow::RecordingChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalRecordingRemoved), this, &ManageRecordingsWindow::RecordingRemoved);

  for (const QSharedPointer<client::Recording>& recording : device_->GetRecordings())
  {
    AddRecording(recording);

  }

  on_tablerecordings_itemSelectionChanged();
}

ManageRecordingsWindow::~ManageRecordingsWindow()
{
  removerecordingconnection_.Close();

}

void ManageRecordingsWindow::AddRecording(const QSharedPointer<client::Recording>& recording)
{
  const int row = ui_.tablerecordings->rowCount();
  ui_.tablerecordings->insertRow(row);
  ui_.tablerecordings->setItem(row, 0, new QTableWidgetItem(QString::number(recording->GetToken())));
  ui_.tablerecordings->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(recording->GetToken()));
  ui_.tablerecordings->setItem(row, 1, new QTableWidgetItem(recording->GetName()));
  ui_.tablerecordings->setItem(row, 2, new QTableWidgetItem(ToString(recording->GetRetentionTime())));
}

QString ManageRecordingsWindow::ToString(const uint64_t retentiontime) const
{
  if (retentiontime == 0)
  {

    return QString("No Limit");
  }

  const uint64_t totalseconds = (retentiontime / 1000);
  const uint64_t days = totalseconds / 86400;
  const uint64_t hours = (totalseconds - (days * 86400)) / 24;

  if ((days == 0) && (hours == 0))
  {
    if (totalseconds == 0)
    {

      return QString::number(totalseconds) + "seconds";
    }
    else
    {

      return QString::number(retentiontime) + "milliseconds";
    }
  }
  else
  {
    if (hours == 0)
    {

      return QString::number(days) + "days";
    }
    else
    {

      return QString::number(days) + "days " + QString::number(hours) + "hours";
    }
  }
}

void ManageRecordingsWindow::RecordingAdded(QSharedPointer<client::Recording>& recording)
{
  AddRecording(recording);

}

void ManageRecordingsWindow::RecordingChanged(QSharedPointer<client::Recording>& recording)
{
  for (int row = 0; row < ui_.tablerecordings->rowCount(); ++row)
  {
    if (ui_.tablerecordings->item(row, 0)->data(Qt::UserRole).toULongLong() == recording->GetToken())
    {
      ui_.tablerecordings->setItem(row, 1, new QTableWidgetItem(recording->GetName()));
      ui_.tablerecordings->setItem(row, 2, new QTableWidgetItem(ToString(recording->GetRetentionTime())));
      return;
    }
  }
}

void ManageRecordingsWindow::RecordingRemoved(const uint64_t token)
{
  for (int i = (ui_.tablerecordings->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablerecordings->item(i, 0)->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.tablerecordings->removeRow(i);

    }
  }
}

void ManageRecordingsWindow::on_tablerecordings_itemSelectionChanged()
{
  const QModelIndexList selectedrows = ui_.tablerecordings->selectionModel()->selectedRows();
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

void ManageRecordingsWindow::on_buttonadd_clicked()
{
  if (device_->GetRecordings().size() >= device_->GetMaxRecordings())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Max recordings reached"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageRecordingWindow(this, device_, boost::none).exec();
}

void ManageRecordingsWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tablerecordings->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageRecordingWindow(this, device_, ui_.tablerecordings->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong()).exec();
}

void ManageRecordingsWindow::on_buttonremove_clicked()
{
  const int row = ui_.tablerecordings->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove Recording"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const qulonglong token = ui_.tablerecordings->item(row, 0)->text().toULongLong();
  removerecordingconnection_ = device_->RemoveRecording(token, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVERECORDINGRESPONSE& removerecordingresponse)
  {
    if (removerecordingresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removerecordingresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
