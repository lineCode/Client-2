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

bool ManageTrackFindONVIFDeviceDiscoveryTree::HasItem(const std::string& address)
{
  std::string schema;
  std::string port;
  std::string host;
  std::string path;
  try
  {
    const network::uri uri(address);
    if (!uri.has_scheme() || !uri.has_host() || !uri.has_path())
    {

      return false;
    }
    schema = uri.scheme().to_string();
    if (uri.has_port() && !uri.port().empty())
    {
      port = uri.port().to_string();

    }
    else
    {
      if (schema == "https")
      {
        port = "443";

      }
      else
      {
        port = "80";

      }
    }
    host = uri.host().to_string();
    path = uri.path().to_string();
  }
  catch (...)
  {

    return false;
  }


  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* item = topLevelItem(i);
    try
    {
      const network::uri uri(item->text(0).toStdString());//TODO store up these details so we don't have to parse them every time imo cos there could be a lot of work to do here...
      if (!uri.has_scheme() || !uri.has_host() || !uri.has_path())
      {

        continue;
      }

      std::string port2;
      if (uri.has_port() && !uri.port().empty())
      {
        port2 = uri.port().to_string();

      }
      else
      {
        if (uri.scheme().to_string() == "https")
        {
          port2 = "443";

        }
        else
        {
          port2 = "80";

        }
      }

      if ((uri.scheme().to_string() == schema) && (port2 == port) && (uri.host().to_string() == host) && (uri.path().to_string() == path))
      {

        return true;
      }
    }
    catch (...)
    {

    }
  }
  return false;
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

    //TODO maybe create a method which adds these addresses in, so we can use it below...

    //TODO network::uri uri(address); to make sure it looks reasonable and collect the details. Then dump them into the setData

    if (HasItem(address)) // Ignore duplicates
    {

      continue;
    }

    ManageTrackFindONVIFDeviceDiscoveryTreeItem* item = new ManageTrackFindONVIFDeviceDiscoveryTreeItem(device_, names, locations, address, username_, password_);

    //TODO add names and locations to the item if they don't already exist
      //TODO they just go on tooltips I think?
    item->setData(0, Qt::UserRole, RECEIVERDISCOVERYITEM_DEVICE);
    item->setData(0, Qt::UserRole + 1, QString::fromStdString(address));
    Filter(item);
    //TODO addTopLevelItem(item);
  }
}

void ManageTrackFindONVIFDeviceDiscoveryTree::DiscoverONVIFDevice(const std::string& address)
{
  if (HasItem(address)) // Ignore duplicates
  {

    return;
  }

  //TODO network::uri uri(address); to make sure it looks reasonable and collect the details

  ManageTrackFindONVIFDeviceDiscoveryTreeItem* item = new ManageTrackFindONVIFDeviceDiscoveryTreeItem(device_, std::vector<std::string>(), std::vector<std::string>(), address, username_, password_);
  item->setData(0, Qt::UserRole, RECEIVERDISCOVERYITEM_DEVICE);
  item->setData(0, Qt::UserRole + 1, QString::fromStdString(address));
  Filter(item);
  addTopLevelItem(item);
}

}
