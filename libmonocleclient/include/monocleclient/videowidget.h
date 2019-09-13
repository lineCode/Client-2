// videowidget.h
//

#ifndef ID8CJO7NQ03L246O2OXH4EQWKTCYMKEDT2
#define ID8CJO7NQ03L246O2OXH4EQWKTCYMKEDT2

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/optional.hpp>
#include <QAction>
#include <QIcon>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QPoint>
#include <QSharedPointer>
#include <random>
#include <vector>

#include "options.h"
#include "videoview.h"
#include "videowidgetgrid.h"
#include "videowidgettoolbar.h"
#include "view.h"

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum class RESIZEDIRECTION
{
  NONE,
  NORTH_WEST,
  NORTH_EAST,
  SOUTH_WEST,
  SOUTH_EAST,
  NORTH,
  EAST,
  SOUTH,
  WEST
};

///// Globals /////

const unsigned int MAXVIDEOWINDOWWIDTH = 10;
const unsigned int MAXVIDEOWINDOWHEIGHT = 10;
const unsigned int MAXUPDATEFREQUENCY = 100;

const int INFO_FONT_HEIGHT = 16;
const int INFO_BORDER = 5;
const int INFO_WIDTH = 576;
const int INFO_HEIGHT = (INFO_BORDER * 2) + INFO_FONT_HEIGHT;

extern const std::array<QVector4D, 30> OBJECT_COLOURS;

///// Declarations /////

class Map;
class MapView;
class Media;
class MediaView;
class VideoView;
class View;

///// Prototypes /////

void ToInfoText(const QDateTime& datetime, const std::string& format, const monocle::Codec codec, const boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> >& bandwidthsizes, const std::pair<const std::string&, const QString&>& location, const std::pair<const std::string&, const QString&>& recordingname, const int width, const int height, std::vector<char>& buffer);

///// Classes /////

class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
 Q_OBJECT

 friend MediaView;
 friend VideoView;
 friend View;

 public:

  VideoWidget(QWidget* parent);
  ~VideoWidget();
  
  void Init(const QResource* arial, const QIcon& showfullscreen, const unsigned int width, const unsigned int height, const bool showtoolbar);
  void Destroy();

  QSharedPointer<MapView> CreateMapView(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool stretch, const boost::shared_ptr<Device>& device, const QSharedPointer<Map>& map);
  QSharedPointer<MediaView> CreateMediaView(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool stretch, const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex);
  QSharedPointer<VideoView> CreateVideoView(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool stretch, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingTrack>& track);
  bool RemoveView(const QSharedPointer<View>& view);

  VideoWidgetToolbar* GetToolbar();

  bool IsEmpty(const QSharedPointer<View>& ignoreview, unsigned int x, unsigned int y, unsigned int width, unsigned int height) const; // Pass in a video view if you're ok with overwriting one video view, otherwise pass in null
  QPoint GetLocation(unsigned int x, unsigned int y); // Get the element location at a pixel location
  QSharedPointer<View> GetView(const QPoint& pos); // Get video view at location(in pixels)
  QSharedPointer<View> GetView(const QPoint& pos) const; // Get video view at location(in pixels)
  QSharedPointer<View> GetView(unsigned int x, unsigned int y); // Get the video view at location(in elements)
  QSharedPointer<View> GetView(unsigned int x, unsigned int y) const; // Get the video view at location(in elements)

  bool AllSelected() const; // Are all the Views selected

  void SetSelectedAll(const bool selected);

  void ResetGrid();

  void SetUpdateFrequency(const unsigned int frequency);

  std::vector< QSharedPointer<View> >& GetViews() { return views_; }

  QOpenGLShaderProgram& GetViewSelectShader() { return viewselectedshader_; }

  inline unsigned int GetWidth() const { return width_; }
  inline unsigned int GetHeight() const { return height_; }

  inline const QIcon& GetShowFullscreenIcon() { return showfullscreen_; }

  QAction* GetShowToolbarAction() { return showtoolbar_; }
  QAction* GetHideToolbarAction() { return hidetoolbar_; }

 protected:

  virtual void dragEnterEvent(QDragEnterEvent* event) override;
  virtual void dragMoveEvent(QDragMoveEvent* event) override;
  virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
  virtual void dropEvent(QDropEvent* event) override;
  virtual void enterEvent(QEvent* event) override;
  virtual void leaveEvent(QEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void timerEvent(QTimerEvent* event) override;

  virtual void initializeGL() override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL() override;


 private:

  RESIZEDIRECTION GetResizeDirection(const QSharedPointer<View>& view, const QPoint& pos) const;

  int timer_;

  FT_Library freetype_;
  FT_Face freetypearial_;

  GLint openglmajorversion_;

  std::vector< QSharedPointer<View> > views_;

  QOpenGLShaderProgram viewrgbshader_;
  int rgbpositionlocation_;
  int rgbtexturecoordlocation_;
  int rgbtexturesamplerlocation_;
  int rgbcolourpickerlocation_;

  QOpenGLShaderProgram viewyuvshader_;
  int yuvpositionlocation_;
  int yuvtexturecoordlocation_;
  int yuvcolourpickerlocation_;
  std::array<int, 3> yuvtexturesamplerlocation_;

  QOpenGLShaderProgram viewnv12shader_;
  int nv12positionlocation_;
  int nv12texturecoordlocation_;
  int nv12colourpickerlocation_;
  std::array<int, 2> nv12texturesamplerlocation_;

  QOpenGLShaderProgram viewselectedshader_;
  int selectedpositionlocation_;
  int selectedcolourlocation_;

  QOpenGLShaderProgram viewinfoshader_;
  int infopositionlocation_;
  int infotexturecoordlocation_;
  int infotexturesamplerlocation_;

  QOpenGLTexture digitalsigngreenpadlock_;
  QOpenGLTexture digitalsignredpadlock_;

  QOpenGLShaderProgram digitalsignshader_;
  int digitalsignpositionlocation_;
  int digitalsigntexturecoordlocation_;
  int digitalsigntexturesamplerlocation_;

  // These are not the dimensions in pixels, but the number of grid elements
  const QResource* arial_;
  QIcon showfullscreen_;
  unsigned int width_;
  unsigned int height_;

  VideoWidgetGrid grid_;

  QAction* showtoolbar_;
  QAction* hidetoolbar_;

  RESIZEDIRECTION resizedirectionstate_;
  QWeakPointer<View> resizeview_;

  std::vector<char> infotextformatbuffer_; // This is a cache so we don't have to reallocate many times over

 public slots:

  void ShowToolbar(bool);
  void HideToolbar(bool);

 private slots:

  void AddRow(bool);
  void RemoveRow(bool);
  void AddColumn(bool);
  void RemoveColumn(bool);
  void Fullscreen(bool);
  void MapRemoved(const QSharedPointer<Map>& map);
  void MediaRemoved(QSharedPointer<Media>& device);
  void RecordingRemoved(const boost::shared_ptr<Device>& device, const uint64_t recordingtoken);
 
};

}

#endif
