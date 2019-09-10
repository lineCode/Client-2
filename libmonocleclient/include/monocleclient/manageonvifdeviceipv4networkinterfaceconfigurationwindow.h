// manageonvifdeviceipv4networkinterfaceconfigurationwindow.h
//

#ifndef IDJPF2S5X65656666666QYT4QU09G70AIX
#define IDJPF2S5X65656666666QYT4QU09G70AIX

///// Includes /////

#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>
#include <QIcon>

#include "ui_manageonvifdeviceipv4networkinterfaceconfigurationwindow.h"

///// Declarations /////

namespace onvif
{
namespace device
{
class DeviceClient;
}
}

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice, const std::string& token);
  virtual ~ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow();

 private:

  void SetEnable(bool enabled);
  void AddIPv4Item(const QString& address, int netmask);

  void Enable(bool enable);

  const std::string token_;

  Ui::ManageONVIFDeviceIPv4NetworkInterfaceConfigurationWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;

  onvif::ScopedConnection getnetworkinterfacesconnection_;
  onvif::ScopedConnection getdot11capabilities_;
  onvif::ScopedConnection scanavailabledot11networks_;
  onvif::ScopedConnection getzeroconfigurationconnection_;
  onvif::ScopedConnection setzeroconfigurationconnection_;
  onvif::ScopedConnection setnetworkinterfacesconnection_;
  onvif::ScopedConnection systemrebootconnection_;

  onvif::IPv6NetworkInterfaceSetConfiguration ipv6_; // We need to remember this to avoid resetting it

 private slots:

 void on_buttonipv4manualadd_clicked();
 void on_buttonipv4manualremove_clicked();
 void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
