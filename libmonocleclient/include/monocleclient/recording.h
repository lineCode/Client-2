// recording.h
//

#ifndef IDLIY0KUASDNSADSAZ26GRYZ1RUCV1EPVI
#define IDLIY0KUASDNSADSAZ26GRYZ1RUCV1EPVI

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "recordingtrack.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Receiver;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
enum class ROTATION : int;

///// Classes /////

class Recording : public QObject
{
 Q_OBJECT

 public:

  Recording(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& sourceid, const QString& name, const QString& location, const QString& description, const QString& address, const QString& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  ~Recording();

  QSharedPointer<client::RecordingJob> GetJob(const uint64_t jobtoken) const;
  QSharedPointer<client::RecordingTrack> GetTrack(const uint32_t trackid) const;
  monocle::RecordingJobState GetState(const QSharedPointer<client::RecordingTrack>& track) const;

  QSharedPointer<client::RecordingTrack> AddTrack(const monocle::RECORDINGTRACK& track);
  QSharedPointer<client::RecordingTrack> ChangeTrack(const uint32_t id, const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices);
  void AddJob(const QSharedPointer<client::RecordingJob>& job);
  QSharedPointer<client::RecordingJob> ChangeJob(const uint64_t token, const QString& name, const bool enabled, const uint64_t priority);
  void RemoveJob(const uint64_t token);
  void RemoveTrack(const uint32_t id);
  inline const std::vector< QSharedPointer<client::RecordingTrack> >& GetTracks() const { return tracks_; }
  inline std::vector< QSharedPointer<client::RecordingTrack> >& GetTracks() { return tracks_; }
  std::vector< QSharedPointer<client::RecordingTrack> > GetTracks(const monocle::TrackType tracktype) const;
  std::vector< QSharedPointer<client::RecordingTrack> > GetVideoTracks() const;
  std::vector< QSharedPointer<client::RecordingTrack> > GetAudioTracks() const;
  std::vector< QSharedPointer<client::RecordingTrack> > GetMetadataTracks() const;
  inline const std::vector< QSharedPointer<client::RecordingJob> >& GetJobs() const { return jobs_; }
  std::vector< QSharedPointer<client::Receiver> > GetActiveReceivers(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<QString> GetActiveProfileTokens(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<ROTATION> GetActiveRotations(const QSharedPointer<client::RecordingTrack>& track) const;
  inline const QSharedPointer<client::RecordingJob>& GetActiveJob() const { return activejob_; }
  size_t GetNumObjectDetectors() const;

  void SetActiveJob(const boost::optional<uint64_t>& activejob);
  void Set(const QString& sourceid, const QString& name, const QString& location, const QString& description, const QString& address, const QString& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  
  inline boost::shared_ptr<Device> GetDevice() const { return device_; }
  inline uint64_t GetToken() const { return token_; }
  inline void SetSourceId(const QString& sourceid) { sourceid_ = sourceid; }
  inline const QString& GetSourceId() const { return sourceid_; }
  inline void SetName(const QString& name) { name_ = name; }
  inline const QString& GetName() const { return name_; }
  inline void SetLocation(const QString& location) { location_ = location; }
  inline const QString& GetLocation() const { return location_; }
  inline void SetDescription(const QString& description) { description_ = description; }
  inline const QString& GetDescription() const { return description_; }
  inline void SetAddress(const QString& address) { address_ = address; }
  inline const QString& GetAddress() const { return address_; }
  inline void SetContent(const QString& content) { content_ = content; }
  inline const QString& GetContent() const { return content_; }
  inline void SetRetentionTime(const uint64_t retentiontime) { retentiontime_ = retentiontime; }
  inline uint64_t GetRetentionTime() const { return retentiontime_; }
  inline const std::vector< QSharedPointer<client::RecordingJob> >& GetRecordingJobs() const { return jobs_; }
  inline std::vector< QSharedPointer<client::RecordingJob> >& GetRecordingJobs() { return jobs_; }
  
 private:

  boost::shared_ptr<Device> device_;
  uint64_t token_;
  QString sourceid_;
  QString name_;
  QString location_;
  QString description_;
  QString address_;
  QString content_;
  uint64_t retentiontime_;

  std::vector< QSharedPointer<client::RecordingTrack> > tracks_;
  std::vector< QSharedPointer<client::RecordingJob> > jobs_;

  QSharedPointer<client::RecordingJob> activejob_;

 signals:

  void ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob);
  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void JobAdded(const QSharedPointer<client::RecordingJob>& job);
  void JobChanged(const QSharedPointer<client::RecordingJob>& job);
  void JobLogMessage(const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t time, const monocle::Severity severity, const QString& message);
  void JobRemoved(const uint64_t token);
  void JobSourceTrackAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void JobSourceTrackChanged(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void JobSourceTrackActiveParametersChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, const std::vector<QString>& activeparameters);
  void JobSourceTrackLogMessage(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack, const uint64_t time, const monocle::Severity severity, const QString& message);
  void JobSourceTrackRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);
  void JobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, const uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate);
  void LogMessage(const uint64_t time, const monocle::Severity severity, const QString& message);
  void JobSourceAdded(const QSharedPointer<client::RecordingJob>& recordingjob, const QSharedPointer<client::RecordingJobSource>& recordingjobsource);
  void JobSourceRemoved(const QSharedPointer<client::RecordingJob>& recordingjob, const uint64_t token);
  void TrackLogMessage(const QSharedPointer<client::RecordingTrack>& recordingtrack, const uint64_t time, const monocle::Severity severity, const QString& message);
  void TrackSetData(const QSharedPointer<client::RecordingTrack> track, const std::vector<monocle::INDEX>& indices);
  void TrackDeleteData(const QSharedPointer<client::RecordingTrack> track, const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);

};

}

#endif
