// view.h
// locally, (0, 0), represents the top left

#ifndef IDNUONL19238728SDSADSADSAD4S6B3TFS
#define IDNUONL19238728SDSADSADSAD4S6B3TFS

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <atomic>
#include <boost/circular_buffer.hpp>
#include <chrono>
#include <cuda.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <memory>
#include <mutex>
#include <QEnableSharedFromThis>
#include <QMenu>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QRectF>
#include <QResource>
#include <QStaticText>
#include <QVector4D>
#include <vector>

#include "connection.h"
#include "decoder.h"
#include "imagecache.h"
#include "objects.h"
#include "options.h"

///// Declarations /////

namespace monocle { enum class ObjectClass : uint16_t; struct Objects; }

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class H264Decoder;
class MJpegDecoder;
class Recording;
class RecordingTrack;
class VideoWidget;

///// Enumerations /////

enum class ROTATION : int
{
  _0,
  _90,
  _180,
  _270
};

enum VIEWTYPE
{
  VIEWTYPE_MAP,
  VIEWTYPE_MEDIA,
  VIEWTYPE_MONOCLE
};

///// Prototypes /////

QString ToString(const ROTATION rotation);
std::array<float, 12> GetVertices(const QRectF& rect, const ROTATION rotation, const bool mirror);
QPointF ImageRectToOpenGL(const QRectF& rect, const bool mirror, const ROTATION rotation, const float x, const float y);
QRectF ImageToRect(const QRect& imagepixelrect, const QRect& rect, const bool mirror, const ROTATION rotation);
void WriteImageBuffer(QOpenGLFunctions* ogl, const IMAGEBUFFERTYPE currenttype, const int currentimagewidth, const int currentimageheight, const ImageBuffer& imagebuffer, const std::array<GLuint, 3>& textures, std::array<CUgraphicsResource, 3>& cudaresources);
QString HTMLColour(const int r, const int g, const int b);
QString FontText(const QVector4D& colour, const QString& text);

///// Globals /////

const uint64_t OBJECT_KILL_DELAY = 1500;
const std::chrono::steady_clock::duration OBJECT_KILL_AGE(std::chrono::milliseconds(1500));

///// Classes /////

class View : public QObject, public QEnableSharedFromThis<View>
{
 Q_OBJECT

 public:

  View(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool showinfo, const bool showobjects, const QResource* arial, const bool showsaveimagemenu, const bool showcopymenu, const bool showinfomenu, const bool showobjectsmenu);
  virtual ~View();

  virtual VIEWTYPE GetViewType() const = 0;

  std::chrono::steady_clock::time_point GetStartTime() const { return starttime_; }
  const QVector4D& GetSelectedColour() const { return selectedcolour_; }

  virtual void GetMenu(QMenu& parent);

  virtual double GetAspectRatio() const = 0; // 0.0 represents to never stretch, even if stretch is true. This will probably because we don't know the resolution yet

  virtual bool GetImage(ImageBuffer& imagebuffer) = 0; // Get the most recently produced image(this should be called from the consumer thread in VideoWidget)

  virtual int64_t GetTimeOffset() const = 0;

  virtual void FrameStep(const bool forwards) = 0;
  virtual void Play(const uint64_t time, const boost::optional<uint64_t>& numframes) = 0;
  virtual void Pause(const boost::optional<uint64_t>& time) = 0;
  virtual void Stop() = 0;
  virtual void Scrub(const uint64_t time) = 0;

  VideoWidget* GetVideoWidget() { return videowidget_; }
  void SetVideoWidget(VideoWidget* videowidget) { videowidget_ = videowidget; }

  CUcontext GetCUDAContext() const { return cudacontext_; }

  Q_INVOKABLE void ResetPosition(const bool makecurrent); // Calls set position with current parameters_
  void SetPosition(VideoWidget* videowidget, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool makecurrent);

  void AddCacheImage(ImageBuffer& imagebuffer);

  QRect GetPixelRect() const;
  QRectF GetImageRect() const;
  QRect GetImagePixelRect() const;
  QRectF GetImagePixelRectF() const;

  QImage GetQImage(const boost::optional<QRect>& rect) const;

  QColor GetPixelColour(const int x, const int y) const;

  inline boost::lockfree::spsc_queue<ImageBuffer, boost::lockfree::capacity<IMAGEQUEUESIZE> >& GetImageQueue() { return imagequeue_; }
  inline SPSCFreeFrameBuffers& GetFreeFrameBuffers() { return freeimagequeue_; }

  const boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> >& GetBandwidthSizes() const { return bandwidthsizes_; }

  inline void IncrementTotalFrames() { ++totalframes_; }
  inline uint64_t GetTotalFrames() const { return totalframes_; }
  inline void AddTotalBytes(const uint64_t bytes) { totalbytes_ += bytes; }
  inline uint64_t GetTotalBytes() const { return totalbytes_; }
  inline void SetCodec(const monocle::Codec codec) { codec_ = codec; }
  inline monocle::Codec GetCodec() const { return codec_; }
  inline void SetTime(const uint64_t time) { time_ = time; }
  inline uint64_t GetTime() const { return time_; }
  inline void SetPlayMarkerTime(const uint64_t playmarkertime) { playmarkertime_ = playmarkertime; }
  inline uint64_t GetPlayMarkerTime() const { return playmarkertime_; }
  inline void SetFrameTime(const std::chrono::steady_clock::time_point frametime) { frametime_ = frametime; }
  inline std::chrono::steady_clock::time_point GetFrameTime() const { return frametime_; }
  inline void SetType(IMAGEBUFFERTYPE type) { type_ = type; }
  IMAGEBUFFERTYPE GetImageType() const { return type_; }
  inline void SetSequenceNum(const int64_t sequencenum) { sequencenum_ = sequencenum; }
  inline int64_t GetSequenceNum() const { return sequencenum_; }
  inline void SetDigitallySigned(const boost::optional<bool>& digitallysigned) { digitallysigned_ = digitallysigned; }
  inline const boost::optional<bool>& GetDigitallySigned() const { return digitallysigned_; }
  inline QOpenGLBuffer& GetTextureBuffer() { return texturebuffer_; }
  inline QOpenGLBuffer& GetTextVertexBuffer() { return textvertexbuffer_; }
  inline QOpenGLBuffer& GetVertexBuffer() { return vertexbuffer_; }
  inline QOpenGLBuffer& GetSelectedVertexBuffer() { return selectvertexbuffer_; }
  inline std::map< std::pair<monocle::ObjectClass, uint64_t>, std::vector<Object> >& GetObjects() { return objects_.GetObjects(); }
  std::array<GLuint, 3>& GetTextures() { return textures_; }
  void SetCUDAResource(const size_t index, const CUgraphicsResource cudaresource) { cudaresources_[index] = cudaresource; }
  CUgraphicsResource GetCUDAResource(const size_t index) const { return cudaresources_[index]; }
  const std::array<CUgraphicsResource, 3>& GetCUDAResources() const { return cudaresources_; }
  std::array<CUgraphicsResource, 3>& GetCUDAResources() { return cudaresources_; }
  inline GLuint GetInfoTexture() const { return infotexture_; }
  inline QOpenGLBuffer& GetInfoTextureBuffer() { return infotexturebuffer_; }
  inline QOpenGLBuffer& GetInfoVertexBuffer() { return infovertexbuffer_; }
  inline void SetInfoTime(const uint64_t infotime) { infotime_ = infotime; }
  inline uint64_t GetInfoTime() { return infotime_; }
  inline QOpenGLBuffer& GetDigitalSignTextureBuffer() { return digitalsigntexturebuffer_; }
  inline QOpenGLBuffer& GetDigitalSignVertexBuffer() { return digitalsignvertexbuffer_; }
  inline const QRect& GetRect() const { return rect_; }
  inline ROTATION GetRotation() const { return rotation_; }
  inline bool GetMirror() const { return mirror_; }
  inline bool GetStretch() const { return stretch_; }
  inline bool GetShowInfo() const { return showinfo_; }
  inline bool GetShowObjects() const { return showobjects_; }
  inline void SetSelected(bool selected) { selected_ = selected; }
  inline bool GetSelected() const { return selected_; }
  void SetPaused(const bool paused);
  bool IsPaused() const { return paused_; }
  inline QAction* GetActionClose() { return actionclose_; }
  inline void SetImageWidth(const int imagewidth) { imagewidth_ = imagewidth; }
  inline int GetImageWidth() const { return imagewidth_; }
  inline void SetImageHeight(const int imageheight) { imageheight_ = imageheight; }
  inline int GetImageHeight() const { return imageheight_; }
  inline uint64_t GetPlayRequestIndex() const { return playrequestindex_; }
  uint64_t GetNextPlayRequestIndex(const bool clearcache);

 protected:

  static const std::array<float, 8> texturecoords_;

  virtual void timerEvent(QTimerEvent* event) override;
  QRectF GetOpenglRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height) const;
  void SetMessage(const uint64_t playrequestindex, bool error, const QString& text);
  void WriteFrame(const ImageBuffer& imagebuffer);
  void UpdateObjects(const monocle::Objects* objects, const uint64_t time);

  const std::chrono::steady_clock::time_point starttime_;
  const QVector4D selectedcolour_;

  VideoWidget* videowidget_;
  CUcontext cudacontext_;

  QAction* actionsaveimage_;
  QAction* actioncopy_;
  QAction* actionrotate0_;
  QAction* actionrotate90_;
  QAction* actionrotate180_;
  QAction* actionrotate270_;
  QAction* actionmirror_;
  QAction* actionstretch_;
  QAction* actioninfo_;
  QAction* actionobjects_;

  FT_Library freetype_;
  FT_Face arial_;

  boost::lockfree::spsc_queue<ImageBuffer, boost::lockfree::capacity<IMAGEQUEUESIZE> > imagequeue_; // Queue of decoded images for the main thread to get
  SPSCFreeFrameBuffers freeimagequeue_;

  uint64_t totalframes_;
  uint64_t totalbytes_;
  monocle::Codec codec_;
  uint64_t time_;
  uint64_t playmarkertime_;
  std::chrono::steady_clock::time_point frametime_;
  IMAGEBUFFERTYPE type_; // What the previous texture type was
  int64_t sequencenum_;
  boost::optional<bool> digitallysigned_; // boost::none represents signing disabled. true represents verified. false represents failed verification
  QOpenGLBuffer texturebuffer_;
  QOpenGLBuffer textvertexbuffer_;
  QOpenGLBuffer vertexbuffer_;
  QOpenGLBuffer selectvertexbuffer_; // Represents the selection box
  Objects objects_;
  std::array<GLuint, 3> textures_;
  std::array<CUgraphicsResource, 3> cudaresources_; // Lazily initialised
  GLuint infotexture_;
  QOpenGLBuffer infotexturebuffer_;
  QOpenGLBuffer infovertexbuffer_;
  uint64_t infotime_; // This is used so we know if we need to generate a new info buffer or not
  QOpenGLBuffer digitalsigntexturebuffer_;
  QOpenGLBuffer digitalsignvertexbuffer_;
  QRect rect_; // The location of this video view(this is not in pixels, but in elements in the grid)
  ROTATION rotation_;
  bool mirror_;
  bool stretch_;
  bool showinfo_;
  bool showobjects_;

  bool selected_;

  std::atomic<bool> paused_;

  QAction* actionclose_;

  mutable std::mutex mutex_;

  ImageCache cache_; // Stores up frames so we can framestep and do things without requesting from the server

  int imagewidth_;
  int imageheight_;

  boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> > bandwidthsizes_; // <time, size>

 private:

  uint64_t playrequestindex_;

 public slots:

  void SaveImage(bool);
  void Copy(bool);
  void Rotate0(bool);
  void Rotate90(bool);
  void Rotate180(bool);
  void Rotate270(bool);
  void ToggleMirror(bool);
  void ToggleStretch(bool);
  void ToggleShowInfo(bool);
  void ToggleShowObjects(bool);

};

}

#endif
