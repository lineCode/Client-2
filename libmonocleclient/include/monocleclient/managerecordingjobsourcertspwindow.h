// managerecordingjobsourcertspwindow.h
//

#ifndef ID2VVADJSAKDSADSAD3ASDSLAJD9WKDN9G
#define ID2VVADJSAKDSADSAD3ASDSLAJD9WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <rtsp/client/client.hpp>
#include <rtsp/client/connection.hpp>
#include <socket/connection.hpp>
#include <QDialog>
#include <QSharedPointer>

#include "ui_managerecordingjobsourcertspwindow.h"

///// Declarations /////

namespace monocle
{

enum class ReceiverMode : int8_t;
enum class StreamingProtocol : int8_t;

}

namespace rtsp
{

namespace sdp { class MediaDescription; }

}

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Declarations /////

class Device;
class Receiver;
class Recording;
class RecordingJob;
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class ManageRecordingJobSourceRTSPWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingJobSourceRTSPWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const bool editing, const bool enabled, const uint32_t trackid, const QString& mediauri, const QString& sourcetag, const monocle::StreamingProtocol streamingprotocol, const QString& username, const QString& password, const QString& rotation, const std::vector<uint32_t>& reservedtrackids);
  ~ManageRecordingJobSourceRTSPWindow();

  bool enabled_;
  uint32_t trackid_;
  QString uri_;
  QString sourcetag_;
  monocle::StreamingProtocol streamingprotocol_;
  QString username_;
  QString password_;
  QString rotation_;

 private:

  void SetEnabled(const bool enabled);
  void AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription);

  Ui::ManageRecordingJobSourceRTSPWindow ui_;

  const boost::shared_ptr<Device> device_;
  const QSharedPointer<client::Recording> recording_;
  const QSharedPointer<client::RecordingJob> recordingjob_;

  sock::Connection rtspconnectconnection_;
  rtsp::client::Connection rtspconnection_;

  boost::shared_ptr< rtsp::Client<ManageRecordingJobSourceRTSPWindow> > rtspclient_;

 private slots:

  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void on_buttontest_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
