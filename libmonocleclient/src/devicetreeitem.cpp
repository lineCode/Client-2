// devicetreeitem.cpp
//

///// Includes /////

#include "monocleclient/devicetreeitem.h"

#include "monocleclient/devicetree.h"
#include "monocleclient/devicetreerecordingitem.h"
#include "monocleclient/recording.h"

///// Namespaces /////

namespace client
{

///// Methods /////

DeviceTreeItem::DeviceTreeItem(DeviceTree* parent, const QString& name, const int type) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ name })), type)
{

}

DeviceTreeItem::DeviceTreeItem(DeviceTreeItem* parent, const QString& name, const int type) :
  QTreeWidgetItem(parent, QStringList(std::initializer_list<QString>({ name })), type)
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

DeviceTreeRecordingItem* DeviceTreeItem::GetRecordingItem(const uint64_t recordingtoken) const
{
  for (int i = 0; i < childCount(); ++i)
  {
    QTreeWidgetItem* item = child(i);
    if (item->type() != DEVICE_TREE_TOP_LEVEL_ITEM_TYPE::DEVICE_RECORDING)
    {

      continue;
    }

    DeviceTreeRecordingItem* recordingitem = static_cast<DeviceTreeRecordingItem*>(item);
    if (recordingitem->GetRecording()->GetToken() == recordingtoken)
    {

      return recordingitem;
    }
  }
  return nullptr;
}

}
