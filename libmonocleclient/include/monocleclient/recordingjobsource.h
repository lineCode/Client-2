// recordingjobsource.h
//

#ifndef IDLIY0KDSFSFJKNVZNKKKRYZ1RUCV1EPVI
#define IDLIY0KDSFSFJKNVZNKKKRYZ1RUCV1EPVI

///// Includes /////

#include <QObject>
#include <QSharedPointer>

///// Declarations /////

namespace monocle
{
enum class TrackType : int8_t;
enum class RecordingJobState : int8_t;
}

///// Namespaces /////

namespace client
{

///// Declarations /////

class Receiver;
class RecordingJobSourceTrack;
class RecordingTrack;
enum class ROTATION : int;

///// Classes /////

class RecordingJobSource : public QObject
{
 Q_OBJECT

 public:

  RecordingJobSource(const uint64_t token, const uint64_t receivertoken);
  ~RecordingJobSource();

  void AddTrack(const QSharedPointer<client::RecordingJobSourceTrack>& track);
  void RemoveTrack(const uint64_t token);
  
  inline const std::vector< QSharedPointer<client::RecordingJobSourceTrack> >& GetTracks() const { return tracks_; }
  inline std::vector< QSharedPointer<client::RecordingJobSourceTrack> >& GetTracks() { return tracks_; }
  std::vector < QSharedPointer<client::RecordingJobSourceTrack> > GetTracks(const monocle::TrackType tracktype);
  QSharedPointer<client::RecordingJobSourceTrack> GetTrack(const uint64_t tracktoken) const;
  monocle::RecordingJobState GetState(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<uint64_t> GetReceivers(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<QString> GetActiveProfileTokens(const QSharedPointer<client::RecordingTrack>& track) const;
  std::vector<ROTATION> GetActiveRotations(const QSharedPointer<client::RecordingTrack>& track) const;

  inline uint64_t GetToken() const { return token_; }
  inline void SetReceiverToken(const uint64_t receivertoken) { receivertoken_ = receivertoken; }
  inline uint64_t GetReceiverToken() const { return receivertoken_; }

 private:

  uint64_t token_;
  uint64_t receivertoken_;
  std::vector< QSharedPointer<client::RecordingJobSourceTrack> > tracks_;

 signals:


};

}

#endif
