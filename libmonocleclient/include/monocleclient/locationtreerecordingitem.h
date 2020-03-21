// locationtreerecordingitem.h
//

#ifndef IDMAJ4UDC2J0XMXE458787878OMFX2F5I0
#define IDMAJ4UDC2J0XMXE458787878OMFX2F5I0

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <memory>
#include <QIcon>
#include <QObject>
#include <QTreeWidget>

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class LocationTree;
class LocationTreeItem;
class Recording;

///// Classes /////

class LocationTreeRecordingItem : public QObject, public QTreeWidgetItem
{
 Q_OBJECT

 public:
 
  LocationTreeRecordingItem(LocationTree* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QIcon& recordingicon);
  LocationTreeRecordingItem(LocationTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QIcon& recordingicon);
  virtual ~LocationTreeRecordingItem();

  virtual void ContextMenuEvent(const QPoint& pos);

  virtual void Expanded();
  virtual void Collapsed();
  
  virtual void DoubleClicked();

  void SetFilter(const QString& filter);

  inline const boost::shared_ptr<Device>& GetDevice() const { return device_; }
  inline const QSharedPointer<Recording>& GetRecording() const { return recording_; }

  uint64_t GetToken() const;

 protected:

  void Clear();
 
 private:

  bool operator<(const QTreeWidgetItem& rhs) const;

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;

};

}

#endif
