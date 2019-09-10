// devicetreerecordingjobitem.h
//

#ifndef ID83QD232367676767672323XXXKI7PSMM
#define ID83QD232367676767672323XXXKI7PSMM

///// Includes /////

#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <stdint.h>
#include <QAction>
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
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class DeviceTreeRecordingJobItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeRecordingJobItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob);
  ~DeviceTreeRecordingJobItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  inline const QSharedPointer<client::RecordingJob>& GetRecordingJob() const { return recordingjob_; }


 protected:



 private:

  void SetToolTip();

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;
  QSharedPointer<client::RecordingJob> recordingjob_;

  QAction* edit_;
  QAction* remove_;
  QAction* viewlog_;

  monocle::client::Connection removerecordingjobconnection_;


 private slots:

  void Edit(bool);
  void Remove(bool);
  void ViewLog(bool);
  void JobChanged(const QSharedPointer<client::RecordingJob>& recordingjob);
  void ActiveJobChanged(const QSharedPointer<client::RecordingJob>& recordingjob);
  void JobSourceTrackAdded(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void JobSourceTrackRemoved(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);

};

}

#endif
