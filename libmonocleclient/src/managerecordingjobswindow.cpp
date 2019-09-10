// managerecordingjobswindow.cpp
//

///// Includes /////

#include "monocleclient/managerecordingjobswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/managerecordingjobwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/recording.h"
#include "monocleclient/recordingjob.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageRecordingJobsWindow::ManageRecordingJobsWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording) :
  QDialog(parent),
  device_(device),
  recording_(recording)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablerecordingjobs->hideColumn(0);

  }

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(recording.data(), QOverload<const QSharedPointer<client::RecordingJob>&>::of(&Recording::JobAdded), this, &ManageRecordingJobsWindow::RecordingJobAdded);
  connect(recording.data(), QOverload<const QSharedPointer<client::RecordingJob>&>::of(&Recording::JobChanged), this, &ManageRecordingJobsWindow::RecordingJobChanged);
  connect(recording.data(), QOverload<const uint64_t>::of(&Recording::JobRemoved), this, &ManageRecordingJobsWindow::RecordingJobRemoved);

  for (const QSharedPointer<client::RecordingJob>& recordingjobs : recording_->GetJobs())
  {
    AddRecordingJob(recordingjobs);

  }

  on_tablerecordingjobs_itemSelectionChanged();
}

ManageRecordingJobsWindow::~ManageRecordingJobsWindow()
{
  removerecordingjobconnection_.Close();

}

void ManageRecordingJobsWindow::AddRecordingJob(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  const int row = ui_.tablerecordingjobs->rowCount();
  ui_.tablerecordingjobs->insertRow(row);
  ui_.tablerecordingjobs->setItem(row, 0, new QTableWidgetItem(QString::number(recordingjob->GetToken())));
  ui_.tablerecordingjobs->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(recordingjob->GetToken()));
  ui_.tablerecordingjobs->setItem(row, 1, new QTableWidgetItem(recordingjob->GetName()));
  ui_.tablerecordingjobs->setItem(row, 2, new QTableWidgetItem(QString::number(recordingjob->GetPriority())));
  ui_.tablerecordingjobs->setItem(row, 3, new QTableWidgetItem(recordingjob->GetEnabled() ? "Enabled" : "Disabled"));
  ui_.tablerecordingjobs->item(row, 3)->setData(Qt::UserRole, static_cast<qulonglong>(recordingjob->GetEnabled()));
}

void ManageRecordingJobsWindow::RecordingJobAdded(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  AddRecordingJob(recordingjob);

}

void ManageRecordingJobsWindow::RecordingJobChanged(const QSharedPointer<client::RecordingJob>& recordingjob)
{
  for (int row = 0; row < ui_.tablerecordingjobs->rowCount(); ++row)
  {
    if (ui_.tablerecordingjobs->item(row, 0)->data(Qt::UserRole).toUInt() == recordingjob->GetToken())
    {
      ui_.tablerecordingjobs->item(row, 1)->setText(recordingjob->GetName());
      ui_.tablerecordingjobs->item(row, 2)->setText(QString::number(recordingjob->GetPriority()));
      ui_.tablerecordingjobs->item(row, 3)->setText(recordingjob->GetEnabled() ? "Enabled" : "Disabled");
      ui_.tablerecordingjobs->item(row, 3)->setData(Qt::UserRole, static_cast<qulonglong>(recordingjob->GetEnabled()));
    }
  }
}

void ManageRecordingJobsWindow::RecordingJobRemoved(const uint64_t token)
{
  for (int i = (ui_.tablerecordingjobs->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablerecordingjobs->item(i, 0)->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.tablerecordingjobs->removeRow(i);

    }
  }
}

void ManageRecordingJobsWindow::on_tablerecordingjobs_itemSelectionChanged()
{
  const QModelIndexList selectedrows = ui_.tablerecordingjobs->selectionModel()->selectedRows();
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

void ManageRecordingJobsWindow::on_buttonadd_clicked()
{
  ManageRecordingJobWindow(this, device_, recording_, nullptr, nullptr).exec();

}

void ManageRecordingJobsWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tablerecordingjobs->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording job selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  const QSharedPointer<client::RecordingJob> recordingjob = recording_->GetJob(ui_.tablerecordingjobs->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong());
  if (!recordingjob)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Unable to retrieve recording job: ") + QString::number(ui_.tablerecordingjobs->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toUInt()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageRecordingJobWindow(this, device_, recording_, recordingjob, nullptr).exec();
}

void ManageRecordingJobsWindow::on_buttonremove_clicked()
{
  const int row = ui_.tablerecordingjobs->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No recording job selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove Recording Job"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  removerecordingjobconnection_ = device_->RemoveRecordingJob(recording_->GetToken(), ui_.tablerecordingjobs->item(row, 0)->text().toULongLong(), [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVERECORDINGJOBRESPONSE& removerecordingjobresponse)
  {
    if (removerecordingjobresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removerecordingjobresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
