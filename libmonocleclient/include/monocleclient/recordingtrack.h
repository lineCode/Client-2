// recordingtrack.h
//

#ifndef IDFMKGWV9DFJDKSRFJSACCDCDC366BC609
#define IDFMKGWV9DFJDKSRFJSACCDCDC366BC609

///// Includes /////

#include <boost/optional.hpp>
#include <monocleprotocol/index_generated.h>
#include <monocleprotocol/monocleprotocol.hpp>
#include <QObject>
#include <QSharedPointer>
#include <vector>

///// Declarations /////

namespace monocle
{

enum class RecordingJobState : int8_t;
enum class TrackType : int8_t;

}

///// Namespaces /////

namespace client
{

///// Declarations /////

class File;
class Recording;
class RecordingTrack;

///// Classes /////

class RecordingTrack : public QObject
{
 Q_OBJECT

 public:

  RecordingTrack(const uint32_t id, const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector< std::pair<uint64_t, uint64_t> >& indices, const std::vector<monocle::CODECINDEX>& codecindices);
  ~RecordingTrack();

  void ChangeTrack(const QString& token, const monocle::TrackType tracktype, const QString& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices);
  void SetData(const std::vector<monocle::INDEX>& indices);
  void DeleteData(const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);
  void SetState(const uint64_t time, const monocle::RecordingJobState state, const monocle::RecordingJobState prevstate);
  
  inline uint32_t GetId() const { return id_; }
  inline void SetToken(const QString& token) { token_ = token; }
  inline const QString& GetToken() const { return token_; }
  inline void SetTrackType(const monocle::TrackType tracktype) { tracktype_ = tracktype; }
  inline monocle::TrackType GetTrackType() const { return tracktype_; }
  inline void SetDescription(const QString& description) { description_ = description; }
  inline const QString& GetDescription() const { return description_; }
  inline void SetFixedFiles(const bool fixedfiles) { fixedfiles_ = fixedfiles; }
  inline bool GetFixedFiles() const { return fixedfiles_; }
  inline void SetDigitalSigning(const bool digitalsigning) { digitalsigning_ = digitalsigning; }
  inline bool GetDigitalSigning() const { return digitalsigning_; }
  inline void SetEncrypt(const bool encrypt) { encrypt_ = encrypt; }
  inline bool GetEncrypt() const { return encrypt_; }
  inline void SetFlushFrequency(const uint32_t flushfrequency) { flushfrequency_ = flushfrequency; }
  inline uint32_t GetFlushFrequency() const { return flushfrequency_; }
  inline const std::vector<uint64_t>& GetFiles() const { return files_; }
  inline const std::vector< std::pair<uint64_t, uint64_t> >& GetIndices() const { return indices_; } // If any RecordingJobSourceTrack is actively recording to this track, the final index in this list should be extended to the current time by the caller. The value stored here is the one that was retrieved at a single point
  inline const std::vector<monocle::CODECINDEX>& GetCodecIndices() const { return codecindices_; }
  const std::vector<monocle::CODECINDEX> GetCodecIndices(const monocle::Codec id) const;
  bool HasFile(const uint64_t file) const;
  
 protected:



 private:

  uint32_t id_;
  QString token_;
  monocle::TrackType tracktype_;
  QString description_;
  bool fixedfiles_;
  bool digitalsigning_;
  bool encrypt_;
  uint32_t flushfrequency_;
  std::vector<uint64_t> files_;
  std::vector< std::pair<uint64_t, uint64_t> > indices_;
  std::vector<monocle::CODECINDEX> codecindices_;
 
 signals:

  


};

}

#endif
