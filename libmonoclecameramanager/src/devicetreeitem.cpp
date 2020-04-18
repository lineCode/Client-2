// onvifitem.cpp
//

///// Includes /////

#include "monoclecameramanager/devicetreeitem.h"

///// Namespaces /////

namespace cm
{

///// Globals /////



///// Methods /////

DeviceTreeItem::DeviceTreeItem(QTreeWidget* parent, const QString& name) :
  QTreeWidgetItem(parent, { name })
{

}

DeviceTreeItem::DeviceTreeItem(QTreeWidgetItem* parent, const QString& name) :
  QTreeWidgetItem(parent, { name })
{

}

  DeviceTreeItem::~DeviceTreeItem()
{

}

void DeviceTreeItem::Expanded()
{

}

void DeviceTreeItem::Collapsed()
{

}

void DeviceTreeItem::DoubleClicked()
{

}

void DeviceTreeItem::ContextMenuEvent(QContextMenuEvent* event)
{

}

void DeviceTreeItem::Update()
{

}

void DeviceTreeItem::Clear()
{
  for (int i = (childCount() - 1); i >= 0; --i)
  {
    auto b = child(i);//TODO remove?
    removeChild(child(i));
    //TODO delete b;
  }
}

}
