// managetrackfindonvifdeviceonvifwindow.h
//

#ifndef IDEPG4GNCJDSADASDASDASDA4PS7Z3HTYF
#define IDEPG4GNCJDSADASDASDASDA4PS7Z3HTYF

///// Includes /////

#include <monocleprotocol/client/connection.hpp>
#include <memory>
#include <QTreeWidget>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;

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
  void Filter(QTreeWidgetItem* item);
  bool ChildrenContainsTextFilter(QTreeWidgetItem* item);

  boost::shared_ptr<Device> device_;
  monocle::client::Connection connection_;

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


};

}

#endif
