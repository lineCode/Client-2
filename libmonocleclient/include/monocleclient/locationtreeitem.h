// locationtreeitem.h
//

#ifndef IDMAJ4UDC2J0XMXE45454545AOMFX2F5I0
#define IDMAJ4UDC2J0XMXE45454545AOMFX2F5I0

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <memory>
#include <QObject>
#include <QStringList>
#include <QTreeWidget>

#include "monocleclient/connection.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class LocationTree;
class LocationTreeItem;
class Map;
class Recording;

///// Classes /////

class LocationTreeItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT

 public:
 
  LocationTreeItem(LocationTree* parent, const QString& name, const boost::shared_ptr<Device>& device, const QIcon& recordingicon);
  LocationTreeItem(LocationTreeItem* parent, const QString& name, const boost::shared_ptr<Device>& device, const QIcon& recordingicon);
  virtual ~LocationTreeItem();

  virtual void ContextMenuEvent(const QPoint& pos);

  virtual void Expanded();
  virtual void Collapsed();
  
  virtual void DoubleClicked();

  void SetFilter(const QString& filter);

  void MapAdded(QStringList locationcomponents, const QSharedPointer<Map>& map, const QIcon& mapicon);
  void MapRemoved(const uint64_t token);
  void RecordingAdded(QStringList locationcomponents, const QSharedPointer<Recording>& recording);
  void RecordingRemoved(const uint64_t token);

 protected:

  void Clear();
 
 private:

  LocationTreeItem* GetChild(const QString& name) const;

  const QIcon& recordingicon_;

 private slots:

  void GuiOrderChanged(const boost::shared_ptr<Device>& device, const std::vector<GUIORDER>& recordingsorder, const std::vector<GUIORDER>& mapsorder);
  
};

}

#endif
