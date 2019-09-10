// manageuserswindow.cpp
//

///// Includes /////

#include "monocleclient/manageuserswindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/group.h"
#include "monocleclient/manageuserwindow.h"
#include "monocleclient/options.h"
#include "monocleclient/user.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageUsersWindow::ManageUsersWindow(QWidget* parent, boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

  if (!Options::Instance().GetShowTokens())
  {
    ui_.tableusers->hideColumn(0);

  }

  connect(device.get(), QOverload<QSharedPointer<Group>&>::of(&Device::SignalGroupChanged), this, &ManageUsersWindow::GroupChanged);
  connect(device.get(), QOverload<QSharedPointer<User>&>::of(&Device::SignalUserAdded), this, &ManageUsersWindow::UserAdded);
  connect(device.get(), QOverload<QSharedPointer<User>&>::of(&Device::SignalUserChanged), this, &ManageUsersWindow::UserChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalUserRemoved), this, &ManageUsersWindow::UserRemoved);
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::accept);
  
  for (const QSharedPointer<User>& user : device_->GetUsers())
  {
    AddUser(user);

  }
}

ManageUsersWindow::~ManageUsersWindow()
{
  removeuserconnection_.Close();

}

void ManageUsersWindow::AddUser(const QSharedPointer<User>& user)
{
  const int row = ui_.tableusers->rowCount();
  ui_.tableusers->insertRow(row);
  QTableWidgetItem* token = new QTableWidgetItem(QString::number(user->GetToken()));
  token->setData(Qt::UserRole, static_cast<qulonglong>(user->GetToken()));
  ui_.tableusers->setItem(row, 0, token);
  ui_.tableusers->setItem(row, 1, new QTableWidgetItem(user->GetUsername()));

  QSharedPointer<Group> group = device_->GetGroup(user->GetGroupToken());
  if (group)
  {
    QTableWidgetItem* item = new QTableWidgetItem(group->GetName());
    item->setData(Qt::UserRole, static_cast<qulonglong>(user->GetGroupToken()));
    ui_.tableusers->setItem(row, 2, item);
  }
  else
  {
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(user->GetGroupToken()));
    item->setData(Qt::UserRole, static_cast<qulonglong>(user->GetGroupToken()));
    ui_.tableusers->setItem(row, 2, new QTableWidgetItem(user->GetGroupToken()));
  }
}

void ManageUsersWindow::GroupChanged(const QSharedPointer<Group>& group)
{
  for (int i = (ui_.tableusers->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tableusers->item(i, 2)->data(Qt::UserRole).toULongLong() == group->GetToken())
    {
      ui_.tableusers->item(i, 2)->setText(group->GetName());

    }
  }
}

void ManageUsersWindow::UserAdded(QSharedPointer<User>& user)
{
  AddUser(user);

}

void ManageUsersWindow::UserChanged(QSharedPointer<User>& user)
{
  for (int i = (ui_.tableusers->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tableusers->item(i, 0)->data(Qt::UserRole).toULongLong() == user->GetToken())
    {
      ui_.tableusers->item(i, 1)->setText(user->GetUsername());

      QSharedPointer<Group> group = device_->GetGroup(user->GetGroupToken());
      if (group)
      {
        ui_.tableusers->setItem(i, 2, new QTableWidgetItem(group->GetName()));

      }
      else
      {
        ui_.tableusers->setItem(i, 2, new QTableWidgetItem(user->GetGroupToken()));

      }
    }
  }
}

void ManageUsersWindow::UserRemoved(const uint64_t token)
{
  for (int i = (ui_.tableusers->rowCount() - 1); i >= 0; --i)
  {
    if (ui_.tableusers->item(i, 0)->data(Qt::UserRole) == static_cast<qulonglong>(token))
    {
      ui_.tableusers->removeRow(i);

    }
  }
}

void ManageUsersWindow::on_buttonadd_clicked()
{
  ManageUserWindow(this, device_, boost::none).exec();

}

void ManageUsersWindow::on_buttonedit_clicked()
{
  const QModelIndexList selectedrows = ui_.tableusers->selectionModel()->selectedRows();
  if (selectedrows.empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No user selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  ManageUserWindow(this,
                   device_,
                   ui_.tableusers->item(selectedrows.at(0).row(), 0)->data(Qt::UserRole).toULongLong()).exec();
}

void ManageUsersWindow::on_buttonremove_clicked()
{
  const int row = ui_.tableusers->currentRow();
  if (row == -1)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No user selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove User"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
  {

    return;
  }

  const qulonglong token = ui_.tableusers->item(row, 0)->text().toULongLong();
  removeuserconnection_ = device_->RemoveUser(token, [this](const std::chrono::steady_clock::duration latency, const monocle::client::REMOVEUSERRESPONSE& removeuserresponse)
  {
    if (removeuserresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Remove failed: ") + QString::fromStdString(removeuserresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  });
}

}

QT_END_NAMESPACE
