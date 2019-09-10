// devicetree.h
//

#ifndef IDSWJ02KDC3YZ2E5DLV06E3Y9JFQ0RGCLM
#define IDSWJ02KDC3YZ2E5DLV06E3Y9JFQ0RGCLM

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <memory>
#include <QIcon>
#include <QList>
#include <QTreeWidget>

///// Declarations /////

class QMimeData;
class QTreeWidgetItem;

///// Namespaces /////

namespace client
{

///// Globals /////

const QString MIME_DEVICE_TREE_DEVICE_IDENTIFIER = "Monocle/DeviceIdentifier";
const QString MIME_DEVICE_TREE_RECORDING_TOKEN = "Monocle/RecordingToken";
const QString MIME_DEVICE_TREE_RECORDING_TRACK_ID = "Monocle/RecordingTrackId";

///// Declarations /////

class Device;
class Media;

///// Classes /////

class DeviceTree : public QTreeWidget
{
 Q_OBJECT

 public:

  DeviceTree(QWidget* parent);
  virtual ~DeviceTree();

  void SetFilter(const QString& filter);

 protected:

  virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const override;
  virtual void contextMenuEvent(QContextMenuEvent* event) override;

 private:

  const QIcon latencygreen_;
  const QIcon latencyyellow_;
  const QIcon latencyred_;
  const QIcon latencynone_;
  const QIcon recordingicon_;
  const QIcon mapicon_;

  QAction* showfilter_;
  QAction* adddevice_;
  QAction* importfile_;

 public slots:
 
  void ItemExpanded(QTreeWidgetItem* item);
  void ItemCollapsed(QTreeWidgetItem* item);
  void DoubleClicked(QTreeWidgetItem* item, int);
  void ShowFilter(bool);
  void AddDevice(bool);
  void ImportFile(bool);
  void DeviceAdded(const boost::shared_ptr<Device>& device);
  void DeviceRemoved(const boost::shared_ptr<Device>& device);
  void MediaAdded(QSharedPointer<Media>& media);
  void MediaRemoved(QSharedPointer<Media>& media);


};

}

#endif
