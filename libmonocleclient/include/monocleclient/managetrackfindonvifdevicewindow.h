// managetrackfindonvifdevicewindow.h
//

#ifndef ID2VVCASAASDJSADISJADAALAJD9WKDN9G
#define ID2VVCASAASDJSADISJADAALAJD9WKDN9G

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

#include "ui_managetrackfindonvifdevicewindow.h"

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

class ManageTrackFindONVIFDeviceWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageTrackFindONVIFDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QString& mediauri, const QString& profiletoken, const QString& sourcetag, const QString& username, const QString& password);
  ~ManageTrackFindONVIFDeviceWindow();

  QString uri_;
  QString profiletoken_;
  QString sourcetag_;
  QString username_;
  QString password_;

 protected:

  virtual void timerEvent(QTimerEvent*) override;

 private:

  void SetEnabled(const bool enabled);
  void GetProfileCallback(const onvif::Profile& profile);
  void AddProfile(const onvif::Profile& profile);
  void AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription);

  Ui::ManageTrackFindONVIFDeviceWindow ui_;

  const boost::shared_ptr<Device> device_;
  const QSharedPointer<client::Recording> recording_;
  const QSharedPointer<client::RecordingJob> recordingjob_;

  onvif::Connection onvifconnection_;
  sock::Connection rtspconnectconnection_;
  rtsp::client::Connection rtspconnection_;

  boost::shared_ptr<onvif::device::DeviceClient> deviceclient_;
  boost::shared_ptr<onvif::media::MediaClient> mediaclient_;
  boost::shared_ptr< rtsp::Client<ManageTrackFindONVIFDeviceWindow> > rtspclient_;

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
