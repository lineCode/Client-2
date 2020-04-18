// devicetree.cpp
//

///// Includes /////

#include "monoclecameramanager/devicetree.h"

#include "monoclecameramanager/onvifitem.h"

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
}

DeviceTree::~DeviceTree()
{

}

void DeviceTree::ItemExpanded(QTreeWidgetItem* item)
{
  static_cast<ONVIFItem*>(item)->Expanded();

}

void DeviceTree::ItemCollapsed(QTreeWidgetItem* item)
{
  static_cast<ONVIFItem*>(item)->Collapsed();

}

void DeviceTree::DoubleClicked(QTreeWidgetItem* item, int)
{
  static_cast<ONVIFItem*>(item)->DoubleClicked();

}

}
