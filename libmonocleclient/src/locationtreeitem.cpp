// locationtreeitem.cpp
//

///// Includes /////

#include "monocleclient/locationtreeitem.h"

#include "monocleclient/locationtree.h"
#include "monocleclient/locationtreemapitem.h"
#include "monocleclient/locationtreerecordingitem.h"
#include "monocleclient/map.h"
#include "monocleclient/recording.h"

///// Namespaces /////

namespace client
{

///// Methods /////

LocationTreeItem::LocationTreeItem(LocationTree* parent, const QString& name, const boost::shared_ptr<Device>& device, const QIcon& recordingicon) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ name }))),
  recordingicon_(recordingicon)
{

}

LocationTreeItem::LocationTreeItem(LocationTreeItem* parent, const QString& name, const boost::shared_ptr<Device>& device, const QIcon& recordingicon) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ name }))),
  recordingicon_(recordingicon)
{

}

LocationTreeItem::~LocationTreeItem()
{

}

void LocationTreeItem::ContextMenuEvent(const QPoint& pos)
{

}

void LocationTreeItem::Expanded()
{

}

void LocationTreeItem::Collapsed()
{

}

void LocationTreeItem::DoubleClicked()
{
  if (isExpanded())
  {
    setExpanded(false);

  }
  else
  {
    setExpanded(true);

  }
}

void LocationTreeItem::SetFilter(const QString& filter)
{
  for (int i = 0; i < childCount(); ++i)
  {
    QTreeWidgetItem* item = child(i);
    if (typeid(*item) == typeid(LocationTreeItem))
    {
      static_cast<LocationTreeItem*>(child(i))->SetFilter(filter);

    }
    else if (typeid(*item) == typeid(LocationTreeMapItem))
    {
      static_cast<LocationTreeMapItem*>(child(i))->SetFilter(filter);

    }
    else if (typeid(*item) == typeid(LocationTreeRecordingItem))
    {
      static_cast<LocationTreeRecordingItem*>(child(i))->SetFilter(filter);

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

void LocationTreeItem::MapAdded(QStringList locationcomponents, const QSharedPointer<Map>& map, const QIcon& mapicon)
{
  if (map.isNull())
  {

    return;
  }

  if (locationcomponents.isEmpty())
  {
    addChild(new LocationTreeMapItem(this, map->GetDevice(), map, mapicon));

  }
  else
  {
    const QString locationcomponent = locationcomponents.front();
    locationcomponents.pop_front();
    LocationTreeItem* child = GetChild(locationcomponent);
    if (child == nullptr)
    {
      child = new LocationTreeItem(this, locationcomponent, map->GetDevice(), recordingicon_);
      addChild(child);
    }
    child->MapAdded(locationcomponents, map, mapicon);
  }
}

void LocationTreeItem::MapRemoved(const uint64_t token)
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    LocationTreeItem* item = static_cast<LocationTreeItem*>(child(i));
    if (strcmp(item->metaObject()->className(), LocationTreeRecordingItem::staticMetaObject.className()) == 0)
    {

      continue;
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeMapItem::staticMetaObject.className()) == 0)
    {
      if (reinterpret_cast<LocationTreeMapItem*>(item)->GetToken() == token)
      {
        delete takeChild(i);

      }
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeItem::staticMetaObject.className()) == 0)
    {
      item->MapRemoved(token);
      if (item->childCount() == 0)
      {
        delete takeChild(i);

      }
    }
  }
}

void LocationTreeItem::RecordingAdded(QStringList locationcomponents, const QSharedPointer<Recording>& recording)
{
  if (recording.isNull())
  {

    return;
  }

  if (locationcomponents.isEmpty())
  {
    addChild(new LocationTreeRecordingItem(this, recording->GetDevice(), recording, recordingicon_));

  }
  else
  {
    const QString locationcomponent = locationcomponents.front();
    locationcomponents.pop_front();
    LocationTreeItem* child = GetChild(locationcomponent);
    if (child == nullptr)
    {
      child = new LocationTreeItem(this, locationcomponent, recording->GetDevice(), recordingicon_);
      addChild(child);
    }
    child->RecordingAdded(locationcomponents, recording);
  }
}

void LocationTreeItem::RecordingRemoved(const uint64_t token)
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    LocationTreeItem* item = static_cast<LocationTreeItem*>(child(i));
    if (strcmp(item->metaObject()->className(), LocationTreeMapItem::staticMetaObject.className()) == 0)
    {

      continue;
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeRecordingItem::staticMetaObject.className()) == 0)
    {
      if (reinterpret_cast<LocationTreeRecordingItem*>(item)->GetToken() == token)
      {
        delete takeChild(i);

      }
    }
    else if (strcmp(item->metaObject()->className(), LocationTreeItem::staticMetaObject.className()) == 0)
    {
      item->RecordingRemoved(token);
      if (item->childCount() == 0)
      {
        delete takeChild(i);

      }
    }
  }
}

void LocationTreeItem::Clear()
{
  for (int i = (childCount()-1); i >= 0; --i)
  {
    removeChild(child(i));

  }
}

LocationTreeItem* LocationTreeItem::GetChild(const QString& name) const
{
  for (int i = 0; i < childCount(); ++i)
  {
    LocationTreeItem* c = static_cast<LocationTreeItem*>(child(i));
    if (strcmp(c->metaObject()->className(), LocationTreeItem::staticMetaObject.className()) != 0)
    {

      continue;
    }

    if (c->text(0) == name)
    {

      return c;
    }
  }
  return nullptr;
}

}
