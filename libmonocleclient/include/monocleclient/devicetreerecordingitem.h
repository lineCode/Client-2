// devicetreerecordingitem.h
//

#ifndef ID83Q9PV264SXXXXKB2NWZNKI7PSMMVVVV
#define ID83Q9PV264SXXXXKB2NWZNKI7PSMMVVVV

///// Includes /////

#include "monocleclient/devicetreeitem.h"

#include <monocleprotocol/client/connection.hpp>
#include <QAction>
#include <QIcon>
#include <QSharedPointer>

///// Declarations /////

namespace monocle { enum class RecordingJobState : int8_t; }

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class DeviceTree;
class DeviceTreeItem;
class DeviceTreeRecordingTrackItem;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class DeviceTreeRecordingItem : public DeviceTreeItem
{
 Q_OBJECT

 public:

  DeviceTreeRecordingItem(DeviceTreeItem* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QIcon& recordingicon, const QIcon& cameraicon);
  virtual ~DeviceTreeRecordingItem();

  virtual void ContextMenuEvent(const QPoint& pos) override;

  virtual void Expanded() override;
  virtual void Collapsed() override;

  virtual void DoubleClicked() override;

  void SetFilter(const QString& filter);

  inline const boost::shared_ptr<Device> GetDevice() const { return device_; }
  inline const QSharedPointer<client::Recording>& GetRecording() const { return recording_; }


 protected:




 private:

  void UpdateToolTip();
  void UpdateChildren();
  DeviceTreeRecordingTrackItem* GetChild(const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& track) const;
  bool Exists(const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& track) const;
  QString Tooltip(const QString& mediauri, const QString& status) const;

  bool operator<(const QTreeWidgetItem& rhs) const;

  boost::shared_ptr<Device> device_;
  QSharedPointer<client::Recording> recording_;

  const QIcon& cameraicon_;

  QAction* edit_;
  QAction* addvideotrack_;
  QAction* remove_;
  QAction* viewlog_;

  monocle::client::Connection removerecordingconnection_;



 public slots:

  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void JobSourceTrackAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void JobSourceTrackRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);
  void ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob);
  void RecordingJobSourceAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource);
  void RecordingJobSourceRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t token);
  void RecordingJobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, const uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate);
  void DataRate();
  void Edit(bool);
  void AddVideoTrack(bool);
  void Remove(bool);
  void ViewLog(bool);

};

}

#endif
