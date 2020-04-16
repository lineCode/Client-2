// managetrackfindonvifdevicediscoverytree.cpp
// 

///// Includes /////

#include "monocleclient/managetrackfindonvifdevicediscoverytree.h"

#include <boost/asio/ip/address.hpp>
#include <network/uri.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QTimer>
#include <QUrl>
#include <utility/ioservicepool.hpp>
#include <utility/utility.hpp>

#include "monocleclient/managetrackfindonvifdevicediscoverytreeitem.h"
#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Globals /////

const int ADDRESS_ROLE = Qt::UserRole + 1;
const int PROFILE_TOKEN_ROLE = Qt::UserRole + 2;
const int SCHEMA_ROLE = Qt::UserRole + 3;
const int PORT_ROLE = Qt::UserRole + 4;
const int HOST_ROLE = Qt::UserRole + 5;
const int PATH_ROLE = Qt::UserRole + 6;

///// Methods /////

ManageTrackFindONVIFDeviceDiscoveryTree::ManageTrackFindONVIFDeviceDiscoveryTree(QWidget* parent) :
  QTreeWidget(parent),
  device_(nullptr),
  showipv4_(true),
  showipv6_(true),
  showlinklocal_(true)
{
  connect(this, &QTreeWidget::itemCollapsed, this, &ManageTrackFindONVIFDeviceDiscoveryTree::ItemCollapsed);
  connect(this, &QTreeWidget::itemExpanded, this, &ManageTrackFindONVIFDeviceDiscoveryTree::ItemExpanded);
  connect(MainWindow::Instance(), &MainWindow::DiscoveryStreamingDeviceHello, this, &ManageTrackFindONVIFDeviceDiscoveryTree::DiscoveryHello);
  connect(&networkmapper_, &NetworkMapper::DiscoverONVIFDevice, this, &ManageTrackFindONVIFDeviceDiscoveryTree::DiscoverONVIFDevice);
  
  startTimer(std::chrono::milliseconds(60));
}

ManageTrackFindONVIFDeviceDiscoveryTree::~ManageTrackFindONVIFDeviceDiscoveryTree()
{
  connection_.Close();

  if (device_)
  {
    device_->UnsubscribeDiscovery();

  }
}

void ManageTrackFindONVIFDeviceDiscoveryTree::Init(const boost::shared_ptr<Device>& device, const std::string& username, const std::string& password)
{
  device_ = device;
  username_ = username;
  password_ = password;

  connection_ = device_->SubscribeDiscovery(true, [this](const std::chrono::steady_clock::duration latency, const monocle::client::SUBSCRIBEDISCOVERYRESPONSE& subscribediscoveryresponse)
  {
    if (subscribediscoveryresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      addTopLevelItem(new QTreeWidgetItem({ QString::fromStdString("Error: " + subscribediscoveryresponse.GetErrorText()) }));
      return;
    }
  });

  connect(device_.get(), &Connection::SignalDiscoveryHello, this, &ManageTrackFindONVIFDeviceDiscoveryTree::DiscoveryHello);

  Refresh();
}

void ManageTrackFindONVIFDeviceDiscoveryTree::SetTextFilter(const QString& textfilter)
{
  textfilter_ = textfilter;
  Filter();
}

void ManageTrackFindONVIFDeviceDiscoveryTree::ShowIpv4(const bool showipv4)
{
  showipv4_ = showipv4;
  Filter();
}

void ManageTrackFindONVIFDeviceDiscoveryTree::ShowIpv6(const bool showipv6)
{
  showipv6_ = showipv6;
  Filter();
}

void ManageTrackFindONVIFDeviceDiscoveryTree::ShowLinkLocal(const bool showlinklocal)
{
  showlinklocal_ = showlinklocal;
  Filter();
}

void ManageTrackFindONVIFDeviceDiscoveryTree::Refresh()
{
  clear();
  connection_ = device_->DiscoveryBroadcast([this](const std::chrono::steady_clock::duration latency, const monocle::client::DISCOVERYBROADCASTRESPONSE& discoverybroadcastresponse)
  {
    if (discoverybroadcastresponse.GetErrorCode() != monocle::ErrorCode::Success)
    {
      addTopLevelItem(new QTreeWidgetItem({ QString::fromStdString("Error: " + discoverybroadcastresponse.GetErrorText()) }));
      return;
    }
  });

  MainWindow::Instance()->DiscoveryBroadcast();

  networkmapper_.Init();
}

void ManageTrackFindONVIFDeviceDiscoveryTree::SetUserPass(const std::string& username, const std::string& password)
{
  username_ = username;
  password_ = password;
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    ManageTrackFindONVIFDeviceDiscoveryTreeItem* item = static_cast<ManageTrackFindONVIFDeviceDiscoveryTreeItem*>(topLevelItem(i));
    item->SetUserPass(username, password);
  }
}

void ManageTrackFindONVIFDeviceDiscoveryTree::timerEvent(QTimerEvent* event)
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    ManageTrackFindONVIFDeviceDiscoveryTreeItem* item = dynamic_cast<ManageTrackFindONVIFDeviceDiscoveryTreeItem*>(topLevelItem(i));
    if (item == nullptr) // Top level item might be an error, so we use this ugly hack to check
    {

      continue;
    }
    if (item->Update())
    {
      // Ignore

    }
  }
  QTreeWidget::timerEvent(event);
}

void ManageTrackFindONVIFDeviceDiscoveryTree::Filter()
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    Filter(topLevelItem(i));

  }
}

void ManageTrackFindONVIFDeviceDiscoveryTree::Filter(QTreeWidgetItem* item)
{
  if (textfilter_.size())
  {
    if (!item->text(0).contains(textfilter_, Qt::CaseInsensitive) && !ChildrenContainsTextFilter(item))
    {
      item->setHidden(true);
      return;
    }
  }

  const QUrl url(item->text(0));
  if (!url.isValid())
  {
    item->setHidden(true);
    return;
  }
  //TODO we can just peel the host now with HOST_ROLE
  boost::system::error_code err;
  const boost::asio::ip::address address = boost::asio::ip::address::from_string(url.host().toStdString(), err);
  if (err)
  {
    item->setHidden(true);
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
  }
}

bool ManageTrackFindONVIFDeviceDiscoveryTree::ChildrenContainsTextFilter(QTreeWidgetItem* item)
{
  // Look to see if any children contain the text filter
  for (int i = 0; i < item->childCount(); ++i)
  {
    if (item->child(i)->text(0).contains(textfilter_))
    {

      return true;
    }
  }
  return false;
}

ManageTrackFindONVIFDeviceDiscoveryTreeItem* ManageTrackFindONVIFDeviceDiscoveryTree::FindItem(const QString& schema, const uint16_t port, const QString& host, const QString& path)
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    ManageTrackFindONVIFDeviceDiscoveryTreeItem* item = static_cast<ManageTrackFindONVIFDeviceDiscoveryTreeItem*>(topLevelItem(i));
    if ((item->data(0, SCHEMA_ROLE).toString() == schema) && (item->data(0, PORT_ROLE).toUInt() == port) && (item->data(0, HOST_ROLE).toString() == host) && (item->data(0, PATH_ROLE).toString() == path))
    {

      return item;
    }
  }
  return nullptr;
}

void ManageTrackFindONVIFDeviceDiscoveryTree::AddItem(const std::string& address, const std::vector<std::string>& names, const std::vector<std::string>& locations)
{
  // Break it down
  QString schema;
  uint16_t port;
  QString host;
  QString path;
  try
  {
    const network::uri uri(address);
    if (!uri.has_scheme() || !uri.has_host() || !uri.has_path())
    {

      return;
    }
    schema = QString::fromStdString(uri.scheme().to_string());
    if (uri.has_port() && !uri.port().empty())
    {
      port = boost::lexical_cast<uint16_t>(uri.port().to_string());

    }
    else
    {
      if (schema == "https")
      {
        port = 443;

      }
      else
      {
        port = 80;

      }
    }
    host = QString::fromStdString(uri.host().to_string());
    path = QString::fromStdString(uri.path().to_string());
  }
  catch (...)
  {

    return;
  }

  ManageTrackFindONVIFDeviceDiscoveryTreeItem* item = FindItem(schema, port, host, path);
  if (item) // Ignore duplicates
  {
    item->AddNames(names);
    item->AddLocations(locations);
    return;
  }

  item = new ManageTrackFindONVIFDeviceDiscoveryTreeItem(device_, names, locations, address, username_, password_);

  item->setData(0, Qt::UserRole, RECEIVERDISCOVERYITEM_DEVICE);
  item->setData(0, ADDRESS_ROLE, QString::fromStdString(address));
  item->setData(0, SCHEMA_ROLE, schema);
  item->setData(0, PORT_ROLE, port);
  item->setData(0, HOST_ROLE, host);
  item->setData(0, PATH_ROLE, path);
  Filter(item);
  addTopLevelItem(item);
}

void ManageTrackFindONVIFDeviceDiscoveryTree::ItemCollapsed(QTreeWidgetItem* item)
{
  static_cast<ManageTrackFindONVIFDeviceDiscoveryTreeItem*>(item)->Collapsed();

}

void ManageTrackFindONVIFDeviceDiscoveryTree::ItemExpanded(QTreeWidgetItem* item)
{
  static_cast<ManageTrackFindONVIFDeviceDiscoveryTreeItem*>(item)->Expanded();

}

void ManageTrackFindONVIFDeviceDiscoveryTree::DiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{
  if (std::find_if(scopes.cbegin(), scopes.cend(), [](const std::string& scope) { return ((scope.find("audio_encoder") != std::string::npos) || (scope.find("video_encoder") != std::string::npos) || (scope.find("Streaming") != std::string::npos)); }) == scopes.cend())
  {
    // Ignore anything that doesn't advertise a audio or video encoder
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
    AddItem(address, addresses, scopes);

  }
}

void ManageTrackFindONVIFDeviceDiscoveryTree::DiscoverONVIFDevice(const std::string& address)
{
  AddItem(address, std::vector<std::string>(), std::vector<std::string>());

}

}
