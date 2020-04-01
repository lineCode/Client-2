// playbackwidget.h
//

#ifndef ID8CJO7DJFLSFJLDSOXH4EQWKTZZSDEDT2
#define ID8CJO7DJFLSFJLDSOXH4EQWKTZZSDEDT2

///// Includes /////

#include <boost/optional.hpp>
#include <QDateTime>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QStaticText>
#include <stdint.h>
#include <vector>

#include "recordingblocks.h"

///// Declarations /////

class QAction;
class QOpenGLShaderProgram;

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class VideoView;
class View;

///// Enumerations /////

enum PLAYBACKMOUSESTATE
{
  PLAYBACKMOUSESTATE_IDLE,
  PLAYBACKMOUSESTATE_MOVESTART,
  PLAYBACKMOUSESTATE_MOVEEND,
  PLAYBACKMOUSESTATE_CLICKED,
  PLAYBACKMOUSESTATE_MOVE,
  PLAYBACKMOUSESTATE_MOVEMARKER
};

///// Glboals /////

extern const qint64 START_EPOCH;
extern const qint64 END_EPOCH;

extern const std::array<float, 8> texturecoords_;

extern const int RECORDINGBLOCKS_HEIGHT;
extern const int PLAYBACK_TEXT_FONT_HEIGHT;
extern const int PLAYBACK_TEXT_BORDER;
extern const int PLAYBACK_TEXTURE_TEXT_WIDTH;
extern const int PLAYBACK_TEXTURE_TEXT_HEIGHT;

///// Structures /////

struct TIMELINEGENERATOR//TODO rename this to something sensible
{
  TIMELINEGENERATOR(const uint64_t topsecincrement,
                    const uint64_t topdayincrement,
                    const uint64_t topmonthincrement,
                    const uint64_t topyearincrement,
                    const uint64_t botsecincrement,
                    const uint64_t botdayincrement,
                    const uint64_t botmonthincrement,
                    QString (*toptext)(const QDateTime&),
                    QString (*bottext)(const QDateTime&),
                    const QDateTime& startdatetime);

  uint64_t topsecincrement_;
  uint64_t topdayincrement_;
  uint64_t topmonthincrement_;
  uint64_t topyearincrement_;
  uint64_t botsecincrement_;
  uint64_t botdayincrement_;
  uint64_t botmonthincrement_;
  QString (*toptext_)(const QDateTime&);
  QString (*bottext_)(const QDateTime&);
  QDateTime startdatetime_;

};

struct TIMELINETEXT
{
  TIMELINETEXT();

  QStaticText text_;
  QPoint position_;
  int width_;
  QDateTime time_;

};

struct PLAYBACKTEXT//TODO hopefully this dies away when QStaticText replaces it
{
  PLAYBACKTEXT();

  uint64_t time_;
  GLuint texture_;
  QOpenGLBuffer texturebuffer_;
  QOpenGLBuffer vertexbuffer_;

};

///// Prototypes /////

TIMELINEGENERATOR TimelineGenerator(const std::pair<uint64_t, uint64_t>& startendtime); // <start, end>
QImage GetTextureTime(const FT_Face freetypearial, const uint64_t exporttime, const float red, const float green, const float blue);

///// Classes /////

class PlaybackWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
 Q_OBJECT

 public:

  PlaybackWidget(QWidget* parent);
  ~PlaybackWidget();

  void Destroy();

  inline uint64_t GetGlobalStartTime() const { return globalstarttime_; }
  inline uint64_t GetGlobalEndTime() const { return globalendtime_; }
  void UpdateRecordingsBlocks();
  
  inline const boost::optional<uint64_t>& GetExportStartTime() const { return exportstarttime_; }
  inline const boost::optional<uint64_t>& GetExportEndTime() const { return exportendtime_; }
  
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

  bool Hit(const uint64_t time, const int top, const int bottom, const QPoint& pos) const;
  bool HitPlayMarker(const QPoint& pos) const;
  void SetState(const PLAYBACKMOUSESTATE state);
  void SetExportStartTime(const uint64_t exportstarttime);
  void SetExportEndTime(const uint64_t exportendtime);
  void UpdateGUIHorizontalLines();
  void UpdateGUITimelines();
  void ZoomIn(const int x);
  void ZoomOut(const int x);
  float GetRecordingBlocksTop() const;
  void Scrub(const std::pair<uint64_t, uint64_t>& startendtime, const int x);

  QAction* actionvideo_;
  QAction* actionmetadata_;

  FT_Library freetype_;
  FT_Face freetypearial_;

  GLint openglmajorversion_;

  QOpenGLShaderProgram* recordingsblocksshader_;
  int recordingsblockspositionlocation_;
  int recordingsblockscolourlocation_;

  QOpenGLShaderProgram* markershader_;
  int markerpositionlocation_;
  int markercolourlocation_;

  QOpenGLShaderProgram* textshader_;
  int textpositionlocation_;
  int texttexturecoordlocation_;
  int texttexturesamplerlocation_;

  std::vector< QSharedPointer<RecordingBlocks> > recordingsblocks_;

  // These are used more of a cache to save processing time
  uint64_t globalstarttime_;
  uint64_t globalendtime_;

  // GUI Elements
  std::pair<uint64_t, uint64_t> prevstartendtime_; // This is used to remember whether we need to update the timeline elements, if it has not changed, we don't need to update the gui
  QOpenGLBuffer guihorizontalvertices_;
  QOpenGLBuffer guitimelinedarkvertices_;
  QOpenGLBuffer guitimelinelightvertices_;
  std::vector<float> guitimelinedarkmarkers_; // This is used as a cache so we don't have to reallocate frequently, and it gives us the size we need when calling Draw
  std::vector<float> guitimelinelightmarkers_; // This is used as a cache so we don't have to reallocate frequently, and it gives us the size we need when calling Draw
  std::vector<TIMELINETEXT> guitimelinetexts_;

  // Green and red marker timers
  QOpenGLBuffer exportstartvertices_;
  QOpenGLBuffer exportendvertices_;
  boost::optional<uint64_t> exportstarttime_;
  boost::optional<uint64_t> exportendtime_;

  PLAYBACKTEXT starttext_;
  PLAYBACKTEXT endtext_;

  PLAYBACKMOUSESTATE state_;
  int clickedpos_; // This is the last clicked position, useful for when moving the timeline
  std::chrono::steady_clock::time_point clickedtime_;

  int timer_;

 public slots:

  void on_buttonframestepbackwards_clicked();
  void on_buttonplay_clicked();
  void on_buttonpause_clicked();
  void on_buttonstop_clicked();
  void on_buttonframestepforwards_clicked();
  void on_buttonzoomin_clicked();
  void on_buttonzoomout_clicked();
  void on_buttonexport_clicked();

 private slots:

  void ViewDestroyed(const QSharedPointer<View>& view);
  void Selected(const QSharedPointer<View>& view, bool select);
  void ShowVideo(bool);
  void ShowMetadata(bool);
  void TimeOffsetChanged(const boost::shared_ptr<Device>& device);
  
};

}

#endif
