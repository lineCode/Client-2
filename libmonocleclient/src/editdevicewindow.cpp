// editdevicewindow.cpp
//

///// Includes /////

#include "monocleclient/editdevicewindow.h"

#include <assert.h>
#include <monocleprotocol/server/server.hpp>
#include <QMessageBox>
#include <QNetworkProxyFactory>
#include <QPushButton>

#include "monocleclient/devicemgr.h"
#include "monocleclient/discoverytree.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/options.h"

///// Namespaces /////

namespace client
{

///// Methods /////

EditDeviceWindow::EditDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);
  
  connect(ui_.treediscovery, &DiscoveryTree::itemClicked, this, &EditDeviceWindow::DiscoveryTreeItemClicked);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editproxyport->setValidator(new QIntValidator(1, 65535, this));
  ui_.editport->setValidator(new QIntValidator(1, 65535, this));

  QPalette palette;
  palette.setColor(QPalette::Base, QColor(200, 200, 200));
  ui_.editconnectresult->setPalette(palette);

  ui_.comboproxytype->addItem(QString::fromStdString(sock::ToString(sock::PROXYTYPE_NONE)), static_cast<int>(sock::PROXYTYPE_NONE));
  ui_.comboproxytype->addItem(QString::fromStdString(sock::ToString(sock::PROXYTYPE_HTTP)), static_cast<int>(sock::PROXYTYPE_HTTP));
  ui_.comboproxytype->addItem(QString::fromStdString(sock::ToString(sock::PROXYTYPE_SOCKS5)), static_cast<int>(sock::PROXYTYPE_SOCKS5));

  if (device_)
  {
    if (device_->GetProxyParams().GetType() == sock::PROXYTYPE_NONE)
    {
      EnableProxy(false);

    }
    else
    {
      const int item = ui_.comboproxytype->findData(static_cast<int>(device_->GetProxyParams().GetType()));
      if (item != -1)
      {
        ui_.comboproxytype->setCurrentIndex(item);
        EnableProxy(true);
      }

      ui_.checkremotedns->setChecked(device_->GetProxyParams().GetRemoteDns());
      ui_.editproxyhostname->setText(QString::fromStdString(device_->GetProxyParams().GetHostname()));
      ui_.editproxyport->setText(QString::number(device_->GetProxyParams().GetPort()));
      ui_.editproxyusername->setText(QString::fromStdString(device_->GetProxyParams().GetUsername()));
      ui_.editproxypassword->setText(QString::fromStdString(device_->GetProxyParams().GetPassword()));
    }

    ui_.editaddress->setText(device_->GetAddress());
    ui_.editport->setText(QString::number(device_->GetPort()));
    ui_.editusername->setText(device_->GetUsername());
    ui_.editpassword->setText(device_->GetPassword());
  }
  else
  {
    EnableProxy(false);

    ui_.editport->setText(QString::number(monocle::server::DEFAULTPORT));
  }
}

EditDeviceWindow::EditDeviceWindow(QWidget* parent, const QString& address, const uint16_t port, const QString& username, const QString& password) :
  QDialog(parent)
{
  ui_.setupUi(this);

  connect(ui_.treediscovery, &DiscoveryTree::itemClicked, this, &EditDeviceWindow::DiscoveryTreeItemClicked);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editproxyport->setValidator(new QIntValidator(1, 65535, this));
  ui_.editport->setValidator(new QIntValidator(1, 65535, this));

  QPalette palette;
  palette.setColor(QPalette::Base, QColor(200, 200, 200));
  ui_.editconnectresult->setPalette(palette);

  ui_.comboproxytype->addItem(QString::fromStdString(sock::ToString(sock::PROXYTYPE_NONE)), static_cast<int>(sock::PROXYTYPE_NONE));
  ui_.comboproxytype->addItem(QString::fromStdString(sock::ToString(sock::PROXYTYPE_HTTP)), static_cast<int>(sock::PROXYTYPE_HTTP));
  ui_.comboproxytype->addItem(QString::fromStdString(sock::ToString(sock::PROXYTYPE_SOCKS5)), static_cast<int>(sock::PROXYTYPE_SOCKS5));

  ui_.editaddress->setText(address);
  ui_.editaddress->setText(QString::number(port));
  ui_.editaddress->setText(username);
  ui_.editaddress->setText(password);
}

EditDeviceWindow::~EditDeviceWindow()
{
  connectconnection_.Close();
  monocleconnection_.Close();
}

void EditDeviceWindow::EnableProxy(bool enable)
{
  ui_.checkremotedns->setEnabled(enable);
  ui_.editproxyhostname->setEnabled(enable);
  ui_.editproxyport->setEnabled(enable);
  ui_.editproxyusername->setEnabled(enable);
  ui_.editproxypassword->setEnabled(enable);
}

void EditDeviceWindow::SetConnectResult(const QString& text)
{
  ui_.editconnectresult->setText(text);
  ui_.buttontest->setEnabled(true);
}

void EditDeviceWindow::DiscoveryTreeItemClicked(QTreeWidgetItem* item, int)
{
  // Grab the address
  const QString address = item->data(0, Qt::UserRole).toString();
  ui_.editaddress->setText(address);
  ui_.editport->setText(QString::number(item->data(0, Qt::UserRole + 1).toUInt()));
}

void EditDeviceWindow::on_checkipv4_clicked()
{
  ui_.treediscovery->ShowIpv4(ui_.checkipv4->isChecked());

}

void EditDeviceWindow::on_checkipv6_clicked()
{
  ui_.treediscovery->ShowIpv6(ui_.checkipv6->isChecked());

}

void EditDeviceWindow::on_checklinklocal_clicked()
{
  ui_.treediscovery->ShowLinkLocal(ui_.checklinklocal->isChecked());

}

void EditDeviceWindow::on_buttondiscoverrefresh_clicked()
{
  ui_.treediscovery->Refresh();

}

void EditDeviceWindow::on_comboproxytype_currentIndexChanged(int index)
{
  if (index == 0)
  {
    EnableProxy(false);

  }
  else
  {
    EnableProxy(true);

  }
}

void EditDeviceWindow::on_buttontest_clicked()
{
  if (testdevice_)
  {
    testdevice_->Destroy();

  }

  ui_.editconnectresult->clear();

  // Validate input
  const sock::ProxyParams proxyparams(sock::FromString(ui_.comboproxytype->currentText().toStdString()), ui_.editproxyhostname->text().toStdString(), ui_.editproxyport->text().toInt(), ui_.checkremotedns->isChecked(), ui_.editproxyusername->text().toStdString(), ui_.editproxypassword->text().toStdString());
  if (!proxyparams.IsValid())
  {
    ui_.editconnectresult->setText("Invalid proxy parameters");
    return;
  }

  if (ui_.editaddress->text().isEmpty())
  {
    ui_.editconnectresult->setText("Invalid address");
    return;
  }

  ui_.editconnectresult->setText("Connecting...");
  testdevice_ = boost::make_shared<Device>(proxyparams, ui_.editaddress->text(), ui_.editport->text().toUInt(), ui_.editusername->text(), ui_.editpassword->text(), 0);
  connectconnection_ = testdevice_->Connect([this](const boost::system::error_code& err)
  {
    if (err)
    {
      ui_.editconnectresult->setText(QString::fromStdString("Unable to connect"));
      return;
    }

    ui_.editconnectresult->setText("Authenticating...");
    monocleconnection_ = testdevice_->GetAuthenticationNonce([this](const std::chrono::steady_clock::duration latency, const monocle::client::GETAUTHENTICATIONNONCERESPONSE& getauthenticationnonceresponse)
    {
      if (getauthenticationnonceresponse.GetErrorCode() != monocle::ErrorCode::Success)
      {
        ui_.editconnectresult->setText(QString::fromStdString(getauthenticationnonceresponse.GetErrorText()));
        return;
      }

      const std::string clientnonce = utility::GenerateRandomString(32);
      monocleconnection_ = testdevice_->Authenticate(ui_.editusername->text().toStdString(), clientnonce, monocle::AuthenticateDigest(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), getauthenticationnonceresponse.authenticatenonce_, clientnonce), [this](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticateresponse)
      {
        if (authenticateresponse.GetErrorCode() != monocle::ErrorCode::Success)
        {
          ui_.editconnectresult->setText(QString::fromStdString(authenticateresponse.GetErrorText()));
          return;
        }

        ui_.editconnectresult->setText("Success");
      });
    });
  });
}

void EditDeviceWindow::on_buttonok_clicked()
{
  // Validate input
  const sock::ProxyParams proxyparams(sock::FromString(ui_.comboproxytype->currentText().toStdString()), ui_.editproxyhostname->text().toStdString(), ui_.editproxyport->text().toInt(), ui_.checkremotedns->isChecked(), ui_.editproxyusername->text().toStdString(), ui_.editproxypassword->text().toStdString());
  if (!proxyparams.IsValid())
  {
    QMessageBox(QMessageBox::Information, tr("Warning"), tr("Invalid proxy parameters"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (ui_.editaddress->text().isEmpty())
  {
    QMessageBox(QMessageBox::Information, tr("Warning"), tr("Address empty"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  for (const boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    if (device == device_)
    {

      continue;
    }

    if ((device->GetProxyParams() == proxyparams) && (device->GetAddress() == ui_.editaddress->text()))
    {
      QMessageBox(QMessageBox::Information, tr("Warning"), tr("Device already exists"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  }
  
  if (device_)
  {
    device_->Set(proxyparams, ui_.editaddress->text(), ui_.editport->text().toUInt(), ui_.editusername->text(), ui_.editpassword->text(), 0);
    MainWindow::Instance()->GetDeviceMgr().Save();
    device_->Subscribe();
  }
  else
  {
    if (!MainWindow::Instance()->GetDeviceMgr().AddDevice(proxyparams, ui_.editaddress->text(), ui_.editport->text().toInt(), ui_.editusername->text(), ui_.editpassword->text(), 0, true))
    {
      QMessageBox(QMessageBox::Information, tr("Warning"), tr("Failed to add onvif device"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
      return;
    }
  }

  accept();
}

}
