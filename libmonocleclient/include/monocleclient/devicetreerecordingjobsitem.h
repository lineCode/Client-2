// devicetreerecordingjobsitem.h
//

#ifndef ID83QDSFDSFSFD45454547775SSKI7PSMM
#define ID83QDSFDSFSFD45454547775SSKI7PSMM

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
class RecordingJob;

///// Classes /////

class DeviceTreeRecordingJobsItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeRecordingJobsItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording);
  ~DeviceTreeRecordingJobsItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;



 protected:



 private:

  void AddRecordingJob(const QSharedPointer<client::RecordingJob>& recordingjob);

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;

  QAction* manage_;
  


 private slots:

  void Manage(bool);
  void RecordingJobAdded(const QSharedPointer<client::RecordingJob>& recordingjob);
  void RecordingJobRemoved(const uint64_t token);


};

}

#endif
