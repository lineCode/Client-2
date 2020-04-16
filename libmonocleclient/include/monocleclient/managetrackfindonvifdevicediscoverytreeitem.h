// managetrackfindonvifdeviceonvifwindow.h
//

#ifndef IDEPG4GNCYQOZXCZXCZCZXZZZSSDSAATYF
#define IDEPG4GNCYQOZXCZXCZCZXZZZSSDSAATYF

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

class ManageTrackFindONVIFDeviceDiscoveryTreeItem : public QTreeWidgetItem
{
 public:

  ManageTrackFindONVIFDeviceDiscoveryTreeItem(const boost::shared_ptr<Device>& device, const std::vector<std::string>& names, const std::vector<std::string>& locations, const std::string& address, const std::string& username, const std::string& password);
  virtual ~ManageTrackFindONVIFDeviceDiscoveryTreeItem();

  void SetUserPass(const std::string& username, const std::string& password);

  void Collapsed();
  void Expanded();

  int Update();

  void AddNames(const std::vector<std::string>& names);
  void AddLocations(const std::vector<std::string>& locations);

  bool TextFilter(const std::string& textfilter) const;

 protected:


 private:

  void RemoveChildren();
  void GetProfiles();
  void SetChildText(const QString& text); // This removes all current children and adds one new one with text
  void UpdateTooltip();

  const boost::shared_ptr<Device> device_;

  std::vector<std::string> names_;
  std::vector<std::string> locations_;
  std::string address_;
  std::string username_;
  std::string password_;

  boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;
  boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::Connection connection_;

};

}

#endif
