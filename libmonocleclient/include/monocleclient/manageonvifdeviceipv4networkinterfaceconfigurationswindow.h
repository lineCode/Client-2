// manageonvifdeviceipv4networkinterfaceconfigurationwindow.h
//

#ifndef ID18TBZQ0UHQE3D43434444MLFIK77GVDQ
#define ID18TBZQ0UHQE3D43434444MLFIK77GVDQ

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifdeviceipv4networkinterfaceconfigurationswindow.h"

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

class ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice);
  virtual ~ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;

  onvif::ScopedConnection getnetworkinterfacesconnection_;

 private slots:

  void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
