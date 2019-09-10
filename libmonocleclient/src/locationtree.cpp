// locationtree.cpp
//

///// Includes /////

#include "monocleclient/locationtree.h"

#include <QMimeData>

#include "monocleclient/devicemgr.h"
#include "monocleclient/locationtreeitem.h"
#include "monocleclient/locationtreemapitem.h"
#include "monocleclient/locationtreerecordingitem.h"
#include "monocleclient/map.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/recording.h"

///// Namespaces /////

namespace client
{

///// Methods /////

LocationTree::LocationTree(QWidget* parent) :
  QTreeWidget(parent),
  recordingicon_(":/recording.png"),
  mapicon_(":/map.png"),
  showfilter_(new QAction("Show Filter", this))
{
  showfilter_->setCheckable(true);
  showfilter_->setChecked(true);

  connect(this, &LocationTree::itemExpanded, this, &LocationTree::ItemExpanded);
  connect(this, &LocationTree::itemCollapsed, this, &LocationTree::ItemCollapsed);
  connect(this, &LocationTree::itemDoubleClicked, this, &LocationTree::DoubleClicked);
  connect(showfilter_, &QAction::triggered, this, &LocationTree::ShowFilter);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::DeviceAdded, this, &LocationTree::DeviceAdded);
  connect(&MainWindow::Instance()->GetDeviceMgr(), &DeviceMgr::DeviceRemoved, this, &LocationTree::DeviceRemoved);

  setExpandsOnDoubleClick(false);

  for (boost::shared_ptr<Device>& device : MainWindow::Instance()->GetDeviceMgr().GetDevices())
  {
    DeviceAdded(device);
  
  }
}

LocationTree::~LocationTree()
{

}

void LocationTree::SetFilter(const QString& filter)
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    QTreeWidgetItem* item = topLevelItem(i);
    if (typeid(*item) == typeid(LocationTreeItem))
    {
      static_cast<LocationTreeItem*>(topLevelItem(i))->SetFilter(filter);

    }
    else if (typeid(*item) == typeid(LocationTreeMapItem))
    {
      static_cast<LocationTreeMapItem*>(topLevelItem(i))->SetFilter(filter);

    }
    else if (typeid(*item) == typeid(LocationTreeRecordingItem))
    {
      static_cast<LocationTreeRecordingItem*>(topLevelItem(i))->SetFilter(filter);

    }
  }
}

QMimeData* LocationTree::mimeData(const QList<QTreeWidgetItem*> items) const
{
  if (items.isEmpty())
  {

    return nullptr;
  }
  
  for (QTreeWidgetItem* item : items)
  {
    if (typeid(*item) == typeid(LocationTreeRecordingItem))
    {
      LocationTreeRecordingItem* locationtreerecordingitem = static_cast<LocationTreeRecordingItem*>(item);
      QMimeData* mimedata = new QMimeData();

      QByteArray bytearray;
      const uint64_t deviceidentifier = locationtreerecordingitem->GetDevice()->GetIdentifier();
      bytearray.append(reinterpret_cast<const char*>(&deviceidentifier), sizeof(deviceidentifier));
      mimedata->setData(MIME_DEVICE_TREE_DEVICE_IDENTIFIER, bytearray);

      bytearray.clear();
      const uint64_t recordingtoken = locationtreerecordingitem->GetRecording()->GetToken();
      bytearray.append(reinterpret_cast<const char*>(&recordingtoken), sizeof(recordingtoken));
      mimedata->setData(MIME_DEVICE_TREE_RECORDING_TOKEN, bytearray);
      return mimedata;
    }
  }

  return nullptr;
}

void LocationTree::contextMenuEvent(QContextMenuEvent* event)
{
  QTreeWidgetItem* item = itemAt(event->pos());
  if (item == nullptr)
  {
    QMenu* menu = new QMenu(this);
    showfilter_->setChecked(!MainWindow::Instance()->IsLocationTreeFilterHidden());
    menu->addAction(showfilter_);
    menu->exec(event->globalPos());
  }
}

LocationTreeItem* LocationTree::GetChild(const QString& name) const
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    LocationTreeItem* child = static_cast<LocationTreeItem*>(topLevelItem(i));
    if (strcmp(child->metaObject()->className(), LocationTreeItem::staticMetaObject.className()) != 0)
    {

      continue;
    }

    if (child->text(0) == name)
    {

      return child;
    }
  }
  return nullptr;
}

void LocationTree::ItemExpanded(QTreeWidgetItem* item)
{
  static_cast<LocationTreeItem*>(item)->Expanded();

}

void LocationTree::ItemCollapsed(QTreeWidgetItem* item)
{
  static_cast<LocationTreeItem*>(item)->Collapsed();

}

void LocationTree::DoubleClicked(QTreeWidgetItem* item, int)
{
  static_cast<LocationTreeItem*>(item)->DoubleClicked();

}

void LocationTree::ShowFilter(const bool checked)
{
  MainWindow::Instance()->ShowLocationTreeFilter(checked);

}

void LocationTree::DeviceAdded(const boost::shared_ptr<Device>& device)
{
  if (!device)
  {
  
    return;
  }
  connect(device.get(), &Device::SignalMapAdded, this, &LocationTree::MapAdded);
  connect(device.get(), &Device::SignalMapChanged, this, &LocationTree::MapChanged);
  connect(device.get(), &Device::SignalMapRemoved, this, &LocationTree::MapRemoved);
  connect(device.get(), &Device::SignalRecordingAdded, this, &LocationTree::RecordingAdded);
  connect(device.get(), &Device::SignalRecordingChanged, this, &LocationTree::RecordingChanged);
  connect(device.get(), &Device::SignalRecordingRemoved, this, &LocationTree::RecordingRemoved);
}

void LocationTree::DeviceRemoved(const boost::shared_ptr<Device>& device)
{
  // All recordings and maps should be removed, don't need to do anything I think

}

void LocationTree::MapAdded(const QSharedPointer<Map>& map)
{
  QStringList locationcomponents = map->GetLocation().split('/');
  locationcomponents.removeAll(QString());

  if (locationcomponents.isEmpty())
  {
    addTopLevelItem(new LocationTreeMapItem(this, map->GetDevice(), map, mapicon_));

  }
  else
  {
    const QString locationcomponent = locationcomponents.front();
    locationcomponents.pop_front();
    LocationTreeItem* child = GetChild(locationcomponent);
    if (child == nullptr)
    {
      child = new LocationTreeItem(this, locationcomponent, map->GetDevice(), recordingicon_);
      addTopLevelItem(child);
    }
    child->MapAdded(locationcomponents, map, mapicon_);
  }
}

void LocationTree::MapChanged(const QSharedPointer<Map>& map)
{
  MapRemoved(map->GetToken());
  MapAdded(map);
}

void LocationTree::MapRemoved(const uint64_t token)
{
  for (int i = (topLevelItemCount() - 1); i >= 0; --i)
  {
    LocationTreeItem* item = static_cast<LocationTreeItem*>(topLevelItem(i));
    if (strcmp(item->metaObject()->className(), LocationTreeRecordingItem::staticMetaObject.className()) == 0)
    {

      continue;
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeMapItem::staticMetaObject.className()) == 0)
    {
      if (reinterpret_cast<LocationTreeMapItem*>(item)->GetToken() == token)
      {
        delete takeTopLevelItem(i);

      }
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeItem::staticMetaObject.className()) == 0)
    {
      item->MapRemoved(token);
      if (item->childCount() == 0)
      {
        delete takeTopLevelItem(i);

      }
    }
  }
}

void LocationTree::RecordingAdded(const QSharedPointer<Recording>& recording)
{
  if (recording.isNull())
  {

    return;
  }

  QStringList locationcomponents = recording->GetLocation().split('/');
  locationcomponents.removeAll(QString());

  if (locationcomponents.isEmpty())
  {
    addTopLevelItem(new LocationTreeRecordingItem(this, recording->GetDevice(), recording, recordingicon_));

  }
  else
  {
    const QString locationcomponent = locationcomponents.front();
    locationcomponents.pop_front();
    LocationTreeItem* child = GetChild(locationcomponent);
    if (child == nullptr)
    {
      child = new LocationTreeItem(this, locationcomponent, recording->GetDevice(), recordingicon_);
      addTopLevelItem(child);
    }
    child->RecordingAdded(locationcomponents, recording);
  }
}

void LocationTree::RecordingChanged(const QSharedPointer<Recording>& recording)
{
  RecordingRemoved(recording->GetToken());
  RecordingAdded(recording);
}

void LocationTree::RecordingRemoved(const uint64_t token)
{
  for (int i = (topLevelItemCount() - 1); i >= 0; --i)
  {
    LocationTreeItem* item = static_cast<LocationTreeItem*>(topLevelItem(i));
    if (strcmp(item->metaObject()->className(), LocationTreeMapItem::staticMetaObject.className()) == 0)
    {

      continue;
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeRecordingItem::staticMetaObject.className()) == 0)
    {
      if (reinterpret_cast<LocationTreeRecordingItem*>(item)->GetToken() == token)
      {
        delete takeTopLevelItem(i);

      }
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeItem::staticMetaObject.className()) == 0)
    {
      item->RecordingRemoved(token);
      if (item->childCount() == 0)
      {
        delete takeTopLevelItem(i);

      }
    }
  }
}

}
