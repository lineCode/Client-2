// managerecordingjobsourceonvifdiscoverytreeitem.h
//

#ifndef IDEPG4GNCYQO5W0Y9348294ADSSDSAATYF
#define IDEPG4GNCYQO5W0Y9348294ADSSDSAATYF

///// Includes /////

#include <memory>
#include <onvifclient/connection.hpp>
#include <QTreeWidgetItem>

///// Declarations /////

class QStringList;

namespace onvif
{
namespace device
{
class DeviceClient;
}
namespace media
{
class MediaClient;
}
}

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;

///// Globals /////

const int RECEIVERDISCOVERYITEM_DEVICE = 0;
const int RECEIVERDISCOVERYITEM_PROFILE = 1;

///// Classes /////

class ManageRecordingJobSourceONVIFDiscoveryTreeItem : public QTreeWidgetItem
{
 public:

  ManageRecordingJobSourceONVIFDiscoveryTreeItem(const boost::shared_ptr<Device>& device, const std::vector<std::string>& names, const std::vector<std::string>& locations, const std::string& address, const std::string& username, const std::string& password);
  virtual ~ManageRecordingJobSourceONVIFDiscoveryTreeItem();

  void SetUserPass(const std::string& username, const std::string& password);

  void Collapsed();
  void Expanded();

  int Update();

 protected:


 private:

  void RemoveChildren();
  void GetProfiles();
  void SetChildText(const QString& text); // This removes all current children and adds one new one with text

  const boost::shared_ptr<Device> device_;

  std::string address_;
  std::string username_;
  std::string password_;

  boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;
  boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::Connection connection_;

};

}

#endif
