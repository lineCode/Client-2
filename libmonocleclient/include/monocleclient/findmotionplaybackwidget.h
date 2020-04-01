// findmotionplaybackwidget.h
//

#ifndef IDLIY0KUASDDSFKSDHFSSMVZZRUCV1EPVI
#define IDLIY0KUASDDSFKSDHFSSMVZZRUCV1EPVI

///// Includes /////

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QSharedPointer>
#include <QVector4D>
#include <vector>

#include "monocleclient/playbackwidget.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class Connection;
class Device;
class FindMotionWindow;
class FindMotionVideoWidget;
class RecordingBlock;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class FindMotionPlaybackWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
 friend class FindMotionWindow;
 friend class FindMotionVideoWidget;

 Q_OBJECT

 public:

  FindMotionPlaybackWidget(QWidget* parent);
  virtual ~FindMotionPlaybackWidget();

  void SetTrack(const QSharedPointer<RecordingTrack>& track);
  void SetColour(const QVector4D& colour);

  void UpdateRecordingBlocks();

  void ZoomIn(const int x);
  void ZoomOut(const int x);

  inline const boost::optional<uint64_t> GetExportStartTime() const { return exportstarttime_; }
  inline const boost::optional<uint64_t> GetExportEndTime() const { return exportendtime_; }

  void FindMotionResult(const uint64_t start, const uint64_t end);

  void SetPaused(const bool paused);
  void SetPlayMarkerTime(const uint64_t time);
  void SetFrameTime(const std::chrono::steady_clock::time_point time);
  bool IsPaused();

  const boost::shared_ptr<Connection>& GetConnection() const;
  const boost::shared_ptr<Device>& GetDevice() const;
  FindMotionWindow* GetFindMotionWindow();
  FindMotionVideoWidget* GetFindMotionVideoWidget();

  void SetExportStartTime(const uint64_t exportstarttime, const bool makecurrent);
  void SetExportEndTime(const uint64_t exportendtime, const bool makecurrent);

 protected:

  virtual void initializeGL() override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL() override;

  virtual void timerEvent(QTimerEvent*) override;

  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void wheelEvent(QWheelEvent* event) override;
  virtual void contextMenuEvent(QContextMenuEvent* event) override;

 private:

  inline uint64_t GetStartTime() const { return starttime_; }
  inline uint64_t GetEndTime() const { return endtime_; }
  float GetRecordingBlocksTop() const;
  void UpdateGUIHorizontalLines();
  bool Hit(const uint64_t time, const int x);
  void SetState(const PLAYBACKMOUSESTATE state);

  QAction* actionvideo_;
  QAction* actionmotion_;

  FT_Library freetype_;
  FT_Face freetypearial_;

  QOpenGLShaderProgram recordingsblocksshader_;
  int recordingsblockspositionlocation_;
  int recordingsblockscolourlocation_;

  QOpenGLShaderProgram markershader_;
  int markerpositionlocation_;
  int markercolourlocation_;

  QOpenGLShaderProgram textshader_;
  int textpositionlocation_;
  int texttexturecoordlocation_;
  int texttexturesamplerlocation_;

  QSharedPointer<RecordingTrack> track_;
  QVector4D colour_;

  std::vector< std::unique_ptr<RecordingBlock> > recordingblocks_;
  std::vector< std::unique_ptr<RecordingBlock> > motionrecordingblocks_;

  QOpenGLBuffer guihorizontalvertices_;

  std::vector<float> recordingblockverticesdata_;
  QOpenGLBuffer recordingblockvertices_;

  std::vector<float> metadatarecordingblockverticesdata_;
  QOpenGLBuffer metadatarecordingblockvertices_;

  uint64_t starttime_;
  uint64_t endtime_;

  QOpenGLBuffer guitimelinedarkvertices_;
  QOpenGLBuffer guitimelinelightvertices_;
  std::vector<float> guitimelinedarkmarkers_; // This is used as a cache so we don't have to reallocate frequently, and it gives us the size we need when calling Draw
  std::vector<float> guitimelinelightmarkers_; // This is used as a cache so we don't have to reallocate frequently, and it gives us the size we need when calling Draw
  std::vector<TIMELINETEXT> guitimelinetexts_;

  QOpenGLBuffer playmarkervertices_;

  // Green and red marker timers
  QOpenGLBuffer exportstartvertices_;
  QOpenGLBuffer exportendvertices_;
  boost::optional<uint64_t> exportstarttime_;
  boost::optional<uint64_t> exportendtime_;

  PLAYBACKTEXT starttext_;
  PLAYBACKTEXT endtext_;

  PLAYBACKMOUSESTATE state_;
  int clickedpos_; // This is the last clicked position, useful for when moving the timeline

  int timer_;

 private slots:

  void ShowVideo(bool);
  void ShowMotion(bool);
  void TimeOffsetChanged(const boost::shared_ptr<Device>& device);
  void JobSourceTrackStateChanged(const QSharedPointer<client::RecordingJob>& job, const QSharedPointer<client::RecordingJobSource>& source, const QSharedPointer<client::RecordingJobSourceTrack>& track, uint64_t time, const monocle::RecordingJobState state, const QString& error, const monocle::RecordingJobState prevstate);
  void TrackSetData(const QSharedPointer<client::RecordingTrack>& track, const std::vector<monocle::INDEX>& indices);
  void TrackDeleteData(const QSharedPointer<client::RecordingTrack>& track, const monocle::RecordingJobState state, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);
 
};

}

#endif
