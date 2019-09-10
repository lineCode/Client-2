// manageuserwindow.cpp
//

///// Includes /////

#include "monocleclient/manageuserwindow.h"

#include <QMessageBox>

#include "monocleclient/device.h"
#include "monocleclient/group.h"
#include "monocleclient/user.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageUserWindow::ManageUserWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token) :
  QDialog(parent),
  device_(device),
  token_(token),
  passworddirty_(false)
{
  ui_.setupUi(this);

  connect(device.get(), QOverload<QSharedPointer<Group>&>::of(&Device::SignalGroupAdded), this, &ManageUserWindow::GroupAdded);
  connect(device.get(), QOverload<QSharedPointer<Group>&>::of(&Device::SignalGroupChanged), this, &ManageUserWindow::GroupChanged);
  connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalGroupRemoved), this, &ManageUserWindow::GroupRemoved);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  if (device_->GetGroups().empty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("No groups available, please add a group before continuing"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  for (const QSharedPointer<Group>& group : device_->GetGroups())
  {
    ui_.combogroup->addItem(group->GetName(), static_cast<qulonglong>(group->GetToken()));

  }

  if (token.is_initialized())
  {
    connect(device.get(), QOverload<QSharedPointer<User>&>::of(&Device::SignalUserChanged), this, &ManageUserWindow::UserChanged);
    connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalUserRemoved), this, &ManageUserWindow::UserRemoved);

    QSharedPointer<User> user = device_->GetUser(*token);
    if (user == nullptr)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("User not found: ") + QString::number(*token), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      reject();
      return;
    }

    ui_.editusername->setText(user->GetUsername());
    ui_.editpassword->setText("ABCDEF");
    ui_.editconfirmpassword->setText("ABCDEF");

    for (int i = 0; i < ui_.combogroup->count(); ++i)
    {
      if (ui_.combogroup->itemData(i, Qt::UserRole).toULongLong() == user->GetGroupToken())
      {
        ui_.combogroup->setCurrentIndex(i);
        break;
      }
    }

    setWindowTitle("Edit User: " + user->GetUsername());
  }
}

ManageUserWindow::~ManageUserWindow()
{
  userconnection_.Close();

}

void ManageUserWindow::SetEnabled(const bool enabled)
{
  ui_.editusername->setEnabled(enabled);
  ui_.editpassword->setEnabled(enabled);
  ui_.editconfirmpassword->setEnabled(enabled);
  ui_.combogroup->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageUserWindow::GroupAdded(const QSharedPointer<Group>& group)
{
  ui_.combogroup->addItem(group->GetName(), static_cast<qulonglong>(group->GetToken()));

}

void ManageUserWindow::GroupChanged(const QSharedPointer<Group>& group)
{
  for (int i = 0; i < ui_.combogroup->count(); ++i)
  {
    if (ui_.combogroup->itemData(i, Qt::UserRole).toULongLong() == group->GetToken())
    {
      ui_.combogroup->setItemText(i, group->GetName());

    }
  }
}

void ManageUserWindow::GroupRemoved(const uint64_t token)
{
  for (int i = (ui_.combogroup->count() - 1); i >= 0; --i)
  {
    if (ui_.combogroup->itemData(i, Qt::UserRole).toULongLong() == token)
    {
      ui_.combogroup->removeItem(i);

    }
  }
}

void ManageUserWindow::UserChanged(QSharedPointer<User>& user)
{
  if (user->GetToken() != *token_)
  {

    return;
  }

  if (QMessageBox::question(this, tr("Warning"), tr("User has changed, would you like to update?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    ui_.editusername->setText(user->GetUsername());

    for (int i = 0; i < ui_.combogroup->count(); ++i)
    {
      if (ui_.combogroup->itemData(i, Qt::UserRole).toULongLong() == user->GetGroupToken())
      {
        ui_.combogroup->setCurrentIndex(i);
        break;
      }
    }
  }
}

void ManageUserWindow::UserRemoved(const uint64_t token)
{
  if (token != *token_)
  {

    return;
  }

  QMessageBox(QMessageBox::Warning, tr("Error"), tr("User has been deleted"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
  reject();
}

void ManageUserWindow::on_editpassword_textChanged()
{
  passworddirty_ = true;

}

void ManageUserWindow::on_editconfirmpassword_textChanged()
{
  passworddirty_ = true;

}

void ManageUserWindow::on_buttonok_clicked()
{
  if (ui_.editusername->text().isEmpty())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a username"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (ui_.editpassword->text() != ui_.editconfirmpassword->text())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Passwords not equal"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (ui_.combogroup->currentData(Qt::UserRole).isNull())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Group not selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (token_.is_initialized())
  {
    for (const QSharedPointer<User>& user : device_->GetUsers())
    {
      if (user->GetToken() == *token_)
      {

        continue;
      }

      if (user->GetUsername() == ui_.editusername->text())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Username already exists"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    }

    boost::optional<std::string> password;
    if (passworddirty_)
    {
      password = monocle::AuthenticateDigest(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

    }

    SetEnabled(false);
    userconnection_ = device_->ChangeUser(*token_, password, ui_.combogroup->currentData(Qt::UserRole).toULongLong(), [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGEUSERRESPONSE& changeuserresponse)
    {
      SetEnabled(true);
      if (changeuserresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changeuserresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });

  }
  else
  {
    for (const QSharedPointer<User>& user : device_->GetUsers())
    {
      if (user->GetUsername() == ui_.editusername->text())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Username already exists"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    }

    if (ui_.editpassword->text().isEmpty() || ui_.editconfirmpassword->text().isEmpty())
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("Please enter a password"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }

    SetEnabled(false);
    userconnection_ = device_->AddUser(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), ui_.combogroup->currentData().toULongLong(), [this](const std::chrono::nanoseconds latency, const monocle::client::ADDUSERRESPONSE& adduserresponse)
    {
      SetEnabled(true);
      if (adduserresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(adduserresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
