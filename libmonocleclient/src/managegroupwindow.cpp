// managegroupwindow.cpp
//

///// Includes /////

#include "monocleclient/managegroupwindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/group.h"
#include "monocleclient/recording.h"
#include "monocleclient/user.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageGroupWindow::ManageGroupWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token) :
  QDialog(parent),
  device_(device),
  token_(token)
{
  ui_.setupUi(this);

  connect(device.get(), &Device::SignalRecordingAdded, this, &ManageGroupWindow::RecordingAdded);
  connect(device.get(), &Device::SignalRecordingChanged, this, &ManageGroupWindow::RecordingChanged);
  connect(device.get(), &Device::SignalRecordingRemoved, this, &ManageGroupWindow::RecordingRemoved);
  connect(device.get(), QOverload<QSharedPointer<User>&>::of(&Device::SignalUserAdded), this, &ManageGroupWindow::UserAdded);
  connect(device.get(), QOverload<QSharedPointer<User>&>::of(&Device::SignalUserChanged), this, &ManageGroupWindow::UserChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalUserRemoved), this, &ManageGroupWindow::UserRemoved);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  if (token.is_initialized())
  {
    connect(device.get(), QOverload<QSharedPointer<Group>&>::of(&Device::SignalGroupChanged), this, &ManageGroupWindow::GroupChanged);
    connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalGroupRemoved), this, &ManageGroupWindow::GroupRemoved);

    QSharedPointer<Group> group = device_->GetGroup(*token);
    if (group == nullptr)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Group does not exist: ") + *token, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    ui_.editname->setText(group->GetName());
    ui_.checkmanageusers->setChecked(group->GetManageUsers());
    ui_.checkmanagerecordings->setChecked(group->GetManageRecordings());
    ui_.checkmanagemaps->setChecked(group->GetManageMaps());
    ui_.checkmanagedevice->setChecked(group->GetManageDevice());
    ui_.checkmanagerecordings->setChecked(group->GetAllRecordings());

    for (const QSharedPointer<client::Recording>& recording : device_->GetRecordings())
    {
      QListWidgetItem* item = new QListWidgetItem(recording->GetName());
      item->setData(Qt::UserRole, static_cast<qulonglong>(recording->GetToken()));
      if (std::find_if(group->GetRecordings().cbegin(), group->GetRecordings().cend(), [&recording](const uint64_t token) { return (token == recording->GetToken()); }) == group->GetRecordings().cend())
      {
        ui_.listavailablerecordings->addItem(item);

      }
      else
      {
        ui_.listselectedrecordings->addItem(item);

      }
    }

    for (const QSharedPointer<User>& user : device_->GetUsers())
    {
      if (user->GetGroupToken() == *token)
      {
        QListWidgetItem* item = new QListWidgetItem(user->GetUsername());
        item->setData(Qt::UserRole, static_cast<qulonglong>(user->GetToken()));
        ui_.listusers->addItem(item);
      }
    }

    setWindowTitle("Edit Group: " + group->GetName());
  }
  else
  {
    for (const QSharedPointer<client::Recording>& recording : device_->GetRecordings())
    {
      QListWidgetItem* item = new QListWidgetItem(recording->GetName());
      item->setData(Qt::UserRole, static_cast<qulonglong>(recording->GetToken()));
      ui_.listavailablerecordings->addItem(item);
    }
  }

  if (!device->SupportsMaps())
  {
    ui_.checkmanagemaps->setEnabled(false);
    ui_.checkmanagemaps->setToolTip("Server does not support maps");
  }

  if (!device->SupportsManageDevice())
  {
    ui_.checkmanagedevice->setEnabled(false);
    ui_.checkmanagedevice->setToolTip("Server does not support device management");
  }
}

ManageGroupWindow::~ManageGroupWindow()
{
  groupconnection_.Close();

}

void ManageGroupWindow::SetEnabled(const bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.checkmanageusers->setEnabled(enabled);
  ui_.checkmanagerecordings->setEnabled(enabled);
  ui_.checkmanagemaps->setEnabled(enabled);
  ui_.checkmanagedevice->setEnabled(enabled);
  ui_.checkallrecordings->setEnabled(enabled);
  ui_.listavailablerecordings->setEnabled(enabled);
  ui_.listselectedrecordings->setEnabled(enabled);
  ui_.buttonright->setEnabled(enabled);
  ui_.buttonleft->setEnabled(enabled);
  ui_.listusers->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageGroupWindow::RecordingAdded(const QSharedPointer<client::Recording>& recording)
{
  if (token_.is_initialized())
  {
    QSharedPointer<Group> group = device_->GetGroup(*token_);
    if (group == nullptr)
    {

      return;
    }

    QListWidgetItem* item = new QListWidgetItem(recording->GetName());
    item->setData(Qt::UserRole, static_cast<qulonglong>(recording->GetToken()));
    if (std::find_if(group->GetRecordings().cbegin(), group->GetRecordings().cend(), [&recording](const uint64_t token) { return (token == recording->GetToken()); }) == group->GetRecordings().cend())
    {
      ui_.listavailablerecordings->addItem(item);

    }
    else
    {
      ui_.listselectedrecordings->addItem(item);

    }
  }
  else
  {
    QListWidgetItem* item = new QListWidgetItem(recording->GetName());
    item->setData(Qt::UserRole, static_cast<qulonglong>(recording->GetToken()));
    ui_.listavailablerecordings->addItem(item);
  }
}

void ManageGroupWindow::RecordingChanged(const QSharedPointer<client::Recording>& recording)
{
  for (int i = 0; i < ui_.listavailablerecordings->count(); ++i)
  {
    QListWidgetItem* item = ui_.listavailablerecordings->item(i);
    if (item->data(Qt::UserRole).toULongLong() == recording->GetToken())
    {
      item->setText(recording->GetName());

    }
  }

  for (int i = 0; i < ui_.listselectedrecordings->count(); ++i)
  {
    QListWidgetItem* item = ui_.listselectedrecordings->item(i);
    if (item->data(Qt::UserRole).toULongLong() == recording->GetToken())
    {
      item->setText(recording->GetName());

    }
  }
}

void ManageGroupWindow::RecordingRemoved(const uint64_t token)
{
  for (int i = (ui_.listavailablerecordings->count() - 1); i >= 0; --i)
  {
    QListWidgetItem* item = ui_.listavailablerecordings->item(i);
    if (item->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.listavailablerecordings->removeItemWidget(item);

    }
  }

  for (int i = (ui_.listselectedrecordings->count() - 1); i >= 0; --i)
  {
    QListWidgetItem* item = ui_.listselectedrecordings->item(i);
    if (item->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.listselectedrecordings->removeItemWidget(item);

    }
  }
}

void ManageGroupWindow::UserAdded(QSharedPointer<User>& user)
{
  QListWidgetItem* item = new QListWidgetItem(user->GetUsername());
  item->setData(Qt::UserRole, static_cast<qulonglong>(user->GetToken()));
  ui_.listusers->addItem(item);
}

void ManageGroupWindow::UserChanged(QSharedPointer<User>& user)
{
  for (int i = (ui_.listusers->count() - 1); i >= 0; --i)
  {
    if (ui_.listusers->item(i)->data(Qt::UserRole).toULongLong() == user->GetToken())
    {
      ui_.listusers->item(i)->setText(user->GetUsername());

    }
  }
}

void ManageGroupWindow::UserRemoved(const uint64_t token)
{
  for (int i = (ui_.listusers->count() - 1); i >= 0; --i)
  {
    if (ui_.listusers->item(i)->data(Qt::UserRole).toULongLong() == token)
    {
      ui_.listusers->removeItemWidget(ui_.listusers->item(i));

    }
  }
}

void ManageGroupWindow::GroupChanged(const QSharedPointer<Group>& group)
{
  if (group->GetToken() != *token_)
  {

    return;
  }

  if (QMessageBox::question(this, tr("Warning"), tr("Group has changed, would you like to update?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    ui_.editname->setText(group->GetName());
    ui_.checkmanageusers->setChecked(group->GetManageUsers());
    ui_.checkmanagerecordings->setChecked(group->GetManageRecordings());
    ui_.checkmanagemaps->setChecked(group->GetManageMaps());
    ui_.checkmanagerecordings->setChecked(group->GetAllRecordings());

    ui_.listavailablerecordings->clear();
    ui_.listselectedrecordings->clear();
    for (const QSharedPointer<client::Recording>& recording : device_->GetRecordings())
    {
      QListWidgetItem* item = new QListWidgetItem(recording->GetName());
      item->setData(Qt::UserRole, static_cast<qulonglong>(recording->GetToken()));
      if (std::find_if(group->GetRecordings().cbegin(), group->GetRecordings().cend(), [&recording](const uint64_t token) { return (token == recording->GetToken()); }) == group->GetRecordings().cend())
      {
        ui_.listavailablerecordings->addItem(item);

      }
      else
      {
        ui_.listselectedrecordings->addItem(item);

      }
    }

    ui_.listusers->clear();
    for (const QSharedPointer<User>& user : device_->GetUsers())
    {
      if (user->GetGroupToken() == *token_)
      {
        QListWidgetItem* item = new QListWidgetItem(user->GetUsername());
        item->setData(Qt::UserRole, static_cast<qulonglong>(user->GetToken()));
        ui_.listusers->addItem(item);
      }
    }
  }
}

void ManageGroupWindow::GroupRemoved(const uint64_t token)
{
  if (token != *token_)
  {

    return;
  }

  QMessageBox(QMessageBox::Warning, tr("Error"), tr("Group has been deleted"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
  reject();
}

void ManageGroupWindow::on_checkallrecordings_stateChanged(int state)
{
  bool enabled = false;
  if (state == Qt::Unchecked)
  {
    enabled = false;

  }
  else if (state == Qt::Checked)
  {
    enabled = true;

  }
  else
  {

    return;
  }

  ui_.listavailablerecordings->setEnabled(enabled);
  ui_.listselectedrecordings->setEnabled(enabled);
  ui_.buttonright->setEnabled(enabled);
  ui_.buttonleft->setEnabled(enabled);
}

void ManageGroupWindow::on_buttonright_clicked()
{
  for (QListWidgetItem* selecteditem : ui_.listavailablerecordings->selectedItems())
  {
    ui_.listavailablerecordings->takeItem(ui_.listavailablerecordings->row(selecteditem));
    ui_.listselectedrecordings->addItem(selecteditem);
  }
}

void ManageGroupWindow::on_buttonleft_clicked()
{
  for (QListWidgetItem* selecteditem : ui_.listselectedrecordings->selectedItems())
  {
    ui_.listselectedrecordings->takeItem(ui_.listselectedrecordings->row(selecteditem));
    ui_.listavailablerecordings->addItem(selecteditem);
  }
}

void ManageGroupWindow::on_buttonok_clicked()
{
  if (ui_.editname->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a name"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  std::vector<uint64_t> recordings;
  recordings.reserve(ui_.listselectedrecordings->count());
  for (int i = 0; i < ui_.listselectedrecordings->count(); ++i)
  {
    recordings.push_back(ui_.listselectedrecordings->item(i)->data(Qt::UserRole).toULongLong());

  }

  bool managemaps = ui_.checkmanagemaps->isChecked();
  if (!device_->SupportsMaps())
  {
    managemaps = false;

  }

  bool managedevice = ui_.checkmanagedevice->isChecked();
  if (!device_->SupportsManageDevice())
  {
    managedevice = false;

  }

  if (token_.is_initialized())
  {
    for (const QSharedPointer<Group>& group : device_->GetGroups())
    {
      if (group->GetToken() == *token_)
      {

        continue;
      }

      if (group->GetName() == ui_.editname->text())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Group name already exists"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    }

    SetEnabled(false);
    groupconnection_ = device_->ChangeGroup(*token_, ui_.editname->text().toStdString(), ui_.checkmanageusers->isChecked(), ui_.checkmanagerecordings->isChecked(), managemaps, managedevice, ui_.checkallrecordings->isChecked(), recordings, [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGEGROUPRESPONSE& changegroupresponse)
    {
      SetEnabled(true);
      if (changegroupresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changegroupresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
  else
  {
    for (const QSharedPointer<Group>& group : device_->GetGroups())
    {
      if (group->GetName() == ui_.editname->text())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Group name already exists"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    }

    SetEnabled(false);
    groupconnection_ = device_->AddGroup(ui_.editname->text().toStdString(), ui_.checkmanageusers->isChecked(), ui_.checkmanagerecordings->isChecked(), managemaps, managedevice, ui_.checkallrecordings->isChecked(), recordings, [this](const std::chrono::nanoseconds latency, const monocle::client::ADDGROUPRESPONSE& addgroupresponse)
    {
      SetEnabled(true);
      if (addgroupresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(addgroupresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
