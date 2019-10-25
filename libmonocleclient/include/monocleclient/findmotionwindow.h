// findmotionwindow.h
//

#ifndef IDLIY0KUADFASKDASDASDASDADSAD1EPVI
#define IDLIY0KUADFASKDASDASDASDADSAD1EPVI

///// Includes /////

#include <boost/asio.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <cuda.h>
#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <monocleprotocol/monocleprotocol.hpp>
#include <monocleprotocol/codecindex_generated.h>
#include <monocleprotocol/errorcode_generated.h>
#include <socket/connection.hpp>
#include <QDialog>
#include <QRectF>
#include <QSharedPointer>
#include <vector>

#include "ui_findmotionwindow.h"

#include "monocleclient/decoder.h"

///// Declarations /////

class QImage;
class QVector4D;

///// Namespaces /////

namespace client
{

///// Declarations /////

class Connection;
class Device;
class H265Decoder;
class H264Decoder;
class MJpegDecoder;
class MPEG4Decoder;
class Recording;
class RecordingTrack;

///// Classes /////

class FindMotionWindow : public QDialog
{
 friend class FindMotionPlaybackWidget;
 friend class FindMotionVideoWidget;

 Q_OBJECT

 public:

  FindMotionWindow(QWidget* parent, const QImage& image, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingTrack>& track, const QVector4D& colour, const uint64_t starttime, const uint64_t endtime, const QRectF& rect, const bool mirror, const ROTATION rotation);
  virtual ~FindMotionWindow();

  inline const boost::shared_ptr<Connection>& GetConnection() const { return connection_; }
  inline const boost::shared_ptr<Device>& GetDevice() const { return device_; }
  FindMotionPlaybackWidget* GetPlaybackWidget() const;
  FindMotionVideoWidget* GetVideoWidget() const;
  QSharedPointer<Recording> GetRecording() const { return recording_; }
  QSharedPointer<RecordingTrack> GetTrack() const { return track_; }

  void SetStartTime(const uint64_t time) { ui_.datetimestart->setDateTime(QDateTime::fromMSecsSinceEpoch(time, Qt::UTC)); }
  QDateTime GetStartTime() const { return ui_.datetimestart->dateTime(); }
  void SetEndTime(const uint64_t time) { ui_.datetimeend->setDateTime(QDateTime::fromMSecsSinceEpoch(time, Qt::UTC)); }
  QDateTime GetEndTime() const { return ui_.datetimeend->dateTime(); }

  void SetPaused(const bool paused);

  void Pause(const boost::optional<uint64_t>& time);
  void Play(const uint64_t time, const boost::optional<uint64_t>& numframes);
  void Stop();
  void FrameStep(const bool forwards);

 protected:


 private:

  static void ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata);
  static void H265Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
  static void H264Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
  static void MetadataCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
  static void JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata);
  static void MPEG4Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
  static void NewCodecIndexCallback(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata);

  inline boost::lockfree::spsc_queue<ImageBuffer, boost::lockfree::capacity<IMAGEQUEUESIZE> >& GetImageQueue() { return ui_.videowidget->GetImageQueue(); }
  inline VectorFreeFrameBuffer& GetFreeImageQueue() { return ui_.videowidget->GetFreeImageQueue(); }
  void AddCodecIndex(const monocle::CODECINDEX& codecindex);
  void DestroyDecoders();
  void ResetDecoders();

  Ui::FindMotionWindow ui_;

  CUcontext cudacontext_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;
  QSharedPointer<RecordingTrack> track_;

  bool mirror_;
  ROTATION rotation_;

  boost::shared_ptr<Connection> connection_;

  sock::Connection connect_;
  monocle::client::Connection getauthenticatenonce_;
  monocle::client::Connection authenticate_;
  monocle::client::Connection createstream_;
  monocle::client::Connection createfindmotion_;

  boost::optional<uint64_t> streamtoken_;
  boost::optional<uint64_t> findmotiontoken_;

  std::vector<monocle::client::Connection> getsnapshotconnections_;

  std::unique_ptr<MJpegDecoder> mjpegdecoder_; // We only ever need one MJPEG decoder because there are never any worthwhile parameters and frames are discrete
  std::vector< std::unique_ptr<H265Decoder> > h265decoders_;
  std::vector< std::unique_ptr<H264Decoder> > h264decoders_;
  std::vector< std::unique_ptr<MPEG4Decoder> > mpeg4decoders_;

  std::function<void(const uint64_t, const monocle::ErrorCode)> controlstreamendcallback_;

 private slots:

  void TrackAdded(const QSharedPointer<RecordingTrack>& track);
  void TrackRemoved(const uint32_t trackid);
  void FindMotionEnd(const uint64_t token, const uint64_t ret);
  void FindMotionProgress(const uint64_t token, const float progress);
  void FindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end);
  void StartDateTimeChanged(const QDateTime& datetime);
  void EndDateTimeChanged(const QDateTime& datetime);
  void on_buttonsearch_clicked();
  void on_tableresults_clicked(QModelIndex);
  void on_slidersensitivity_valueChanged(int);
  void on_buttonframestepbackwards_clicked();
  void on_buttonplay_clicked();
  void on_buttonpause_clicked();
  void on_buttonstop_clicked();
  void on_buttonframestepforwards_clicked();
  void on_buttonzoomin_clicked();
  void on_buttonzoomout_clicked();
  void on_buttonexport_clicked();

};

}

#endif
