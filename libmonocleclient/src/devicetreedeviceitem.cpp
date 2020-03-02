// devicetreertspitem.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleclient/devicetreedeviceitem.h"

#include <QMenu>
#include <QMessageBox>

#include "monocleclient/devicemgr.h"
#include "monocleclient/devicepropertieswindow.h"
#include "monocleclient/devicetreemapitem.h"
#include "monocleclient/devicetreerecordingitem.h"
#include "monocleclient/editdevicewindow.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/managedevicewindow.h"
#include "monocleclient/managefileswindow.h"
#include "monocleclient/managegroupswindow.h"
#include "monocleclient/managemapswindow.h"
#include "monocleclient/manageonvifuserswindow.h"
#include "monocleclient/managerecordingswindow.h"
#include "monocleclient/manageuserswindow.h"
#include "monocleclient/options.h"
#include "monocleclient/recording.h"
#include "monocleclient/devicelogwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeDeviceItem::DeviceTreeDeviceItem(DeviceTree* parent, const boost::shared_ptr<Device>& device, const QIcon& latencygreen, const QIcon& latencyyellow, const QIcon& latencyred, const QIcon& latencynone, const QIcon& recordingicon, const QIcon& mapicon) :
  DeviceTreeItem(parent, device->GetName()),
  device_(device),
  latencygreen_(latencygreen),
  latencyyellow_(latencyyellow),
  latencyred_(latencyred),
  latencynone_(latencynone),
  recordingicon_(recordingicon),
  mapicon_(mapicon),
  reconnect_(new QAction("Reconnect", this)),
  edit_(new QAction("Edit", this)),
  remove_(new QAction("Remove", this)),
  manage_(new QAction("Manage", this)),
  managefiles_(new QAction("Manage Files", this)),
  managerecordings_(new QAction("Manage Recordings", this)),
  managemaps_(new QAction("Manage Maps", this)),
  managegroups_(new QAction("Manage Groups", this)),
  manageusers_(new QAction("Manage Users", this)),
  manageonvifusers_(new QAction("Manage ONVIF Users", this)),
  viewlog_(new QAction("View Log", this)),
  properties_(new QAction("Properties", this))
{
  connect(reconnect_, &QAction::triggered, this, &DeviceTreeDeviceItem::Reconnect);
  connect(edit_, &QAction::triggered, this, &DeviceTreeDeviceItem::Edit);
  connect(remove_, &QAction::triggered, this, &DeviceTreeDeviceItem::Remove);
  connect(manage_, &QAction::triggered, this, &DeviceTreeDeviceItem::Manage);
  connect(managefiles_, &QAction::triggered, this, &DeviceTreeDeviceItem::ManageFiles);
  connect(managerecordings_, &QAction::triggered, this, &DeviceTreeDeviceItem::ManageRecordings);
  connect(managemaps_, &QAction::triggered, this, &DeviceTreeDeviceItem::ManageMaps);
  connect(managegroups_, &QAction::triggered, this, &DeviceTreeDeviceItem::ManageGroups);
  connect(manageusers_, &QAction::triggered, this, &DeviceTreeDeviceItem::ManageUsers);
  connect(manageonvifusers_, &QAction::triggered, this, &DeviceTreeDeviceItem::ManageONVIFUsers);
  connect(viewlog_, &QAction::triggered, this, &DeviceTreeDeviceItem::ViewLog);
  connect(properties_, &QAction::triggered, this, &DeviceTreeDeviceItem::Properties);
  connect(device_.get(), &Device::SignalDisconnected, this, &DeviceTreeDeviceItem::Disconnected);
  connect(device_.get(), &Device::SignalLatency, this, &DeviceTreeDeviceItem::Latency);
  connect(device_.get(), &Device::SignalMapAdded, this, &DeviceTreeDeviceItem::MapAdded);
  connect(device_.get(), &Device::SignalMapChanged, this, &DeviceTreeDeviceItem::MapChanged);
  connect(device_.get(), &Device::SignalMapRemoved, this, &DeviceTreeDeviceItem::MapRemoved);
  connect(device_.get(), &Device::SignalNameChanged, this, &DeviceTreeDeviceItem::NameChanged);
  connect(device_.get(), &Device::SignalRecordingAdded, this, &DeviceTreeDeviceItem::RecordingAdded);
  connect(device_.get(), &Device::SignalRecordingChanged, this, &DeviceTreeDeviceItem::RecordingChanged);
  connect(device_.get(), &Device::SignalRecordingRemoved, this, &DeviceTreeDeviceItem::RecordingRemoved);
  connect(device_.get(), &Device::SignalStateChanged, this, &DeviceTreeDeviceItem::StateChanged);
  connect(device_.get(), &Device::SignalLicenseStateChanged, this, &DeviceTreeDeviceItem::LicenseStateChanged);
  
  setIcon(0, latencynone_);
}

DeviceTreeDeviceItem::~DeviceTreeDeviceItem()
{

}

void DeviceTreeDeviceItem::ContextMenuEvent(const QPoint& pos)
{
  QMenu* menu = new QMenu(treeWidget());
  menu->addAction(reconnect_);
  menu->addAction(edit_);
  menu->addAction(remove_);
  if (device_->GetState() == DEVICESTATE::SUBSCRIBED)
  {
    if (device_->CanManageDevice())
    {
      menu->addAction(manage_);

    }
    if (device_->CanManageRecordings())
    {
      menu->addAction(managefiles_);
      menu->addAction(managerecordings_);
    }

    if (device_->CanManageMaps())
    {
      menu->addAction(managemaps_);

    }

    if (device_->CanManageUsers())
    {
      menu->addAction(managegroups_);
      menu->addAction(manageusers_);
      menu->addAction(manageonvifusers_);
    }

    menu->addAction(viewlog_);
  }
  menu->addAction(properties_);
  menu->exec(pos);
}

void DeviceTreeDeviceItem::Expanded()
{

}

void DeviceTreeDeviceItem::Collapsed()
{

}

void DeviceTreeDeviceItem::DoubleClicked()
{
  DeviceTreeItem::DoubleClicked();

}

void DeviceTreeDeviceItem::SetFilter(const QString& filter)
{
  for (int i = 0; i < childCount(); ++i)
  {
    if (typeid(*child(i)) == typeid(DeviceTreeMapItem))
    {
      static_cast<DeviceTreeMapItem*>(child(i))->SetFilter(filter);

    }
    else if (typeid(*child(i)) == typeid(DeviceTreeRecordingItem))
    {
      static_cast<DeviceTreeRecordingItem*>(child(i))->SetFilter(filter);

    }
  }

  if (filter.isEmpty())
  {
    setHidden(false);

  }
  else
  {
    // If all the children are hidden, then we can hide this one too
    bool hide = true;
    for (int i = 0; i < childCount(); ++i)
    {
      if (!child(i)->isHidden())
      {
        hide = false;
        break;
      }
    }

    setHidden(hide);
  }
}

void DeviceTreeDeviceItem::ResetLatency()
{
  setIcon(0, latencynone_);
  setToolTip(0, QString());
}

void DeviceTreeDeviceItem::SetUnlicensed()
{
  setBackground(0, Qt::darkYellow);
  setToolTip(0, "Invalid license, please insert USB license dongle or remove recordings");
}

void DeviceTreeDeviceItem::Reconnect(bool)
{
  device_->Subscribe();

}

void DeviceTreeDeviceItem::Edit(bool)
{
  EditDeviceWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::Remove(bool)
{
  if (QMessageBox::question(treeWidget(), tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    MainWindow::Instance()->GetDeviceMgr().RemoveDevice(device_);

  }
}

void DeviceTreeDeviceItem::Manage(bool)
{
  ManageDeviceWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ManageFiles(bool)
{
  ManageFilesWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ManageRecordings(bool)
{
  ManageRecordingsWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ManageMaps(bool)
{
  ManageMapsWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ManageGroups(bool)
{
  ManageGroupsWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ManageUsers(bool)
{
  ManageUsersWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ManageONVIFUsers(bool)
{
  ManageONVIFUsersWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::ViewLog(bool)
{
  DeviceLogWindow(treeWidget(), { device_ }).exec();

}

void DeviceTreeDeviceItem::Properties(bool)
{
  DevicePropertiesWindow(treeWidget(), device_).exec();

}

void DeviceTreeDeviceItem::Disconnected()
{
  setIcon(0, latencynone_);

}

void DeviceTreeDeviceItem::Latency(const std::chrono::steady_clock::duration latency)
{
  if (!device_->IsValidLicense())
  {
    SetUnlicensed();
    return;
  }

  const auto latencyms = std::chrono::duration_cast<std::chrono::milliseconds>(latency).count();
  if (latencyms < 1000)
  {
    setIcon(0, latencygreen_);

  }
  else if (latencyms < 3000)
  {
    setIcon(0, latencyyellow_);

  }
  else
  {
    setIcon(0, latencyred_);

  }
  setToolTip(0, QString("Latency: ") + QString::number(latencyms) + QString("ms"));
}

void DeviceTreeDeviceItem::MapAdded(const QSharedPointer<client::Map>& map)
{
  addChild(new DeviceTreeMapItem(this, device_, map, mapicon_));

}

void DeviceTreeDeviceItem::MapChanged(const QSharedPointer<client::Map>& map)
{
  for (int i = 0; i < childCount(); ++i)
  {
    if (typeid(*child(i)) != typeid(DeviceTreeMapItem))
    {

      continue;
    }

    DeviceTreeMapItem* mapitem = static_cast<DeviceTreeMapItem*>(child(i));
    if (mapitem->GetMap() == map)
    {
      mapitem->setText(0, map->GetName());

    }
  }
}

void DeviceTreeDeviceItem::MapRemoved(const uint64_t token)
{
  for (int i = 0; i < childCount(); ++i)
  {
    if (typeid(*child(i)) != typeid(DeviceTreeMapItem))
    {

      continue;
    }

    DeviceTreeMapItem* mapitem = static_cast<DeviceTreeMapItem*>(child(i));
    if (mapitem->GetMap()->GetToken() == token)
    {
      removeChild(mapitem);

    }
  }
}

void DeviceTreeDeviceItem::NameChanged(const QString& name)
{
  setText(0, name);

}

void DeviceTreeDeviceItem::RecordingAdded(QSharedPointer<client::Recording>& recording)
{
  addChild(new DeviceTreeRecordingItem(this, device_, recording, recordingicon_, MainWindow::Instance()->GetCameraIcon()));

}

void DeviceTreeDeviceItem::RecordingChanged(QSharedPointer<client::Recording>& recording)
{
  for (int i = 0; i < childCount(); ++i)
  {
    if (typeid(*child(i)) != typeid(DeviceTreeRecordingItem))
    {

      continue;
    }

    DeviceTreeRecordingItem* recordingitem = static_cast<DeviceTreeRecordingItem*>(child(i));
    if (recordingitem->GetRecording() == recording)
    {
      recordingitem->setText(0, recording->GetName());

    }
  }
}

void DeviceTreeDeviceItem::RecordingRemoved(const uint64_t token)
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    if (typeid(*child(i)) != typeid(DeviceTreeRecordingItem))
    {

      continue;
    }

    DeviceTreeRecordingItem* recordingitem = static_cast<DeviceTreeRecordingItem*>(child(i));
    if (recordingitem->GetRecording()->GetToken() == token)
    {
      removeChild(recordingitem);

    }
  }
}

void DeviceTreeDeviceItem::StateChanged(const DEVICESTATE state, const QString& message)
{
  SetState(state, message);

}

void DeviceTreeDeviceItem::LicenseStateChanged(const bool valid)
{
  SetState(device_->GetState(), toolTip(0));

}

void DeviceTreeDeviceItem::SetState(const DEVICESTATE state, const QString& message)
{
  if (state == DEVICESTATE::SUBSCRIBED)
  {
    if (device_->IsValidLicense())
    {
      setData(0, Qt::BackgroundRole, QVariant());
      setData(0, Qt::ToolTipRole, QVariant());
    }
    else
    {
      SetUnlicensed();

    }
  }
  else
  {
    setBackground(0, Qt::red);
    setToolTip(0, message);
  }
}

}
