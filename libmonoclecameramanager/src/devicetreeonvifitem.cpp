// onvifitem.cpp
//

///// Includes /////

#include "monoclecameramanager/devicetreeonvifitem.h"

#include <onvifclient/deviceclient.hpp>
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QUrl>

///// Namespaces /////

namespace cm
{

///// Globals /////



///// Methods /////

DeviceTreeONVIFItem::DeviceTreeONVIFItem(QTreeWidget* parent, const QString& uri, const QString& username, const QString& password) :
  DeviceTreeItem(parent, { uri }),
  uri_(uri),
  username_(username),
  password_(password),
  hack_(new QAction("Hack", this))
{
  connect(hack_, &QAction::triggered, this, &DeviceTreeONVIFItem::Hack);

  const QUrl url(uri);
  if (!url.isValid())
  {

    return;
  }

  setText(0, url.host() + ":" + QString::number(url.port(80)));

  setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
}

DeviceTreeONVIFItem::~DeviceTreeONVIFItem()
{
  hackconnection_.Close();

}

void DeviceTreeONVIFItem::Expanded()
{
  //TODO clear everything
  //TODO "Loading..." child

  connection_.Close();
  device_ = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
  if (hackdevice_->Init(sock::ProxyParams(), uri_.toStdString(), username_.toStdString(), password_.toStdString(), 1, false, true))
  {
    //TODO failure child
    return;
  }

  //TODO GetCapabilities() I think

  //TODO GetProfiles()
    //TODO when a user clicks on a profile, it updates the widget on the right hand panel

}

void DeviceTreeONVIFItem::Collapsed()
{
  connection_.Close();
  device_.reset();
  media_.reset();
}

void DeviceTreeONVIFItem::DoubleClicked()
{
  Expanded();

}

void DeviceTreeONVIFItem::ContextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = new QMenu(treeWidget());
  //TODO edit menu, which allows us to change address and username/password
    //TODO if we are expanded, then reconnect go back to Loading...
  menu->addAction(hack_);
  menu->exec(event->globalPos());
}

void DeviceTreeONVIFItem::HackGetUsers(std::vector< std::pair<std::string, std::string> >& credentials)
{
  if (credentials.empty())
  {
    //TODO failure
    return;
  }

  hackdevice_->SetUsername(credentials.back().first);
  hackdevice_->SetPassword(credentials.back().second);
  credentials.pop_back();

  hackconnection_ = hackdevice_->GetUsersCallback([this, credentials](const onvif::device::GetUsersResponse& response) mutable // GetUsers should definitely require authentication so it is a good test
  {
    if (!hackdevice_ || !hackdevice_->IsInitialised())
    {
      // Someone killed us
      return;
    }

    if (response.Error())//TODO err dunno... do we just crack on?
    {
      HackGetUsers(credentials);

    }
    else
    {
      username_ = QString::fromStdString(hackdevice_->GetUsername());
      password_ = QString::fromStdString(hackdevice_->GetPassword());
      //TODO success or something? change icon or kick off some new connection?
    }
  });
}

void DeviceTreeONVIFItem::Hack(bool)
{
  hackconnection_.Close();
  hackdevice_ = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
  if (hackdevice_->Init(sock::ProxyParams(), uri_.toStdString(), username_.toStdString(), password_.toStdString(), 1, false, true))
  {
    //TODO QMessageBox error?
    return;
  }

  // Gather up potential username and password combinations
  std::vector< std::pair<std::string, std::string> > credentials;//TODO grab these from somewhere...
  if (username_.size() || password_.size())
  {
    credentials.push_back(std::make_pair(username_.toStdString(), password_.toStdString()));

  }
  credentials.push_back(std::make_pair("test", "test"));
  credentials.push_back(std::make_pair("admin", "Zoggins6"));

  hackconnection_ = hackdevice_->GetSystemDateAndTimeCallback([this, credentials](const onvif::device::GetSystemDateAndTimeResponse& response) mutable
  {
    if (!hackdevice_ || !hackdevice_->IsInitialised())
    {
      // Someone killed us
      return;
    }

    if (response.Error())
    {
      // We just ignore this I think

    }

    HackGetUsers(credentials);
  });
}

void DeviceTreeONVIFItem::Update()
{
  if (hackdevice_)
  {
    if (hackdevice_->Update())
    {
      //TODO QMessageBox
      hackdevice_.reset();
    }
  }
}

}
