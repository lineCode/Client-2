// mediaview.h
// 

#ifndef IDNUONLEDZOSDFKJDASDFDDFSFDSDS3TFS
#define IDNUONLEDZOSDFKJDASDFDDFSFDSDS3TFS

///// Includes /////

#include <boost/asio.hpp>

#include <atomic>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
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

  MediaView(VideoWidget* videowidget, CUcontext cudacontext, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool info, const QSharedPointer<Media>& media, const uint64_t deviceindex, const uint64_t recordingindex, const uint64_t videotrackindex, const QResource* arial);
  ~MediaView();

  virtual VIEWTYPE GetViewType() const override { return VIEWTYPE_MEDIA; }

  virtual void GetMenu(QMenu& parent) override;

  virtual double GetAspectRatio() const override;

  virtual bool GetImage(ImageBuffer& imagebuffer) override;

  virtual int64_t GetTimeOffset() const override { return 0; }

  virtual void FrameStep(const bool forwards) override;
  virtual void Play(const uint64_t time, const boost::optional<uint64_t>& numframes) override;
  virtual void Pause(const boost::optional<uint64_t>& time) override;
  virtual void Stop() override;
  virtual void Scrub(const uint64_t time) override;

  std::vector<int> GetCUDADevices() const;

  QSharedPointer<Media>& GetMedia() { return media_; }
  const QSharedPointer<Media>& GetMedia() const { return media_; }
  const std::string& GetVideoLocation() const { return videolocation_; }
  const std::string& GetVideoName() const { return videoname_; }
  file::TRACK GetVideoTrack() const;
  std::vector<file::TRACK> GetMetadataTracks() const;
  void SetPaused(const bool paused, const boost::optional<uint64_t>& frame);

 private:

  class MediaStream : public boost::enable_shared_from_this<MediaStream>
  {

    friend MediaView;

   public:
    
    MediaStream(const file::DEVICE& device, const file::RECORDING& recording, const file::TRACK& track, const uint64_t index);
    ~MediaStream();

    void Init(MediaView* mediaview, const bool mainstream, const uint64_t playrequestindex);
    void Destroy(); // Calling this is necessary because the thread holds a pointer to itself

    std::vector< std::shared_ptr<file::FRAMEHEADER> >::const_iterator GetFrame(const uint64_t time) const;

    inline uint64_t GetIndex() const { return index_; }
    inline size_t GetDeviceIndex() const { return device_.index_; }
    inline size_t GetRecordingIndex() const { return recording_.index_; }
    inline size_t GetTrackIndex() const { return track_.index_; }
    bool IsRunning() const { return running_; }

    template<typename U, typename... Args>
    void SendControlRequest(Args... args)
    {
      std::promise< std::unique_ptr<ControlRequest> > nextpromise;
      std::future< std::unique_ptr<ControlRequest> > future = nextpromise.get_future();
      promise_.set_value(std::make_unique<U>(std::move(future), args...));
      promise_ = std::move(nextpromise);
    }

   private:

    const file::DEVICE& device_;
    const file::RECORDING& recording_;
    const file::TRACK& track_;

    uint64_t index_;

    std::thread thread_;
    std::atomic<bool> running_;

    std::promise< std::unique_ptr<ControlRequest> > promise_;

  };

  Q_INVOKABLE void MetadataCallback(const uint64_t playrequestindex, const size_t streamindex, const size_t frame);

  void Init(const size_t deviceindex, const size_t recordingindex, const size_t trackindex, boost::optional<uint64_t> starttime);
  void FrameStepForwards(const bool mainstream, const uint64_t playrequestindex, const boost::shared_ptr<MediaStream>& stream, size_t& frame, std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> >& buffer, size_t& buffersize);
  void FrameStepBackwards(const bool mainstream, const uint64_t playrequestindex, const boost::shared_ptr<MediaStream>& stream, size_t& frame, std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> >& buffer, size_t& buffersize);
  std::pair<int, bool> SendFrame(const uint64_t playrequestindex, const bool mainstream, const boost::shared_ptr<MediaStream>& stream, const size_t frame, std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> >& buffer, size_t& buffersize); // <ret, frame> The boolean represents whether a frame was decoded or not for this buffer
  void ResetDecoders();
  void PlayThread(boost::shared_ptr<MediaStream> stream, const bool mainstream, uint64_t playrequestindex, std::future< std::unique_ptr<ControlRequest> >&& future);

  QSharedPointer<Media> media_;

  utility::PublicKey signingkey_;

  boost::shared_ptr<MediaStream> videostream_;
  std::vector< boost::shared_ptr<MediaStream> > metadatastreams_;

  mutable std::recursive_mutex decodersmutex_;
  std::unique_ptr<MJpegDecoder> mjpegdecoder_;
  std::vector< std::unique_ptr<H265Decoder> > h265decoders_;
  std::vector< std::unique_ptr<H264Decoder> > h264decoders_;
  std::vector< std::unique_ptr<MPEG4Decoder> > mpeg4decoders_;

  std::string videolocation_;
  std::string videoname_;

  uint64_t nextstreamindex_;

  std::unique_ptr< uint8_t[], utility::DeleteAligned<uint8_t> > buffer_;
  size_t buffersize_;

};

}

#endif
