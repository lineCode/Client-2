// recordingblock.h
//

#ifndef IDG4KFGDFJKESFSDFSDFNBNBXXXCRYVBT4
#define IDG4KFGDFJKESFSDFSDFNBNBXXXCRYVBT4

///// Includes /////

#include <boost/optional.hpp>
#include <map>
#include <memory>
#include <monocleprotocol/index_generated.h>
#include <QObject>
#include <QOpenGLBuffer>
#include <QSharedPointer>
#include <stdint.h>
#include <vector>

///// Declarations /////

namespace monocle
{

enum class RecordingJobState : int8_t;

}

///// Namespaces /////

namespace client
{

///// Declarations /////

class PlaybackWidget;
class RecordingBlock;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;
class View;

///// Classes /////

class RecordingBlocks : public QObject
{
 Q_OBJECT

 public:

  RecordingBlocks(PlaybackWidget* playbackwidget, const QSharedPointer<View>& view);
  ~RecordingBlocks();

  void Init();
  void Destroy();

  void Update(const float top, const float bottom, const float metadatatop, const float metadatabottom, const float minwidth, const uint64_t starttime, const uint64_t endtime);

  inline const QSharedPointer<View>& GetView() const { return view_; }
  inline QSharedPointer<View>& GetView() { return view_; }
  inline float GetTop() const { return top_; }
  inline float GetBottom() const { return bottom_; }
  inline size_t GetNumRecordingBlockTriangles() const { return (recordingblockverticesdata_.size()); }
  inline size_t GetNumMetadataRecordingBlockTriangles() const { return (metadatarecordingblockverticesdata_.size()); }
  inline QOpenGLBuffer& GetRecordingBlockVertices() { return recordingblockvertices_; }
  inline QOpenGLBuffer& GetMetadataRecordingBlockVertices() { return metadatarecordingblockvertices_; }
  inline const std::map< uint32_t, std::vector< std::unique_ptr<RecordingBlock> > >& GetRecordingTracks() { return recordingtracks_; }
  inline QOpenGLBuffer& GetPlayMarkerVertices() { return playmarkervertices_; }
  uint64_t GetPlayMarkerTime() const;

  boost::optional<uint64_t> GetStartTime() const;
  boost::optional<uint64_t> GetEndTime() const;
  
 private:

  std::vector< std::unique_ptr<RecordingBlock> > InitRecordingBlocks(const QSharedPointer<client::RecordingTrack> track) const;

  PlaybackWidget* playbackwidget_;
  QSharedPointer<View> view_;
  float top_;
  float bottom_;
  float metadatatop_;
  float metadatabottom_;
  float minwidth_;

  std::vector<float> recordingblockverticesdata_;
  QOpenGLBuffer recordingblockvertices_;

  std::vector<float> metadatarecordingblockverticesdata_;
  QOpenGLBuffer metadatarecordingblockvertices_;

  std::map< uint32_t, std::vector< std::unique_ptr<RecordingBlock> > > recordingtracks_; // <trackindex, recordingblocks> These are stored in order from earliest to latest in the vector

  QOpenGLBuffer playmarkervertices_;

 private slots:

  void ChangeTrack(const QSharedPointer<client::RecordingTrack>& track);
  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void JobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate);
  void TrackSetData(const QSharedPointer<client::RecordingTrack>& track, const std::vector<monocle::INDEX>& indices);
  void TrackDeleteData(const QSharedPointer<client::RecordingTrack>& track, const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);

};

}

#endif
