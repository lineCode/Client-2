// managerecordingjobsourceonvifwindow.h
//

#ifndef ID2VVCASASDKSADL45944DSLAJD9WKDN9G
#define ID2VVCASASDKSADL45944DSLAJD9WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <onvifclient/connection.hpp>
#include <rtsp/client/client.hpp>
#include <rtsp/client/connection.hpp>
#include <socket/connection.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QString>

#include "ui_managerecordingjobsourceonvifwindow.h"

///// Declarations /////

namespace monocle
{
enum class ReceiverMode : int8_t;
enum class StreamingProtocol : int8_t;
}

namespace onvif
{
namespace device { class DeviceClient; }
namespace media { class MediaClient; }
class Profile;
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

class ManageRecordingJobSourceONVIFWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageRecordingJobSourceONVIFWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<client::Recording>& recording, const QSharedPointer<client::RecordingJob>& recordingjob, const bool editing, const bool enabled, const uint32_t trackid, const QString& mediauri, const QString& profiletoken, const QString& sourcetag, const monocle::StreamingProtocol streamingprotocol, const QString& username, const QString& password, const QString& rotation, const std::vector<uint32_t>& reservedtrackids);
  ~ManageRecordingJobSourceONVIFWindow();

  bool enabled_;
  uint32_t trackid_;
  QString uri_;
  QString profiletoken_;
  QString sourcetag_;
  monocle::StreamingProtocol streamingprotocol_;
  QString username_;
  QString password_;
  QString rotation_;

 protected:

  virtual void timerEvent(QTimerEvent*) override;

 private:

  void SetEnabled(const bool enabled);
  void GetProfileCallback(const onvif::Profile& profile);
  void AddProfile(const onvif::Profile& profile);
  void AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription);

  Ui::ManageRecordingJobSourceONVIFWindow ui_;

  const boost::shared_ptr<Device> device_;
  const QSharedPointer<client::Recording> recording_;
  const QSharedPointer<client::RecordingJob> recordingjob_;

  onvif::Connection onvifconnection_;
  sock::Connection rtspconnectconnection_;
  rtsp::client::Connection rtspconnection_;

  boost::shared_ptr<onvif::device::DeviceClient> deviceclient_;
  boost::shared_ptr<onvif::media::MediaClient> mediaclient_;
  boost::shared_ptr< rtsp::Client<ManageRecordingJobSourceONVIFWindow> > rtspclient_;

 private slots:

  void TrackAdded(const QSharedPointer<client::RecordingTrack>& track);
  void TrackChanged(const QSharedPointer<client::RecordingTrack>& track);
  void TrackRemoved(const uint32_t id);
  void on_edittextfilter_textChanged();
  void on_checkipv4_clicked();
  void on_checkipv6_clicked();
  void on_checklinklocal_clicked();
  void on_buttondiscoverrefresh_clicked();
  void on_treediscovery_itemPressed(QTreeWidgetItem* current, int column);
  void on_editusername_editingFinished();
  void on_editpassword_editingFinished();
  void on_buttontest_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
