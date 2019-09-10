// discoverytree.cpp
//

///// Includes /////

#include "monocleclient/discoverytree.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address.hpp>
#include <network/uri.hpp>
#include <onviftypes/onviftypes.hpp>
#include <utility/ioservicepool.hpp>
#include <utility/utility.hpp>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DiscoveryTree::DiscoveryTree(QWidget* parent) :
  QTreeWidget(parent),
  showipv4_(true),
  showipv6_(true),
  showlinklocal_(true)
{
  // Set header
  setHeaderItem(new QTreeWidgetItem(QStringList({ tr("Names"), tr("Locations"), tr("Addresses") })));

  // First try to connect to any servers which may already hold the discovery port open via a localhost pipe
  if (pipeclient_.Init("980d28c8-4485-46ec-a91f-fb7543640d8f"))
  {
    // Begin WS discovery
    discover_ = boost::make_shared<onvif::wsdiscover::WsDiscoverClient>(MainWindow::Instance()->GetGUIIOService());
    discover_->hello_.connect(std::bind(&DiscoveryTree::Hello, this, std::placeholders::_1, std::placeholders::_2));
    if (discover_->Init())
    {
      LOG_GUI_WARNING(QString("WsDiscoverClient::Init failed"));
      addTopLevelItem(new QTreeWidgetItem({ QString("WsDiscoverClient::Init failed") }));
      return;
    }
  }
  else
  {
    connect(&pipeclient_, QOverload<const QString&>::of(&PipeClient::Message), this, &DiscoveryTree::Message);
    startTimer(50);
  }
  
  Refresh();
}

DiscoveryTree::~DiscoveryTree()
{
  if (discover_)
  {
    if (discover_->Destroy())
    {
      LOG_GUI_WARNING(QString("WsDiscoverClient::Destroy failed"));
      return;
    }
    discover_.reset();
  }
}

void DiscoveryTree::ShowIpv4(const bool showipv4)
{
  showipv4_ = showipv4;
  Filter();
}

void DiscoveryTree::ShowIpv6(const bool showipv6)
{
  showipv6_ = showipv6;
  Filter();
}

void DiscoveryTree::ShowLinkLocal(const bool showlinklocal)
{
  showlinklocal_ = showlinklocal;
  Filter();
}

void DiscoveryTree::Refresh()
{
  clear();

  if (discover_)
  {
    if (discover_->Broadcast())
    {
      LOG_GUI_WARNING(QString("WsDiscoverClient::Broadcast failed"));
      addTopLevelItem(new QTreeWidgetItem({ QString("WsDiscoverClient::Broadcast failed") }));
      return;
    }
  }
  else
  {
    if (pipeclient_.SendText("DISCOVER"))
    {
      LOG_GUI_WARNING(QString("PipeClient::SendText failed"));
      addTopLevelItem(new QTreeWidgetItem({ QString("PipeClient::SendText failed") }));
      return;
    }
  }
}

void DiscoveryTree::Hello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{
  if (std::find(scopes.cbegin(), scopes.cend(), std::string("onvif://www.onvif.org/manufacturer/Monocle")) == scopes.cend())
  {

    return;
  }

  std::vector<std::string> names;
  std::vector<std::string> locations;
  for (const std::string& scope : scopes)
  {
    if (scope.compare(0, 27, "onvif://www.onvif.org/name/") == 0)
    {
      names.push_back(onvif::UrlDecode(scope.substr(27, std::string::npos)));

    }
    else if (scope.compare(0, 31, "onvif://www.onvif.org/location/") == 0)
    {
      locations.push_back(onvif::UrlDecode(scope.substr(31, std::string::npos)));

    }
  }

  for (const std::string& address : addresses)
  {
    if (!boost::algorithm::iends_with(address, "/monocle_service"))
    {

      continue;
    }

    std::string host;
    uint16_t port = 0;
    try
    {
      const network::uri uri(address);
      if (!uri.has_host())
      {
        
        continue;
      }
      host = uri.host().to_string();

      if (!uri.has_port())
      {
        
        continue;
      }
      port = static_cast<uint16_t>(std::stoul(uri.port().to_string()));
    }
    catch (...)
    {

      continue;
    }

    const QString a = QString::fromStdString(host) + ":" + QString::number(port);
    if (!findItems(a, Qt::MatchExactly, 2).isEmpty()) // Ignore duplicates
    {

      continue;
    }

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList({ QString::fromStdString(boost::algorithm::join(names, ", ")), QString::fromStdString(boost::algorithm::join(locations, ", ")), a }));
    item->setData(0, Qt::UserRole, QString::fromStdString(host));
    item->setData(0, Qt::UserRole + 1, port);
    Filter(item);
    addTopLevelItem(item);
  }
}

void DiscoveryTree::timerEvent(QTimerEvent* event)
{
  pipeclient_.Update();
  QTreeWidget::timerEvent(event);
}

void DiscoveryTree::Filter()
{
  QTreeWidgetItemIterator item(this);
  while (*item)
  {
    Filter(*item);
    ++item;
  }
}

void DiscoveryTree::Filter(QTreeWidgetItem* item)
{
  boost::system::error_code err;
  const boost::asio::ip::address address = boost::asio::ip::address::from_string(item->text(2).toStdString(), err);
  if (err)
  {
    item->setHidden(false);
    return;
  }

  if (address.is_v4())
  {
    if (address.to_string().substr(0, 7) == std::string("169.254"))
    {
      item->setHidden(!(showipv4_ && showlinklocal_));
      
    }
    else
    {
      item->setHidden(!showipv4_);
      
    }
  }
  else if (address.is_v6())
  {
    if (address.to_v6().is_link_local())
    {
      item->setHidden(!(showipv6_ && showlinklocal_));
      
    }
    else
    {
      item->setHidden(!showipv6_);
      
    }
    return;
  }
}

void DiscoveryTree::Message(const QString& message)
{
  const int pos = message.indexOf(';');
  if (pos == -1)
  {
    // Ignore funny looking strings
    return;
  }

  QStringList tmp = message.left(pos).split(',');
  std::vector<std::string> addresses;
  addresses.reserve(tmp.size());
  for (const QString& t : tmp)
  {
    addresses.push_back(t.toStdString());

  }

  tmp = message.mid(pos + 1).split(',');
  std::vector<std::string> scopes;
  scopes.reserve(tmp.size());
  for (const QString& t : tmp)
  {
    scopes.push_back(t.toStdString());

  }

  Hello(addresses, scopes);
}

}
