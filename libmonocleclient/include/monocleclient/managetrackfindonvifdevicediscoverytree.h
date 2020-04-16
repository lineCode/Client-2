// managetrackfindonvifdevicediscoverytree.h
//

#ifndef IDEPG4GNCJDSADASDASDASDA4PS7Z3HTYF
#define IDEPG4GNCJDSADASDASDASDA4PS7Z3HTYF

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <memory>
#include <QTreeWidget>

#include "monocleclient/networkmapper.h"

///// Namespaces /////

namespace client
{

///// Globals /////

extern const int ADDRESS_ROLE;
extern const int PROFILE_TOKEN_ROLE;
extern const int SCHEMA_ROLE;
extern const int PORT_ROLE;
extern const int HOST_ROLE;
extern const int PATH_ROLE;

///// Declarations /////

class Device;
class ManageTrackFindONVIFDeviceDiscoveryTreeItem;

///// Classes /////

class ManageTrackFindONVIFDeviceDiscoveryTree : public QTreeWidget
{
 Q_OBJECT

 public:

  ManageTrackFindONVIFDeviceDiscoveryTree(QWidget* parent);
  ~ManageTrackFindONVIFDeviceDiscoveryTree();

  void Init(const boost::shared_ptr<Device>& device, const std::string& username, const std::string& password);

  void SetTextFilter(const QString& textfilter);
  void ShowIpv4(const bool showipv4);
  void ShowIpv6(const bool showipv6);
  void ShowLinkLocal(const bool showlinklocal);

  void Refresh();

  void SetUserPass(const std::string& username, const std::string& password);


 protected:

  void timerEvent(QTimerEvent* event) override;


 private:

  void Filter();
  void Filter(ManageTrackFindONVIFDeviceDiscoveryTreeItem* item);
  bool ChildrenContainsTextFilter(QTreeWidgetItem* item);
  ManageTrackFindONVIFDeviceDiscoveryTreeItem* FindItem(const QString& schema, const uint16_t port, const QString& host, const QString& path);
  void AddItem(const std::string& address, const std::vector<std::string>& names, const std::vector<std::string>& locations);

  boost::shared_ptr<Device> device_;
  monocle::client::Connection connection_;
  NetworkMapper networkmapper_;

  QString textfilter_;
  bool showipv4_;
  bool showipv6_;
  bool showlinklocal_;

  std::string username_;
  std::string password_;


 private slots:

  void ItemCollapsed(QTreeWidgetItem* item);
  void ItemExpanded(QTreeWidgetItem* item);
  void DiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes);
  void DiscoverONVIFDevice(const std::string& address);


};

}

#endif
