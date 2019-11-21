// discoverytree.h
//

#ifndef IDEPG4GNCYQO5W0Y5RG9PPVI0PS7Z3HTYF
#define IDEPG4GNCYQO5W0Y5RG9PPVI0PS7Z3HTYF

///// Includes /////

#include <QTreeWidget>
#include <wsdiscover/wsdiscoverclient.hpp>

#include "pipeclient.h"

///// Namespaces /////

namespace client
{

///// Classes /////

class DiscoveryTree : public QTreeWidget
{
 Q_OBJECT

 public:

  DiscoveryTree(QWidget* parent);
  virtual ~DiscoveryTree();

  void ShowIpv4(const bool showipv4);
  void ShowIpv6(const bool showipv6);
  void ShowLinkLocal(const bool showlinklocal);

  void Refresh();

  void Hello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes);

 protected:

  virtual void timerEvent(QTimerEvent* event) override;

 private:

  void Filter();
  void Filter(QTreeWidgetItem* item);

  PipeClient pipeclient_;
  boost::shared_ptr<onvif::wsdiscover::WsDiscoverClient> discover_;//TODO need to use this MainWindow

  bool showipv4_;
  bool showipv6_;
  bool showlinklocal_;


 private slots:

  void Message(const QString& message);

};

}

#endif
