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

///// Classes /////

class DeviceTreeItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT

 public:
 
  DeviceTreeItem(DeviceTree* parent, const QString& name);
  DeviceTreeItem(DeviceTreeItem* parent, const QString& name);
  virtual ~DeviceTreeItem();

  virtual void ContextMenuEvent(const QPoint& pos);

  virtual void Expanded();
  virtual void Collapsed();
  
  virtual void DoubleClicked();

 protected:

  void Clear();
 
 private:



};

}

#endif
