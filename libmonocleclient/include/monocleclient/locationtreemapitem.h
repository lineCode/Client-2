// locationtreemapitem.h
//

#ifndef IDMAJ4UDC2J0XMXEASDSDS45AOMFX2F5I0
#define IDMAJ4UDC2J0XMXEASDSDS45AOMFX2F5I0

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <memory>
#include <QObject>
#include <QSharedPointer>
#include <QTreeWidget>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class LocationTree;
class LocationTreeItem;
class Map;

///// Classes /////

class LocationTreeMapItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT

 public:
 
  LocationTreeMapItem(LocationTree* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Map>& map, const QIcon& mapicon);
  LocationTreeMapItem(LocationTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Map>& map, const QIcon& mapicon);
  virtual ~LocationTreeMapItem();

  virtual void ContextMenuEvent(const QPoint& pos);

  virtual void Expanded();
  virtual void Collapsed();
  
  virtual void DoubleClicked();

  void SetFilter(const QString& filter);

  uint64_t GetToken() const;

 protected:

  void Clear();
 
 private:

  boost::shared_ptr<Device> device_;
  QSharedPointer<Map> map_;



};

}

#endif
