// devicetreerecordingjobsourceitem.h
//

#ifndef ID83QD23FASFASDASDSADSDADDDDI7PSMM
#define ID83QD23FASFASDASDSADSDADDDDI7PSMM

///// Includes /////

#include "devicetreeitem.h"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <QAction>

///// Declarations /////

class QAction;

namespace monocle
{

enum class RecordingJobState : int8_t;

}

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;

///// Classes /////

class DeviceTreeRecordingJobSourceItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeRecordingJobSourceItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  ~DeviceTreeRecordingJobSourceItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  inline const QSharedPointer<client::RecordingJobSource>& GetRecordingJobSource() const { return recordingjobsource_; }
  inline const QSharedPointer<client::RecordingJobSourceTrack>& GetRecordingJobSourceTrack() const { return recordingjobsourcetrack_; }


 protected:



 private:

  QString GetName(const boost::shared_ptr<Device>& device, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack) const;
  void UpdateToolTip();

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;
  QSharedPointer<client::RecordingJob> recordingjob_;
  QSharedPointer<RecordingJobSource> recordingjobsource_;
  QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack_;

  QAction* manage_;
  QAction* edit_;
  QAction* remove_;
  QAction* viewlog_;

  monocle::client::Connection removerecordingjobsourceconnection_;


 private slots:

  void Manage(bool);
  void Edit(bool);
  void Remove(bool);
  void ViewLog(bool);
  void ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob);
  void SourceTrackStateChanged(const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate);

};

}

#endif
