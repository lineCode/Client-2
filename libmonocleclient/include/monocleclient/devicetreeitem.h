// devicetreeitem.h
//

#ifndef IDMAJ4UDC2J0XMXEOVXBS30UAOMFX2F5I0
#define IDMAJ4UDC2J0XMXEOVXBS30UAOMFX2F5I0

///// Includes /////

#include <memory>
#include <QObject>
#include <QTreeWidget>

///// Namespaces /////

namespace client
{

///// Declarations /////

class DeviceTree;
class DeviceTreeMapItem;
class DeviceTreeRecordingItem;

///// Enumerations /////

enum DEVICE_TREE_ITEM_TYPE : int
{
  // Avoid zero, because that is the default
  DEVICE = 1,
  DEVICE_MAP = 2,
  DEVICE_RECORDING = 3,
  DEVICE_RECORDING_TRACK = 4,
  MEDIA = 5,
  MEDIA_RECORDING = 6,
  MEDIA_RECORDING_TRACK = 7,
};

///// Classes /////

class DeviceTreeItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT

 public:
 
  DeviceTreeItem(DeviceTree* parent, const QString& name, const int type);
  DeviceTreeItem(DeviceTreeItem* parent, const QString& name, const int type);
  virtual ~DeviceTreeItem();

  virtual void ContextMenuEvent(const QPoint& pos);

  virtual void Expanded();
  virtual void Collapsed();
  
  virtual void DoubleClicked();

  DeviceTreeMapItem* GetMapItem(const uint64_t maptoken) const;
  DeviceTreeRecordingItem* GetRecordingItem(const uint64_t recordingtoken) const;

 protected:

  void Clear();
 
 private:



};

}

#endif
