// devicetreerecordingtracksitem.h
//

#ifndef ID83QDSFDSFSFD454545454ZXXXKI7PSMM
#define ID83QDSFDSFSFD454545454ZXXXKI7PSMM

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <memory>
#include <stdint.h>
#include <QSharedPointer>
#include <QString>

#include "devicetreeitem.h"

///// Declarations /////

class QAction;

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Recording;
class RecordingTrack;

///// Classes /////

class DeviceTreeRecordingTracksItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeRecordingTracksItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording);
  ~DeviceTreeRecordingTracksItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;



 protected:



 private:

  void AddTrack(const QSharedPointer<client::RecordingTrack>& track);

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;

  QAction* manage_;


 private slots:

  void Manage(bool);
  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint64_t id);

};

}

#endif
