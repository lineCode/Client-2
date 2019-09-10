// managemapswindow.cpp
//

///// Includes /////

#include "monocleclient/managemapswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/managemapwindow.h"
#include "monocleclient/map.h"
#include "monocleclient/options.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageMapsWindow::ManageMapsWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablemaps->hideColumn(0);
  
  }

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  connect(device.get(), QOverload<const QSharedPointer<client::Map>&>::of(&Device::SignalMapAdded), this, &ManageMapsWindow::MapAdded);
  connect(device.get(), QOverload<const QSharedPointer<client::Map>&>::of(&Device::SignalMapChanged), this, &ManageMapsWindow::MapChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalMapRemoved), this, &ManageMapsWindow::MapRemoved);

  for (const QSharedPointer<client::Map>& map : device_->GetMaps())
  {
    AddMap(map);
  
  }

  on_tablemaps_itemSelectionChanged();
}

ManageMapsWindow::~ManageMapsWindow()
{
  removemapconnection_.Close();

}

void ManageMapsWindow::AddMap(const QSharedPointer<client::Map>& map)
{
  const int row = ui_.tablemaps->rowCount();
  ui_.tablemaps->insertRow(row);
  ui_.tablemaps->setItem(row, 0, new QTableWidgetItem(QString::number(map->GetToken())));
  ui_.tablemaps->item(row, 0)->setData(Qt::UserRole, static_cast<qulonglong>(map->GetToken()));
  ui_.tablemaps->setItem(row, 1, new QTableWidgetItem(map->GetName()));
  ui_.tablemaps->setItem(row, 2, new QTableWidgetItem(map->GetLocation()));
}

void ManageMapsWindow::MapAdded(const QSharedPointer<client::Map>& map)
{
  AddMap(map);

}

void ManageMapsWindow::MapChanged(const QSharedPointer<client::Map>& map)
{
  for (int row = 0; row < ui_.tablemaps->rowCount(); ++row)
  {
    if (ui_.tablemaps->item(row, 0)->data(Qt::UserRole).toULongLong() == map->GetToken())
    {
      ui_.tablemaps->setItem(row, 1, new QTableWidgetItem(map->GetName()));
      ui_.tablemaps->setItem(row, 2, new QTableWidgetItem(map->GetLocation()));
      return;
    }
  }
}

void ManageMapsWindow::MapRemoved(const uint64_t token)
{
  for (int i = (ui_.tablemaps->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablemaps->item(i, 0)->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.tablemaps->removeRow(i);
  
    }
  }
}

void ManageMapsWindow::on_tablemaps_itemSelectionChanged()
{
  const QModelIndexList selectedrows = ui_.tablemaps->selectionModel()->selectedRows();
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

void ManageMapsWindow::on_buttonadd_clicked()
{
  ManageMapWindow(this, device_, boost::none).exec();

}

void ManageMapsWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tablemaps->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No map selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  
  ManageMapWindow(this, device_, ui_.tablemaps->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong()).exec();
}

void ManageMapsWindow::on_buttonremove_clicked()
{
  const int row = ui_.tablemaps->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No map selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  
  if (QMessageBox::question(this, tr("Remove Map"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {
  
    return;
  }
  
  const qulonglong token = ui_.tablemaps->item(row, 0)->text().toULongLong();
  removemapconnection_ = device_->RemoveMap(token, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVEMAPRESPONSE& removemapresponse)
  {
    if (removemapresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removemapresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
