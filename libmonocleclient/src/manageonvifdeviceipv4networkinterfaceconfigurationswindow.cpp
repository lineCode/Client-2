// manageonvifdeviceipv4networkinterfaceconfigurationwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifdeviceipv4networkinterfaceconfigurationswindow.h"

#include <onvifclient/deviceclient.hpp>
#include <QListWidget>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifdeviceipv4networkinterfaceconfigurationwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow::ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifdevice_(onvifdevice)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getnetworkinterfacesconnection_ = onvifdevice_->GetNetworkInterfacesCallback([this](const onvif::device::GetNetworkInterfacesResponse& getnetworkinterfacesresponse)
  {
    SetEnabled(true);

    if (getnetworkinterfacesresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getnetworkinterfacesresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& networkinterface : getnetworkinterfacesresponse.networkinterfaces_)
    {
      if (!networkinterface.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("NetworkInterface::token_ not initialised"));
        continue;
      }
      
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*networkinterface.token_) + ((networkinterface.info_.is_initialized() && networkinterface.info_->name_.is_initialized()) ? (QString("(") + QString::fromStdString(*networkinterface.info_->name_) + (QString(")"))) : QString()));
      item->setData(Qt::UserRole, QString::fromStdString(*networkinterface.token_));
      ui_.listinterfaces->addItem(item);
    }
    ui_.listinterfaces->setCurrentRow(0);
  });
}

ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow::~ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow()
{

}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listinterfaces->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listinterfaces->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow(this, devicecapabilities_, onvifdevice_, item->data(Qt::UserRole).toString().toStdString()).exec();
}

}

QT_END_NAMESPACE
