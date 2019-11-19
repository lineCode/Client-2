// findobjectvideowidget.h
//

#ifndef IDLIDSFMAZXCZXCZXCZFDSFDSFSFSFSFFF
#define IDLIDSFMAZXCZXCZXCZFDSFDSFSFSFSFFF

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/lockfree/spsc_queue.hpp>
#include <QAction>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QRectF>

#include "monocleclient/decoder.h"
#include "monocleclient/imagecache.h"
#include "monocleclient/objects.h"
#include "monocleclient/view.h"

///// Declarations /////

class QImage;
class QWidget;

///// Namespaces /////

namespace client
{

///// Enumerations /////

enum FINDOBJECTSTATE
{
  FINDOBJECTSTATE_SELECT,
  FINDOBJECTSTATE_DRAWING
};

///// Declarations /////

class FindObjectWindow;
class FindObjectPlaybackWidget;

///// Classes /////

class FindObjectVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
 friend class FindObjectWindow;
 friend class FindObjectPlaybackWidget;

 Q_OBJECT

 public:

  FindObjectVideoWidget(QWidget* parent);
  ~FindObjectVideoWidget();

  FindObjectWindow* GetFindObjectWindow();
  FindObjectWindow* GetFindObjectWindow() const;
  FindObjectPlaybackWidget* GetFindObjectPlaybackWidget();
  FindObjectPlaybackWidget* GetFindObjectPlaybackWidget() const;

  inline boost::lockfree::spsc_queue<ImageBuffer, boost::lockfree::capacity<IMAGEQUEUESIZE> >& GetImageQueue() { return imagequeue_; }
  inline VectorFreeFrameBuffer& GetFreeImageQueue() { return freeimagequeue_; }

  uint64_t GetNextVideoPlayRequestIndex();
  inline uint64_t GetVideoPlayRequestIndex() const { return videoplayrequestindex_; }

  uint64_t GetNextMetadataPlayRequestIndex();
  inline uint64_t GetMetadataPlayRequestIndex() const { return metadataplayrequestindex_; }

  inline const QRectF& GetSelectedRect() const { return selectedrect_; }

  void SetImage(const QImage& image);
  void SetSelectedRect(const QRectF& selectedrect);

  void SetPaused(const bool paused);
  bool IsPaused() const;

 protected:

  virtual void contextMenuEvent(QContextMenuEvent* event) override;
  virtual void initializeGL() override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL() override;
  void timerEvent(QTimerEvent* event) override;

 private:

  bool GetImage(ImageBuffer& imagebuffer);
  void WriteFrame(const ImageBuffer& imagebuffer);
  QRectF GetImageRect() const;
  QRect GetImagePixelRect() const;
  QRectF GetImagePixelRectF() const;
  void SetPosition(const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent);

  static const std::array<float, 8> texturecoords_;

  QAction* actionrotate0_;
  QAction* actionrotate90_;
  QAction* actionrotate180_;
  QAction* actionrotate270_;
  QAction* actionmirror_;
  QAction* actionstretch_;
  QAction* actionobjects_;

  FT_Library freetype_;
  FT_Face freetypearial_;

  QOpenGLShaderProgram viewrgbshader_;
  int rgbpositionlocation_;
  int rgbtexturecoordlocation_;
  int rgbtexturesamplerlocation_;

  QOpenGLShaderProgram viewyuvshader_;
  int yuvpositionlocation_;
  int yuvtexturecoordlocation_;
  std::array<int, 3> yuvtexturesamplerlocation_;

  QOpenGLShaderProgram viewnv12shader_;
  int nv12positionlocation_;
  int nv12texturecoordlocation_;
  std::array<int, 2> nv12texturesamplerlocation_;

  QOpenGLShaderProgram viewselectedshader_;
  int selectedpositionlocation_;
  int selectedcolourlocation_;

  QOpenGLShaderProgram viewinfoshader_;
  int infopositionlocation_;
  int infotexturecoordlocation_;
  int infotexturesamplerlocation_;

  boost::lockfree::spsc_queue<ImageBuffer, boost::lockfree::capacity<IMAGEQUEUESIZE> > imagequeue_;
  VectorFreeFrameBuffer freeimagequeue_;

  QRectF selectedrect_; // This is the rect before any transformations of stretched, mirrored or rotated

  uint64_t videoplayrequestindex_;
  uint64_t metadataplayrequestindex_;
  bool paused_;

  IMAGEBUFFERTYPE type_; // What the previous texture type was
  monocle::Codec codec_;
  uint64_t time_;
  uint64_t playmarkertime_;
  int64_t sequencenum_;
  std::chrono::steady_clock::time_point frametime_;
  QOpenGLBuffer texturebuffer_;
  QOpenGLBuffer vertexbuffer_;
  std::array<GLuint, 3> textures_;
  std::array<CUgraphicsResource, 3> cudaresources_; // Lazily initialised
  GLuint infotexture_;
  QOpenGLBuffer infotexturebuffer_;
  QOpenGLBuffer infovertexbuffer_;
  uint64_t infotime_; // This is used so we know if we need to generate a new info buffer or not

  ImageCache cache_;

  boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> > bandwidthsizes_; // <time, size>

  Objects objects_;

  FINDOBJECTSTATE state_;
  QPoint selectionpoint_;

 public slots:

  void Rotate0(bool);
  void Rotate90(bool);
  void Rotate180(bool);
  void Rotate270(bool);
  void ToggleMirror(bool);
  void ToggleStretch(bool);
  void ToggleShowObjects(bool);

};

}

#endif
