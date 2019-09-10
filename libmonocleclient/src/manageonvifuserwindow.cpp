// manageonvifuserwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifuserwindow.h"

#include <monocleprotocol/onvifuserlevel_generated.h>
#include <QMessageBox>
#include <QVariant>

#include "monocleclient/device.h"
#include "monocleclient/onvifuser.h"

///// Declarations /////

Q_DECLARE_METATYPE(monocle::ONVIFUserlevel);

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFUserWindow::ManageONVIFUserWindow(QWidget* parent, boost::shared_ptr<Device>& device, const boost::optional<uint64_t>& token) :
  QDialog(parent),
  device_(device),
  token_(token),
  passworddirty_(false)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  for (int8_t onvifuserlevel = static_cast<int8_t>(monocle::ONVIFUserlevel::MIN); onvifuserlevel <= static_cast<int8_t>(monocle::ONVIFUserlevel::MAX); ++onvifuserlevel)
  {
    QVariant tmp;
    tmp.setValue(static_cast<monocle::ONVIFUserlevel>(onvifuserlevel));
    ui_.combouserlevel->addItem(monocle::EnumNameONVIFUserlevel(static_cast<monocle::ONVIFUserlevel>(onvifuserlevel)), tmp);

  }

  if (token.is_initialized())
  {
    connect(device.get(), QOverload<QSharedPointer<ONVIFUser>&>::of(&Device::SignalONVIFUserChanged), this, &ManageONVIFUserWindow::ONVIFUserChanged);
    connect(device.get(), QOverload<const uint64_t>::of(&Device::SignalONVIFUserRemoved), this, &ManageONVIFUserWindow::ONVIFUserRemoved);

    QSharedPointer<ONVIFUser> user = device_->GetONVIFUser(*token);
    if (user == nullptr)
    {
      QMessageBox(QMessageBox::Warning, tr("Error"), tr("ONVIF User not found: ") + QString::number(*token), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      reject();
      return;
    }

    ui_.editusername->setText(user->GetUsername());
    ui_.editpassword->setText("ABCDEF");
    ui_.editconfirmpassword->setText("ABCDEF");

    for (int i = 0; i < ui_.combouserlevel->count(); ++i)
    {
      if (ui_.combouserlevel->itemData(i, Qt::UserRole).value<monocle::ONVIFUserlevel>() == user->GetUserlevel())
      {
        ui_.combouserlevel->setCurrentIndex(i);
        break;
      }
    }

    setWindowTitle("Edit ONVIF User: " + user->GetUsername());
  }
}

ManageONVIFUserWindow::~ManageONVIFUserWindow()
{
  userconnection_.Close();

}

void ManageONVIFUserWindow::SetEnabled(const bool enabled)
{
  ui_.editusername->setEnabled(enabled);
  ui_.editpassword->setEnabled(enabled);
  ui_.editconfirmpassword->setEnabled(enabled);
  ui_.combouserlevel->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageONVIFUserWindow::ONVIFUserChanged(QSharedPointer<ONVIFUser>& onvifuser)
{
  if (onvifuser->GetToken() != *token_)
  {

    return;
  }

  if (QMessageBox::question(this, tr("Warning"), tr("ONVIF User has changed, would you like to update?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    ui_.editusername->setText(onvifuser->GetUsername());

    for (int i = 0; i < ui_.combouserlevel->count(); ++i)
    {
      if (ui_.combouserlevel->itemData(i, Qt::UserRole).value<monocle::ONVIFUserlevel>() == onvifuser->GetUserlevel())
      {
        ui_.combouserlevel->setCurrentIndex(i);
        break;
      }
    }
  }
}

void ManageONVIFUserWindow::ONVIFUserRemoved(const uint64_t token)
{
  if (token != *token_)
  {

    return;
  }

  QMessageBox(QMessageBox::Warning, tr("Error"), tr("ONVIF User has been deleted"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
  reject();
}

void ManageONVIFUserWindow::on_editpassword_textChanged()
{
  passworddirty_ = true;

}

void ManageONVIFUserWindow::on_editconfirmpassword_textChanged()
{
  passworddirty_ = true;

}

void ManageONVIFUserWindow::on_buttonok_clicked()
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

  if (ui_.combouserlevel->currentData(Qt::UserRole).isNull())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Group not selected"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (token_.is_initialized())
  {
    for (const QSharedPointer<ONVIFUser>& onvifuser : device_->GetONVIFUsers())
    {
      if (onvifuser->GetToken() == *token_)
      {

        continue;
      }

      if (onvifuser->GetUsername() == ui_.editusername->text())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Username already exists"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    }

    boost::optional<std::string> password;
    if (passworddirty_)
    {
      password = ui_.editpassword->text().toStdString();

    }

    SetEnabled(false);
    userconnection_ = device_->ChangeONVIFUser(*token_, ui_.editusername->text().toStdString(), password, ui_.combouserlevel->currentData(Qt::UserRole).value<monocle::ONVIFUserlevel>(), [this](const std::chrono::nanoseconds latency, const monocle::client::CHANGEONVIFUSERRESPONSE& changeonvifuserresponse)
    {
      SetEnabled(true);
      if (changeonvifuserresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(changeonvifuserresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
  else
  {
    for (const QSharedPointer<ONVIFUser>& onvifuser : device_->GetONVIFUsers())
    {
      if (onvifuser->GetUsername() == ui_.editusername->text())
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
    userconnection_ = device_->AddONVIFUser(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), ui_.combouserlevel->currentData().value<monocle::ONVIFUserlevel>(), [this](const std::chrono::nanoseconds latency, const monocle::client::ADDONVIFUSERRESPONSE& addonvifuserresponse)
    {
      SetEnabled(true);
      if (addonvifuserresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error: ") + QString::fromStdString(addonvifuserresponse.GetErrorText()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
      accept();
    });
  }
}

}

QT_END_NAMESPACE
