// devicetreeitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreeitem.h"

#include "monocleclient/devicetree.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeItem::DeviceTreeItem(DeviceTree* parent, const QString& name) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ name })))
{

}

DeviceTreeItem::DeviceTreeItem(DeviceTreeItem* parent, const QString& name) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ name })))
{

}

DeviceTreeItem::~DeviceTreeItem()
{

}

void DeviceTreeItem::ContextMenuEvent(const QPoint& pos)
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
  if (isExpanded())
  {
    setExpanded(false);

  }
  else
  {
    setExpanded(true);

  }
}

void DeviceTreeItem::Clear()
{
  for (int i = (childCount()-1); i >= 0; --i)
  {
    removeChild(child(i));

  }
}

}
