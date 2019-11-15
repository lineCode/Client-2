// findobjectwindow.h
//

#ifndef IDASDASKNDSKDNZSKDZXLDKZXASDASDDSA
#define IDASDASKNDSKDNZSKDZXLDKZXASDASDDSA

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

#include "ui_findobjectwindow.h"

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

///// Structures /////

struct FINDOBJECTRESULT
{
  FINDOBJECTRESULT(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight);

  uint64_t token_;
  uint64_t start_;
  uint64_t end_;
  monocle::ObjectClass objectclass_;
  uint64_t id_;
  uint64_t largesttime_;
  float largestx_;
  float largesty_;
  float largestwidth_;
  float largestheight_;

};

///// Classes /////

class FindObjectWindow : public QDialog
{
 friend class FindObjectPlaybackWidget;
 friend class FindObjectVideoWidget;

 Q_OBJECT

 public:

  FindObjectWindow(QWidget* parent, const QImage& image, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingTrack>& track, const QVector4D& colour, const uint64_t starttime, const uint64_t endtime, const QRectF& rect, const int imagewidth, const int imageheight, const bool mirror, const ROTATION rotation, const bool stretch);
  virtual ~FindObjectWindow();

  inline const boost::shared_ptr<Connection>& GetConnection() const { return connection_; }
  inline const boost::shared_ptr<Device>& GetDevice() const { return device_; }
  FindObjectPlaybackWidget* GetPlaybackWidget() const;
  FindObjectVideoWidget* GetVideoWidget() const;
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

  virtual void timerEvent(QTimerEvent*) override;

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
  bool Filter(const monocle::ObjectClass objectclass, const uint64_t start, const uint64_t end);
  void UpdateFilter();

  Ui::FindObjectWindow ui_;

  CUcontext cudacontext_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;
  QSharedPointer<RecordingTrack> track_;

  int imagewidth_;
  int imageheight_;

  bool mirror_;
  ROTATION rotation_;
  bool stretch_;

  boost::shared_ptr<Connection> connection_;

  sock::Connection connect_;
  monocle::client::Connection getauthenticatenonce_;
  monocle::client::Connection authenticate_;
  monocle::client::Connection videocreatestream_;
  monocle::client::Connection metadatacreatestream_;
  monocle::client::Connection createfindobject_;

  boost::optional<uint64_t> videostreamtoken_;
  boost::optional<uint64_t> metadatastreamtoken_;
  boost::optional<uint64_t> findobjecttoken_;

  monocle::client::Connection getsnapshotconnection_;

  std::unique_ptr<MJpegDecoder> mjpegdecoder_; // We only ever need one MJPEG decoder because there are never any worthwhile parameters and frames are discrete
  std::vector< std::unique_ptr<H265Decoder> > h265decoders_;
  std::vector< std::unique_ptr<H264Decoder> > h264decoders_;
  std::vector< std::unique_ptr<MPEG4Decoder> > mpeg4decoders_;

  std::function<void(const uint64_t, const monocle::ErrorCode)> controlstreamendcallback_;

  std::vector<FINDOBJECTRESULT> retrievethumbnails_;

 private slots:

  void TrackAdded(const QSharedPointer<RecordingTrack>& track);
  void TrackRemoved(const uint32_t trackid);
  void FindObjectEnd(const uint64_t token, const uint64_t ret);
  void FindObjectProgress(const uint64_t token, const float progress);
  void FindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight);
  void StartDateTimeChanged(const QDateTime& datetime);
  void EndDateTimeChanged(const QDateTime& datetime);
  void on_buttonsearch_clicked();
  void on_checkfilterhumans_clicked();
  void on_checkfilterbicycles_clicked();
  void on_checkfiltercars_clicked();
  void on_checkfiltermotorbikes_clicked();
  void on_checkfilteraeroplanes_clicked();
  void on_checkfilterbuses_clicked();
  void on_checkfiltertrains_clicked();
  void on_checkfiltertrucks_clicked();
  void on_checkfilterboats_clicked();
  void on_checkfiltercats_clicked();
  void on_checkfilterdogs_clicked();
  void on_checkfilterhorses_clicked();
  void on_checkfilterbackpacks_clicked();
  void on_checkfilterumbrellas_clicked();
  void on_checkfilterhandbags_clicked();
  void on_checkfiltersuitcases_clicked();
  void on_spinminimumduration_valueChanged(int);
  void on_tableresults_clicked(QModelIndex);
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
