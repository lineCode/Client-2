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
  //TODO kick off a connection and say "loading..." while we grab things

}

void DeviceTreeONVIFItem::Collapsed()
{
  //TODO kill it all off

}

void DeviceTreeONVIFItem::DoubleClicked()
{
  //TODO expand it I think anyway?

}

void DeviceTreeONVIFItem::ContextMenuEvent(QContextMenuEvent* event)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(hack_);
  menu->exec(event->globalPos());
}

void DeviceTreeONVIFItem::Hack(bool)
{
  hackconnection_.Close();
  hackdevice_ = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
  if (hackdevice_->Init(sock::ProxyParams(), uri_.toStdString(), username_.toStdString(), password_.toStdString()))
  {
    //TODO QMessageBox error?
    return;
  }

  // Gather up all 

  hackconnection_ = hackdevice_->GetSystemDateAndTimeCallback([this](const onvif::device::GetSystemDateAndTimeResponse& response)
  {
    if (!hackdevice_ || hackdevice_->IsInitialised())
    {
      // Someone killed us
      return;
    }

    if (response.Error())//TODO err dunno... do we just crack on?
    {

    }

    //TODO pick a username/password combo off the list

    hackconnection_ = hackdevice_->GetUsersCallback([this](const onvif::device::GetUsersResponse& response) // This should definitely require authentication
    {
      if (!hackdevice_ || hackdevice_->IsInitialised())
      {
        // Someone killed us
        return;
      }

      if (response.Error())//TODO err dunno... do we just crack on?
      {
        //TODO peel one off and try again(probably need to save the lambda or put it into a method

      }
      else
      {
        //TODO hurray username/password

      }
    });
  });

  //TODO bag up a load of usernames/passwords(including username_, password_ if they are valid)

  //TODO once we are successful in finding *a* username/password, set username_ and password_ and just grab things

  int i = 0;//TODO

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
