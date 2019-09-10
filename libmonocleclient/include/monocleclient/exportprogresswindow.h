// exportprogresswindow.h
//

#ifndef ID2VVC4FDKSDCSDFSJ23B4QAES19WKDN9G
#define ID2VVC4FDKSDCSDFSJ23B4QAES19WKDN9G

///// Includes /////

#include <boost/asio.hpp>

#include <atomic>
#include <boost/circular_buffer.hpp>
#include <file/filewrite.hpp>
#include <memory>
#include <QDialog>
#include <QDir>
#include <QSharedPointer>
#include <QString>
#include <vector>

#include "connection.h"
#include "savevideo.h"
#include "ui_exportprogresswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class H265Decoder;
class H264Decoder;
class MJpegDecoder;
class MPEG4Decoder;
class Recording;
class SaveVideo;

///// Enumerations /////

enum EXPORTFORMAT
{
  EXPORTFORMAT_MONOCLE,
  EXPORTFORMAT_AVI
};

enum EXPORTTRACKSTATE
{
  EXPORTTRACKSTATE_CONNECTING,
  EXPORTTRACKSTATE_CONNECTED,
  EXPORTTRACKSTATE_OPENING,
  EXPORTTRACKSTATE_STREAMING,
  EXPORTTRACKSTATE_SUCCESS,
  EXPORTTRACKSTATE_ERROR
};

///// Prototypes /////

QString ToString(const EXPORTFORMAT exportformat);
boost::optional<EXPORTFORMAT> GetExportFormat(const QString& exportformat);

///// Classes /////

class ExportProgressWindow : public QDialog
{
 Q_OBJECT

 public:

  ExportProgressWindow(QWidget* parent, const QString& directory, const EXPORTFORMAT exportformat, const uint64_t starttime, const uint64_t endtime, const std::vector< QSharedPointer<client::Recording> >& recordings);
  ~ExportProgressWindow();

 protected:

  virtual void timerEvent(QTimerEvent*) override;

 private:

  struct ExportTrackConnection : public Connection
  {
    ExportTrackConnection(const EXPORTFORMAT exportformat, const std::shared_ptr<std::recursive_mutex>& filewritermutex, const std::shared_ptr<file::FileWrite>& filewriter, const uint64_t starttime, const uint64_t endtime, const QDir& directory, const uint64_t deviceindex, const uint64_t recordingtoken, const QString& devicename, const QString& deviceaddress, const QString& recordingname, const QString& recordinglocation, const utility::PublicKey& publickey, const uint64_t trackid, const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port);
    ~ExportTrackConnection();

    void AddFrame(const ImageBuffer& imagebuffer, const float progress, const uint64_t timestamp, const size_t size);

    uint64_t GetCurrentSpeed() const;

    std::atomic<EXPORTTRACKSTATE> state_;
    EXPORTFORMAT exportformat_;
    std::shared_ptr<std::recursive_mutex> filewritermutex_;
    std::shared_ptr<file::FileWrite> filewriter_;
    uint64_t starttime_;
    uint64_t endtime_;
    QDir directory_;
    std::chrono::steady_clock::time_point end_;
    uint64_t deviceindex_;
    uint64_t recordingtoken_;
    QString devicename_;
    QString deviceaddress_;
    QString recordingname_;
    QString recordinglocation_;
    utility::PublicKey publickey_;
    uint64_t trackid_;
    std::recursive_mutex mutex_;
    std::unique_ptr<SaveVideo> savevideo_;
    VectorFreeFrameBuffer freeframebuffer_;
    sock::Connection connect_;
    monocle::client::Connection getauthenticatenonce_;
    monocle::client::Connection authenticate_;
    monocle::client::Connection createstream_;
    monocle::client::Connection controlstream_;
    std::vector<monocle::CODECINDEX> codecindices_;
    std::vector< std::unique_ptr<H265Decoder> > h265decoders_;
    std::vector< std::unique_ptr<H264Decoder> > h264decoders_;
    std::unique_ptr<MJpegDecoder> mjpegdecoder_;
    std::vector< std::unique_ptr<MPEG4Decoder> > mpeg4decoders_;
    std::atomic<float> currentprogress_;
    std::atomic<uint64_t> totalsize_;
    boost::circular_buffer< std::pair<std::chrono::steady_clock::time_point, size_t> > currentspeed_;
    float framerate_;
    uint64_t framecount_;
    std::vector<QString> audit_;

  };

  static void ControlStreamEndCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error, void* callbackdata);
  static void H265Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
  static void H264Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint32_t* offsets, const size_t numoffsets, const char* framedata, const size_t size, void* callbackdata);
  static void MetadataCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
  static void JPEGCallback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* framedata, const size_t size, void* callbackdata);
  static void MPEG4Callback(const uint64_t streamtoken, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* signaturedata, const size_t signaturedatasize, const char* framedata, const size_t size, void* callbackdata);
  static void NewCodecIndexCallback(const uint64_t streamtoken, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp, void* callbackdata);
  QString SpeedToString(const uint64_t speedkbps) const;

  const QDir directory_;
  const EXPORTFORMAT exportformat_;

  Ui::ExportProgressWindow ui_;

  std::vector< boost::shared_ptr<ExportTrackConnection> > exporttrackconnections_;
  std::shared_ptr<std::recursive_mutex> filewritermutex_;
  std::shared_ptr<file::FileWrite> filewriter_;
  std::chrono::steady_clock::time_point start_;
  int timer_;

 public slots:

  void on_buttonclose_clicked();
  void on_buttonopen_clicked();

};

}

QT_END_NAMESPACE

#endif
