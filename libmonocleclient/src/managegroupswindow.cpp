// managegroupswindow.cpp
//

///// Includes /////

#include "monocleclient/managegroupswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/group.h"
#include "monocleclient/managegroupwindow.h"
#include "monocleclient/options.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageGroupsWindow::ManageGroupsWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tablegroups->hideColumn(0);

  }

  connect(device.get(), QOverload<QSharedPointer<Group>&>::of(&Device::SignalGroupAdded), this, &ManageGroupsWindow::GroupAdded);
  connect(device.get(), QOverload<QSharedPointer<Group>&>::of(&Device::SignalGroupChanged), this, &ManageGroupsWindow::GroupChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalGroupRemoved), this, &ManageGroupsWindow::GroupRemoved);
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  
  for (const QSharedPointer<Group>& group : device_->GetGroups())
  {
    AddGroup(group);

  }
}

ManageGroupsWindow::~ManageGroupsWindow()
{
  removegroupconnection_.Close();

}

void ManageGroupsWindow::AddGroup(const QSharedPointer<Group>& group)
{
  const int row = ui_.tablegroups->rowCount();
  ui_.tablegroups->insertRow(row);
  QTableWidgetItem* tokenitem = new QTableWidgetItem(QString::number(group->GetToken()));
  tokenitem->setData(Qt::UserRole, static_cast<qulonglong>(group->GetToken()));
  ui_.tablegroups->setItem(row, 0, tokenitem);
  ui_.tablegroups->setItem(row, 1, new QTableWidgetItem(group->GetName()));
}

void ManageGroupsWindow::GroupAdded(const QSharedPointer<Group>& group)
{
  AddGroup(group);

}

void ManageGroupsWindow::GroupChanged(const QSharedPointer<Group>& group)
{
  for (int i = 0; i < ui_.tablegroups->rowCount(); ++i)
  {
    if (ui_.tablegroups->item(i, 0)->data(Qt::UserRole).toULongLong() == group->GetToken())
    {
      ui_.tablegroups->item(i, 1)->setText(group->GetName());

    }
  }
}

void ManageGroupsWindow::GroupRemoved(const uint64_t token)
{
  for (int i = (ui_.tablegroups->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tablegroups->item(i, 0)->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.tablegroups->removeRow(i);

    }
  }
}

void ManageGroupsWindow::on_buttonadd_clicked()
{
  ManageGroupWindow(this, device_, boost::none).exec();

}

void ManageGroupsWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tablegroups->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No group selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageGroupWindow(this,
                    device_,
                    ui_.tablegroups->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong()).exec();
}

void ManageGroupsWindow::on_buttonremove_clicked()
{
  const int row = ui_.tablegroups->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No group selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove Group"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const qulonglong token = ui_.tablegroups->item(row, 0)->text().toULongLong();
  removegroupconnection_ = device_->RemoveGroup(token, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVEGROUPRESPONSE& removegroupresponse)
  {
    if (removegroupresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removegroupresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
