// managefileswindow.cpp
//

///// Includes /////

#include "monocleclient/managefileswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/file.h"
#include "monocleclient/managefilewindow.h"
#include "monocleclient/options.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageFilesWindow::ManageFilesWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);
  
  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablefiles->hideColumn(0);

  }

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(device.get(), QOverload<QSharedPointer<client::File>&>::of(&Device::SignalFileAdded), this, &ManageFilesWindow::FileAdded);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalFileRemoved), this, &ManageFilesWindow::FileRemoved);
  connect(device.get(), QOverload<QSharedPointer<client::File>&, const monocle::FileMonitorState>::of(&Device::SignalFileMonitorStateChanged), this, &ManageFilesWindow::FileMonitorStateChanged);
  connect(device.get(), QOverload<QSharedPointer<client::File>&, const monocle::FileState>::of(&Device::SignalFileStateChanged), this, &ManageFilesWindow::FileStateChanged);

  connect(ui_.tablefiles->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ManageFilesWindow::CurrentRowChanged);

  if (device->IsWindows()) // Windows doesn't have mount points, so we can hide this column
  {
    ui_.tablefiles->hideColumn(2);

  }

  for (const QSharedPointer<client::File>& file : device_->GetFiles())
  {
    AddFile(file);

  }
  UpdateTotalCapacity();
}

ManageFilesWindow::~ManageFilesWindow()
{
  removefileconnection_.Close();
  mountfileconnection_.Close();
  unmountfileconnection_.Close();
}

void ManageFilesWindow::AddFile(const QSharedPointer<client::File>& file)
{
  const int row = ui_.tablefiles->rowCount();
  ui_.tablefiles->insertRow(row);
  ui_.tablefiles->setItem(row, 0, new QTableWidgetItem(QString::number(file->GetToken())));
  ui_.tablefiles->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(file->GetToken()));
  ui_.tablefiles->setItem(row, 1, new QTableWidgetItem(file->GetPath()));
  ui_.tablefiles->setItem(row, 2, new QTableWidgetItem(file->GetMountPoint()));
  ui_.tablefiles->setItem(row, 3, new QTableWidgetItem(QString::number(file->GetNumChunks())));
  ui_.tablefiles->item(row, 3)->setData(Qt::UserRole, static_cast<qulonglong>(file->GetNumChunks()));
  ui_.tablefiles->setItem(row, 4, new QTableWidgetItem(QString::number((file->GetChunkSize() / (1024 * 1024)))));
  ui_.tablefiles->item(row, 4)->setData(Qt::UserRole, static_cast<qulonglong>(file->GetChunkSize()));
  const uint64_t capacitygb = (file->GetNumChunks() * file->GetChunkSize()) / (1024 * 1024 * 1024);
  ui_.tablefiles->setItem(row, 5, new QTableWidgetItem(QString::number(capacitygb)));
  ui_.tablefiles->setItem(row, 6, new QTableWidgetItem(file->GetAutomount() ? "True" : "False"));
  ui_.tablefiles->setItem(row, 7, new QTableWidgetItem(monocle::EnumNameFileState(file->GetState())));
  ui_.tablefiles->item(row, 7)->setData(Qt::UserRole, static_cast<int>(file->GetState()));
  ui_.tablefiles->setItem(row, 8, new QTableWidgetItem(monocle::EnumNameFileMonitorState(file->GetMonitorState())));
  ui_.tablefiles->item(row, 8)->setData(Qt::UserRole, static_cast<int>(file->GetMonitorState()));
  UpdateTotalCapacity();
}

void ManageFilesWindow::UpdateTotalCapacity()
{
  uint64_t totalcapacitygb = 0;
  for (int i = 0; i < ui_.tablefiles->rowCount(); ++i)
  {
    totalcapacitygb += (ui_.tablefiles->item(i, 3)->data(Qt::UserRole).toULongLong() * ui_.tablefiles->item(i, 4)->data(Qt::UserRole).toULongLong()) / (1024 * 1024 * 1024);

  }
  ui_.labeltotalcapacity->setText("Total Capacity(GB): " + QString::number(totalcapacitygb));
}

void ManageFilesWindow::SetEnabled(const bool enabled)
{
  ui_.buttonmount->setEnabled(enabled);
  ui_.buttonunmount->setEnabled(enabled);
  ui_.buttonadd->setEnabled(enabled);
  ui_.buttonremove->setEnabled(enabled);
}

void ManageFilesWindow::FileAdded(QSharedPointer<client::File>& file)
{
  AddFile(file);

}

void ManageFilesWindow::FileRemoved(const uint64_t token)
{
  for (int i = (ui_.tablefiles->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablefiles->item(i, 0)->data(Qt::UserRole) == static_cast<qulonglong>(token))
    {
      ui_.tablefiles->removeRow(i);

    }
  }
  UpdateTotalCapacity();
}

void ManageFilesWindow::FileMonitorStateChanged(QSharedPointer<client::File>& file, const monocle::FileMonitorState monitorstate)
{
  for (int i = (ui_.tablefiles->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablefiles->item(i, 0)->data(Qt::UserRole) == static_cast<qulonglong>(file->GetToken()))
    {
      ui_.tablefiles->item(i, 8)->setText(monocle::EnumNameFileMonitorState(monitorstate));
      ui_.tablefiles->item(i, 8)->setData(Qt::UserRole, static_cast<int>(monitorstate));

      const auto selectedrows = ui_.tablefiles->selectionModel()->selectedRows(); // If the currently selected row has changed, update the button state
      if (selectedrows.size() == 1 && selectedrows[0].row() == i)
      {
        CurrentRowChanged(selectedrows[0], QModelIndex());

      }
    }
  }
}

void ManageFilesWindow::FileStateChanged(QSharedPointer<client::File>& file, const monocle::FileState state)
{
  for (int i = (ui_.tablefiles->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablefiles->item(i, 0)->data(Qt::UserRole) == static_cast<qulonglong>(file->GetToken()))
    {
      ui_.tablefiles->item(i, 7)->setText(monocle::EnumNameFileState(state));
      ui_.tablefiles->item(i, 7)->setData(Qt::UserRole, static_cast<int>(state));

      const auto selectedrows = ui_.tablefiles->selectionModel()->selectedRows(); // If the currently selected row has changed, update the button state
      if (selectedrows.size() == 1 && selectedrows[0].row() == i)
      {
        CurrentRowChanged(selectedrows[0], QModelIndex());

      }
    }
  }
}

void ManageFilesWindow::CurrentRowChanged(const QModelIndex& current, const QModelIndex&)
{
  ui_.buttonremove->setEnabled(current.isValid());
  if (!current.isValid())
  {
    ui_.buttonmount->setEnabled(false);
    ui_.buttonunmount->setEnabled(false);
    return;
  }

  const monocle::FileMonitorState monitorstate = monocle::EnumValuesFileMonitorState()[ui_.tablefiles->item(current.row(), 8)->data(Qt::UserRole).value<int>()];
  if (monitorstate == monocle::FileMonitorState::Unavailable)
  {
    ui_.buttonmount->setEnabled(false);
    ui_.buttonunmount->setEnabled(false);
  }
  else
  {
    const monocle::FileState state = monocle::EnumValuesFileState()[ui_.tablefiles->item(current.row(), 7)->data(Qt::UserRole).value<int>()];
    if (state == monocle::FileState::Mounted)
    {
      ui_.buttonmount->setEnabled(false);
      ui_.buttonunmount->setEnabled(true);
    }
    else if (state == monocle::FileState::Unmounted)
    {
      ui_.buttonmount->setEnabled(true);
      ui_.buttonunmount->setEnabled(false);
    }
    else
    {
      ui_.buttonmount->setEnabled(false);
      ui_.buttonunmount->setEnabled(false);
    }
  }
}

void ManageFilesWindow::on_buttonmount_clicked()
{
  QModelIndexList selectedrows = ui_.tablefiles->selectionModel()->selectedRows(0);
  if (selectedrows.size() != 1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No file selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  SetEnabled(false);
  mountfileconnection_ = device_->MountFile(selectedrows[0].data(Qt::UserRole).toULongLong(), [this](const std::chrono::nanoseconds, const monocle::client::MOUNTFILERESPONSE& mountfileresponse)
  {
    SetEnabled(true);
    if (mountfileresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to mount file"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

void ManageFilesWindow::on_buttonunmount_clicked()
{
  QModelIndexList selectedrows = ui_.tablefiles->selectionModel()->selectedRows(0);
  if (selectedrows.size() != 1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No file selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  SetEnabled(false);
  unmountfileconnection_ = device_->UnmountFile(selectedrows[0].data(Qt::UserRole).toULongLong(), [this](const std::chrono::nanoseconds, const monocle::client::UNMOUNTFILERESPONSE& unmountfileresponse)
  {
    SetEnabled(true);
    if (unmountfileresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to unmount file"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

void ManageFilesWindow::on_buttonadd_clicked()
{
  ManageFileWindow(this, device_).exec();

}

void ManageFilesWindow::on_buttonremove_clicked()
{
  const int row = ui_.tablefiles->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No row selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove File"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const qulonglong token = ui_.tablefiles->item(row, 0)->text().toULongLong();
  ui_.buttonremove->setEnabled(false);
  removefileconnection_ = device_->RemoveFile(token, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVEFILERESPONSE& removefileresponse)
  {
    ui_.buttonremove->setEnabled(true);
    if (removefileresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removefileresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
