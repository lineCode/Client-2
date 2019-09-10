// videoview.h
// 

#ifndef IDNUONLEDZODEV2BSHG9Z3CFN24S6B3TFS
#define IDNUONLEDZODEV2BSHG9Z3CFN24S6B3TFS

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <memory>
#include <mutex>
#include <onvifclient/connection.hpp>
#include <QEnableSharedFromThis>
#include <QMenu>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QRectF>
#include <QResource>
#include <QVector4D>
#include <vector>

#include "connection.h"
#include "decoder.h"
#include "options.h"
#include "view.h"

///// Declarations /////

namespace onvif
{
namespace device
{
class DeviceClient;
}
namespace event
{
class EventClient;
}
namespace media
{
class MediaClient;
}
namespace ptz
{
class PTZClient;
}
}

///// Namespaces /////

namespace client
{

///// Declarations /////

class Device;
class H265Decoder;
class H264Decoder;
class MJpegDecoder;
class MPEG4Decoder;
class Receiver;
class Recording;
class RecordingJob;
class RecordingTrack;
class VideoWidget;
enum class DEVICESTATE : int;

///// Classes /////

class VideoView : public View
{
 Q_OBJECT

 public:

  VideoView(VideoWidget* videowidget, const QColor& selectedcolour, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const ROTATION rotation, const bool mirror, const bool stretch, const bool info, boost::shared_ptr<client::Device> device, QSharedPointer<client::Recording> recording, QSharedPointer<client::RecordingTrack> track, const QResource* arial);
  virtual ~VideoView();

  virtual VIEWTYPE GetViewType() const override { return VIEWTYPE_MONOCLE; }

  void Connect();
  void Disconnect();

  virtual void GetMenu(QMenu& parent) override;

  virtual double GetAspectRatio() const override;

  virtual bool GetImage(ImageBuffer& imagebuffer) override;

  virtual int64_t GetTimeOffset() const override;

  virtual void Play() override;
  virtual void FrameStep(const bool forwards) override;
  virtual void Play(const uint64_t time, const boost::optional<uint64_t>& numframes) override;
  virtual void Pause(const boost::optional<uint64_t>& time) override;
  virtual void Stop() override;
  virtual void Scrub(const uint64_t time) override;

  std::vector<int> GetCUDADevices() const;

  boost::shared_ptr<Device>& GetDevice() { return device_; }
  const boost::shared_ptr<Device>& GetDevice() const { return device_; }
  QSharedPointer<client::Recording>& GetRecording() { return recording_; }
  const QSharedPointer<client::Recording>& GetRecording() const { return recording_; }
  QSharedPointer<client::RecordingTrack>& GetTrack() { return track_; }
  const QSharedPointer<client::RecordingTrack>& GetTrack() const { return track_; }
  boost::shared_ptr<onvif::ptz::PTZClient>& GetONVIFPTZClient() { return onvifptz_; }
  const boost::shared_ptr<onvif::ptz::PTZClient>& GetONVIFPTZClient() const { return onvifptz_; }

  std::vector<QString> GetProfileTokens() const;
  inline const boost::optional<float>& GetPTZXMin() const { return ptzxmin_; }
  inline const boost::optional<float>& GetPTZXMax() const { return ptzxmax_; }
  inline const boost::optional<float>& GetPTZYMin() const { return ptzymin_; }
  inline const boost::optional<float>& GetPTZYMax() const { return ptzymax_; }
  inline const boost::optional<float>& GetZoomMin() const { return zoommin_; }
  inline const boost::optional<float>& GetZoomMax() const { return zoommax_; }

 protected:

  virtual void timerEvent(QTimerEvent* event) override;

 private:

  static void ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata);
  static void H265Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
  static void H264Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
  static void MetadataCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
  static void JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata);
  static void MPEG4Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
  static void NewCodecIndexCallback(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata);
  
  void ConnectONVIF(const QSharedPointer<client::Receiver>& receiver);
  void AddCodecIndex(const monocle::CODECINDEX& codecindex);
  void DestroyDecoders();
  void Keepalive();
  ROTATION GetRotation() const;
  void ResetDecoders();

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;
  QSharedPointer<client::RecordingTrack> track_;

  QAction* actionreconnect_;
  QAction* actionproperties_;

  std::chrono::steady_clock::time_point frametime_;

  mutable std::mutex mutex_;

  boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;
  boost::shared_ptr<onvif::event::EventClient> onvifevent_;
  boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;
  boost::shared_ptr<onvif::ptz::PTZClient> onvifptz_;
  onvif::Connection onvifconnection_;

  boost::optional<float> ptzxmin_;
  boost::optional<float> ptzxmax_;
  boost::optional<float> ptzymin_;
  boost::optional<float> ptzymax_;
  boost::optional<float> zoommin_;
  boost::optional<float> zoommax_;

  boost::shared_ptr<Connection> connection_;

  sock::Connection connect_;
  monocle::client::Connection getauthenticatenonce_;
  monocle::client::Connection authenticate_;
  monocle::client::Connection createstream_;
  monocle::client::Connection controlstream_;
  monocle::client::Connection keepalive_;

  uint64_t streamtoken_;

  std::unique_ptr<MJpegDecoder> mjpegdecoder_; // We only ever need one MJPEG decoder because there are never any worthwhile parameters and frames are discrete
  std::vector< std::unique_ptr<H265Decoder> > h265decoders_;
  std::vector< std::unique_ptr<H264Decoder> > h264decoders_;
  std::vector< std::unique_ptr<MPEG4Decoder> > mpeg4decoders_;

  std::function<void(const uint64_t, const monocle::ErrorCode)> controlstreamendcallback_;

 public slots:

  void DeviceStateChanged(const DEVICESTATE state, const QString&);
  void Reconnect(bool);
  void Properties(bool);
  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t token);
  void ActiveJobChanged(const QSharedPointer<client::RecordingJob>& activejob);

};

}

#endif
