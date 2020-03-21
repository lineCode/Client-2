// locationtree.h
//

#ifndef IDSWJ02KDC3YZ4354355345555FQ0RGCLM
#define IDSWJ02KDC3YZ4354355345555FQ0RGCLM

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <memory>
#include <QIcon>
#include <QSharedPointer>
#include <QTreeWidget>

///// Declarations /////

class QMimeData;

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class LocationTreeItem;
class Map;
class Media;
class Recording;

///// Classes /////

class LocationTree : public QTreeWidget
{
 Q_OBJECT

 public:

  LocationTree(QWidget* parent);
  virtual ~LocationTree();

  void SetFilter(const QString& filter);

 protected:

  virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const override;
  virtual void contextMenuEvent(QContextMenuEvent* event);


 private:

  LocationTreeItem* GetChild(const QString& name) const;

  const QIcon recordingicon_;
  const QIcon mapicon_;

  QAction* showfilter_;

 public slots:
 
  void ItemExpanded(QTreeWidgetItem* item);
  void ItemCollapsed(QTreeWidgetItem* item);
  void DoubleClicked(QTreeWidgetItem* item, int);
  void ShowFilter(const bool checked);
  void DeviceAdded(const boost::shared_ptr<Device>& device);
  void DeviceRemoved(const boost::shared_ptr<Device>& device);
  void GuiOrderChanged(const boost::shared_ptr<Device>& device, const std::vector< std::pair<uint64_t, uint64_t> >& recordingsorder, const std::vector< std::pair<uint64_t, uint64_t> >& mapsorder);
  void MapAdded(const QSharedPointer<Map>& map);
  void MapChanged(const QSharedPointer<Map>& map);
  void MapRemoved(const uint64_t token);
  void RecordingAdded(const QSharedPointer<Recording>& map);
  void RecordingChanged(const QSharedPointer<Recording>& map);
  void RecordingRemoved(const uint64_t token);


};

}

#endif
