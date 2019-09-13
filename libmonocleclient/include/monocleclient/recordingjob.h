// recordingjob.h
//

#ifndef IDLIY0KCXNMSAADSAZ26GRYZ1RUCV1EPVI
#define IDLIY0KCXNMSAADSAZ26GRYZ1RUCV1EPVI

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QSharedPointer>

///// Declarations /////

namespace monocle
{
enum class RecordingJobMode : int8_t;
enum class RecordingJobState : int8_t;
}

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class Receiver;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;
enum class ROTATION : int;

///// Classes /////

class RecordingJob : public QObject
{
 Q_OBJECT

 public:

  RecordingJob(const boost::shared_ptr<Device>& device, const uint64_t token, const QString& name, const bool enabled, const uint64_t priority);
  ~RecordingJob();

  void Set(const QString& name, const bool enabled, const uint64_t priority);

  void AddSource(const QSharedPointer<client::RecordingJobSource>& source);

  QSharedPointer<RecordingJobSource> GetSource(const uint64_t sourcetoken) const;
  std::pair< QSharedPointer<RecordingJobSource>, QSharedPointer<client::RecordingJobSourceTrack> > GetSourceTrack(const uint64_t sourcetracktoken) const;
  void RemoveSource(const uint64_t token);
  monocle::RecordingJobState GetState(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<uint64_t> GetReceivers(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<QString> GetActiveProfileTokens(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<ROTATION> GetActiveRotations(const QSharedPointer<client::RecordingTrack>& track) const;
  size_t GetNumObjectDetectors() const;

  inline uint64_t GetToken() const { return token_; }
  inline void SetName(const QString& name) { name_ = name; }
  inline const QString& GetName() const { return name_; }
  inline void SetEnabled(const bool enabled) { enabled_ = enabled; }
  inline bool GetEnabled() const { return enabled_; }
  inline void SetPriority(const uint64_t priority) { priority_ = priority; }
  inline uint64_t GetPriority() const { return priority_; }
  inline const std::vector< QSharedPointer<RecordingJobSource> >& GetSources() const { return sources_; }
  inline std::vector< QSharedPointer<RecordingJobSource> >& GetSources() { return sources_; }

 private:

  boost::shared_ptr<Device> device_;
  uint64_t token_;
  QString name_;
  bool enabled_;
  uint64_t priority_;
  std::vector< QSharedPointer<RecordingJobSource> > sources_;

 signals:

  void SourceTrackAdded(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const QSharedPointer<client::RecordingJobSourceTrack>& recordingjobsourcetrack);
  void SourceTrackRemoved(const QSharedPointer<client::RecordingJobSource>& recordingjobsource, const uint64_t token);

};

}

#endif
