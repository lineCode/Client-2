// devicetreemapitem.h
//

#ifndef ID83Q9PV264E55454555WZNKI7PSMMVVVV
#define ID83Q9PV264E55454555WZNKI7PSMMVVVV

///// Includes /////

#include "monocleclient/devicetreeitem.h"

#include <monocleprotocol/client/connection.hpp>
#include <QIcon>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Map;

///// Classes /////

class DeviceTreeMapItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeMapItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Map>& map, const QIcon mapicon);
  virtual ~DeviceTreeMapItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  void SetFilter(const QString& filter);

  inline const QSharedPointer<client::Map>& GetMap() const { return map_; }


 protected:




 private:

  bool operator<(const QTreeWidgetItem& rhs) const;

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Map> map_;

  QAction* edit_;
  QAction* remove_;

  monocle::client::Connection removemapconnection_;


 public slots:

  void Edit(bool);
  void Remove(bool);


};

}

#endif
