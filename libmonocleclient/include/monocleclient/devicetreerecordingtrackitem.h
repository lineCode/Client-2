// devicetreerecordingtrackitem.h
//

#ifndef ID83QD232323232323232323XXXKI7PSMM
#define ID83QD232323232323232323XXXKI7PSMM

///// Includes /////

#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <stdint.h>
#include <QIcon>
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

class DeviceTreeRecordingTrackItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeRecordingTrackItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<client::RecordingTrack>& track, const QIcon& cameraicon);
  ~DeviceTreeRecordingTrackItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  inline const boost::shared_ptr<Device>& GetDevice() const { return device_; }
  inline const QSharedPointer<client::Recording>& GetRecording() const { return recording_; }
  inline const QSharedPointer<client::RecordingJobSourceTrack>& GetRecordingJobSourceTrack() const { return recordingjobsourcetrack_; }
  inline const QSharedPointer<client::RecordingTrack>& GetTrack() const { return track_; }


 protected:



 private:

  QString GetName(const QSharedPointer<client::RecordingTrack>& track) const;

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;
  QSharedPointer<client::RecordingJob> recordingjob_;
  QSharedPointer<client::RecordingJobSource> recordingjobsource_;
  QSharedPointer<client::RecordingJobSourceTrack> recordingjobsourcetrack_;
  QSharedPointer<client::RecordingTrack> track_;

  QAction* edit_;
  QAction* remove_;
  QAction* viewlog_;

  monocle::client::Connection removetrackconnection_;

 private slots:

  void Edit(bool);
  void Remove(bool);
  void ViewLog(bool);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);

};

}

#endif
