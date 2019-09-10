// mediaview.h
// 

#ifndef IDNUONLEDZOSDFKJDASDFDDFSFDSDS3TFS
#define IDNUONLEDZOSDFKJDASDFDDFSFDSDS3TFS

///// Includes /////

#include <boost/asio.hpp>

#include <atomic>
#include <chrono>
#include <file/file.hpp>
#include <ft2build.h>
#include <future>
#include FT_FREETYPE_H
#include <memory>
#include <mutex>
#include <QEnableSharedFromThis>
#include <QMenu>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QRectF>
#include <QResource>
#include <QVector4D>
#include <thread>
#include <utility/utility.hpp>
#include <vector>

#include "connection.h"
#include "decoder.h"
#include "options.h"
#include "view.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

class ControlRequest;
class H265Decoder;
class H264Decoder;
class Media;
class MJpegDecoder;
class MPEG4Decoder;
class VideoWidget;

///// Classes /////

class MediaView : public View
{
 Q_OBJECT

 using View::SetPaused;

 public:

  MediaView(VideoWidget* videowidget, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool info, const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t trackindex, const QResource* arial);
  ~MediaView();

  virtual VIEWTYPE GetViewType() const override { return VIEWTYPE_MEDIA; }

  virtual void GetMenu(QMenu& parent) override;

  virtual double GetAspectRatio() const override;

  virtual bool GetImage(ImageBuffer& imagebuffer) override;

  virtual int64_t GetTimeOffset() const override { return 0; }

  virtual void Play() override;
  virtual void FrameStep(const bool forwards) override;
  virtual void Play(const uint64_t time, const boost::optional<uint64_t>& numframes) override;
  virtual void Pause(const boost::optional<uint64_t>& time) override;
  virtual void Stop() override;
  virtual void Scrub(const uint64_t time) override;

  std::vector<int> GetCUDADevices() const;

  QSharedPointer<Media>& GetMedia() { return media_; }
  const QSharedPointer<Media>& GetMedia() const { return media_; }
  inline uint64_t GetDeviceIndex() const { return deviceindex_; }
  inline uint64_t GetRecordingIndex() const { return recordingindex_; }
  inline uint64_t GetTrackIndex() const { return trackindex_; }
  inline const file::DEVICE& GetDevice() const { return device_; }
  inline const file::RECORDING& GetRecording() const { return recording_; }
  inline const file::TRACK& GetCurrentTrack() const { return currenttrack_; }
  inline void SetPaused(const bool paused, const boost::optional<uint64_t>& frame);

 private:

  void Init(const boost::optional<uint64_t>& starttime);
  void FrameStepForwards(const uint64_t playrequestindex, size_t& frame, std::unique_ptr< char[], utility::DeleteAligned<char> >& buffer, size_t& buffersize);
  void FrameStepBackwards(const uint64_t playrequestindex, size_t& frame, std::unique_ptr< char[], utility::DeleteAligned<char> >& buffer, size_t& buffersize);
  std::pair<int, bool> SendFrame(const uint64_t playrequestindex, const size_t frame, std::unique_ptr< char[], utility::DeleteAligned<char> >& buffer, size_t& buffersize); // <ret, frame> The boolean represents whether a frame was decoded or not for this buffer
  void ResetDecoders();

  template<typename U, typename... Args>
  void SendControlRequest(Args... args)
  {
    std::promise< std::unique_ptr<ControlRequest> > nextpromise;
    std::future< std::unique_ptr<ControlRequest> > future = nextpromise.get_future();
    promise_.set_value(std::make_unique<U>(std::move(future), args...));
    promise_ = std::move(nextpromise);
  }

  QSharedPointer<Media> media_;
  uint64_t deviceindex_;
  uint64_t recordingindex_;
  uint64_t trackindex_;

  file::DEVICE device_;
  file::RECORDING recording_;
  file::TRACK currenttrack_;

  utility::PublicKey signingkey_;

  std::thread thread_;
  std::atomic<bool> running_;

  std::promise< std::unique_ptr<ControlRequest> > promise_;

  std::unique_ptr<MJpegDecoder> mjpegdecoder_;
  std::vector< std::unique_ptr<H265Decoder> > h265decoders_;
  std::vector< std::unique_ptr<H264Decoder> > h264decoders_;
  std::vector< std::unique_ptr<MPEG4Decoder> > mpeg4decoders_;

};

}

#endif
