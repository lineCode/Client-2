// manageonvifdeviceipv4networkinterfacewindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifdeviceipv4networkinterfaceconfigurationwindow.h"

#include <boost/asio.hpp>
#include <onvifclient/deviceclient.hpp>
#include <QComboBox>
#include <QMessageBox>

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice, const std::string& token) :
  QDialog(parent),
  token_(token),
  devicecapabilities_(devicecapabilities),
  onvifdevice_(onvifdevice)
{
  ui_.setupUi(this);
  
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  setWindowTitle(windowTitle() + QString(": ") + QString::fromStdString(token));

  SetEnable(false);

  getnetworkinterfacesconnection_ = onvifdevice_->GetNetworkInterfacesCallback([this](const onvif::device::GetNetworkInterfacesResponse& getnetworkinterfacesresponse)
  {
    if (getnetworkinterfacesresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getnetworkinterfacesresponse.Message()), QMessageBox::Ok);
      return;
    }

    SetEnable(true);

    boost::optional<onvif::NetworkInterface> networkinterface = getnetworkinterfacesresponse.GetNetworkInterface(token_);
    if (!networkinterface.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString("Could not find interface: ") + QString::fromStdString(token_), QMessageBox::Ok);
      return;
    }

    if (networkinterface->ipv6_)
    {
      ipv6_.enabled_ = networkinterface->ipv6_->enabled_.is_initialized() ? *networkinterface->ipv6_->enabled_ : false;
      if (networkinterface->ipv6_->config_.is_initialized())
      {
        ipv6_.acceptrouteradvert_ = networkinterface->ipv6_->config_->acceptrouteradvert_.is_initialized() ? *networkinterface->ipv6_->config_->acceptrouteradvert_ : false;
        ipv6_.dhcp_ = networkinterface->ipv6_->config_->dhcp_;
        ipv6_.manual_ = networkinterface->ipv6_->config_->manual_;
      }
    }

    // IPv4
    if (networkinterface->ipv4_.is_initialized())
    {
      if (networkinterface->ipv4_->enabled_.is_initialized() && *networkinterface->ipv4_->enabled_)
      {
        ui_.checkipv4enabled->setChecked(true);

      }
        
      if (networkinterface->ipv4_->config_.is_initialized())
      {
        if (networkinterface->ipv4_->config_->dhcp_.is_initialized() && *networkinterface->ipv4_->config_->dhcp_)
        {
          ui_.radioipv4dhcp->setChecked(true);

        }
        else
        {
          ui_.radioipv4manual->setChecked(true);

        }

        for (const auto& address : networkinterface->ipv4_->config_->manual_)
        {
          if (!address.address_.is_initialized())
          {

            continue;
          }

          AddIPv4Item(QString::fromStdString(*address.address_), address.prefixlength_.is_initialized() ? *address.prefixlength_ : 0);
        }
      }
    }
  });
}

ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::~ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow()
{
  if (setzeroconfigurationconnection_.IsConnected() || setnetworkinterfacesconnection_.IsConnected() || systemrebootconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();
    
  }
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::SetEnable(bool enabled)
{
  ui_.checkipv4enabled->setEnabled(enabled);
  ui_.radioipv4manual->setEnabled(enabled);
  ui_.radioipv4dhcp->setEnabled(enabled);
  ui_.tableipv4manual->setEnabled(enabled);
  ui_.buttonipv4manualadd->setEnabled(enabled);
  ui_.buttonipv4manualremove->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::AddIPv4Item(const QString& address, int netmask)
{
  ui_.tableipv4manual->insertRow(ui_.tableipv4manual->rowCount());
  ui_.tableipv4manual->setItem(ui_.tableipv4manual->rowCount() - 1, 0, new QTableWidgetItem(address));

  QComboBox* combonetmask = new QComboBox(ui_.tableipv4manual);
  combonetmask->addItems(QStringList({ QString("0"), QString("1"), QString("2"), QString("3"), QString("4"), QString("5"), QString("6"), QString("7"), QString("8"), QString("9"), QString("10"), QString("11"), QString("12"), QString("13"), QString("14"), QString("15"), QString("16"), QString("17"), QString("18"), QString("19"), QString("20"), QString("21"), QString("22"), QString("23"), QString("24"), QString("25"), QString("26"), QString("27"), QString("28"), QString("29"), QString("30"), QString("31"), QString("32") }));
  ui_.tableipv4manual->setCellWidget(ui_.tableipv4manual->rowCount() - 1, 1, combonetmask);
  combonetmask->setCurrentIndex(combonetmask->findText(QString::number(utility::Clip(netmask, 0, 32))));
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::on_buttonipv4manualadd_clicked()
{
  AddIPv4Item(QString(), 0);
  ui_.tableipv4manual->editItem(ui_.tableipv4manual->item(ui_.tableipv4manual->rowCount() - 1, 0));
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::on_buttonipv4manualremove_clicked()
{
  const int index = ui_.tableipv4manual->currentRow();
  if (index == -1)
  {
    QMessageBox::warning(this, tr("Error"), QString("No item selected"), QMessageBox::Ok);
    return;
  }

  ui_.tableipv4manual->removeRow(index);
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::on_buttonok_clicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  Enable(false);

  onvif::NetworkInterfaceSetConfiguration networkinterface;
  networkinterface.ipv6_ = ipv6_;
  networkinterface.ipv4_ = onvif::IPv4NetworkInterfaceSetConfiguration();
  if (ui_.checkipv4enabled->isChecked())
  {
    networkinterface.ipv4_->enabled_ = true;

  }
  else
  {
    networkinterface.ipv4_->enabled_ = false;

  }

  if (ui_.radioipv4manual->isChecked())
  {
    networkinterface.ipv4_->dhcp_ = false;

  }
  else if (ui_.radioipv4dhcp->isChecked())
  {
    networkinterface.ipv4_->dhcp_ = true;

  }
  else
  {
    QMessageBox::warning(this, tr("Error"), QString("Invalid IPv4 DHCP type"), QMessageBox::Ok);
    return;
  }

  for (int i = 0; i < ui_.tableipv4manual->rowCount(); ++i)
  {
    boost::system::error_code err;
    boost::asio::ip::address_v4::from_string(ui_.tableipv4manual->item(i, 0)->text().toStdString(), err);
    if (err)
    {
      QMessageBox::warning(this, tr("Error"), QString("Invalid IPv4 Address: ") + ui_.tableipv4manual->item(i, 0)->text(), QMessageBox::Ok);
      return;
    }

    networkinterface.ipv4_->manual_.push_back(onvif::PrefixedIPv4Address(ui_.tableipv4manual->item(i, 0)->text().toStdString(), static_cast<QComboBox*>(ui_.tableipv4manual->cellWidget(i, 1))->currentText().toInt()));
  }

  setnetworkinterfacesconnection_ = onvifdevice_->SetNetworkInterfacesCallback(token_, networkinterface, [this](const onvif::device::SetNetworkInterfacesResponse& setnetworkinterfacesresponse)
  {
    QApplication::restoreOverrideCursor();
    Enable(true);

    if (setnetworkinterfacesresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setnetworkinterfacesresponse.Message()), QMessageBox::Ok);
      return;
    }

    if (setnetworkinterfacesresponse.rebootneeded_.is_initialized() && *setnetworkinterfacesresponse.rebootneeded_)
    {
      if (QMessageBox::question(this, tr("Restart required"), tr("Restart now?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        Enable(false);

        systemrebootconnection_ = onvifdevice_->SystemRebootCallback([this](const onvif::device::SystemRebootResponse& systemrebootresponse)
        {
          QApplication::restoreOverrideCursor();

          if (systemrebootresponse.Error())
          {
            QMessageBox::warning(this, tr("Error"), QString::fromStdString(systemrebootresponse.Message()), QMessageBox::Ok);

          }

          accept();
        });
      }
    }
    else
    {
      accept();

    }
  });
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow::Enable(bool enable)
{
  SetEnable(enable);

}

}

QT_END_NAMESPACE
