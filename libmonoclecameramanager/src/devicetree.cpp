// devicetree.cpp
//

///// Includes /////

#include "monoclecameramanager/devicetree.h"

#include <QContextMenuEvent>

#include "monoclecameramanager/devicetreeitem.h"

///// Namespaces /////

namespace cm
{

///// Globals /////



///// Methods /////

DeviceTree::DeviceTree(QWidget* parent) :
  QTreeWidget(parent)
{
  connect(this, &DeviceTree::itemExpanded, this, &DeviceTree::ItemExpanded);
  connect(this, &DeviceTree::itemCollapsed, this, &DeviceTree::ItemCollapsed);
  connect(this, &DeviceTree::itemDoubleClicked, this, &DeviceTree::DoubleClicked);

  startTimer(std::chrono::milliseconds(60));
}

DeviceTree::~DeviceTree()
{

}

void DeviceTree::ItemExpanded(QTreeWidgetItem* item)
{
  static_cast<DeviceTreeItem*>(item)->Expanded();

}

void DeviceTree::ItemCollapsed(QTreeWidgetItem* item)
{
  static_cast<DeviceTreeItem*>(item)->Collapsed();

}

void DeviceTree::DoubleClicked(QTreeWidgetItem* item, int)
{
  static_cast<DeviceTreeItem*>(item)->DoubleClicked();

}

void DeviceTree::contextMenuEvent(QContextMenuEvent* event)
{
  QTreeWidgetItem* item = itemAt(event->pos());
  if (item)
  {
    static_cast<DeviceTreeItem*>(item)->ContextMenuEvent(event);

  }
}

void DeviceTree::timerEvent(QTimerEvent* event)
{
  for (int i = 0; i < topLevelItemCount(); ++i)
  {
    static_cast<DeviceTreeItem*>(topLevelItem(i))->Update();

  }
}

}
