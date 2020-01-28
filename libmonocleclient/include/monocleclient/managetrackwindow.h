// managetrackwindow.h
//

#ifndef ID2VV1211212SADSADASDASDDD1212DN9G
#define ID2VV1211212SADSADASDASDDD1212DN9G

///// Includes /////

#include "ui_managetrackwindow.h"

#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <onvifclient/connection.hpp>
#include <rtsp/client/client.hpp>
#include <rtsp/client/connection.hpp>
#include <QSharedPointer>

///// Declarations /////

namespace monocle { enum class TrackType : int8_t; }

///// Namespaces /////

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
class Recording;
class RecordingJob;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class ManageTrackWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageTrackWindow(QWidget* parent, boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& recordingtrack);
  ~ManageTrackWindow();

 protected:

  void timerEvent(QTimerEvent*) override;

 private:

  QSharedPointer<RecordingJob> GetJob() const;
  void DisableSource();
  void AddProfile(const onvif::Profile& profile);
  void AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription);
  void GetProfileCallback(const onvif::Profile& profile);
  void RTSPCallback(const std::string& uri, const std::string& host, const uint16_t port, const onvif::Profile& profile);

  Ui::ManageTrackWindow ui_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;
  QSharedPointer<RecordingJobSourceTrack> recordingjobsourcetrack_;
  QSharedPointer<RecordingTrack> recordingtrack_;

  boost::shared_ptr<onvif::device::DeviceClient> deviceclient_;
  boost::shared_ptr<onvif::media::MediaClient> mediaclient_;
  boost::shared_ptr< rtsp::Client<ManageTrackWindow> > rtspclient_;

  int accuracy_; // 0=Low,1=Medium,2=High,3=VeryHigh
  bool humans_;
  bool bicycles_;
  bool cars_;
  bool motorbikes_;
  bool buses_;
  bool trucks_;
  bool backpacks_;
  bool umbrellas_;
  bool handbags_;
  bool suitcases_;
  bool cats_;
  bool dogs_;
  bool aeroplanes_;
  bool trains_;
  bool boats_;
  bool horses_;
  double humanssensitivity_;
  double bicyclessensitivity_;
  double carssensitivity_;
  double motorbikessensitivity_;
  double busessensitivity_;
  double truckssensitivity_;
  double backpackssensitivity_;
  double umbrellassensitivity_;
  double handbagssensitivity_;
  double suitcasessensitivity_;
  double catssensitivity_;
  double dogssensitivity_;
  double aeroplanessensitivity_;
  double trainssensitivity_;
  double boatssensitivity_;
  double horsessensitivity_;

  monocle::client::Connection addtrack2connection_;

  onvif::Connection onvifconnection_;
  sock::Connection rtspconnectconnection_;
  rtsp::client::Connection rtspconnection_;

 private slots:

  void on_edituri_textChanged(const QString& text);
  void on_checkfixedfiles_stateChanged(int);
  void on_checkobjectdetector_stateChanged(int);
  void on_buttonobjectdetectorsettings_clicked();
  void on_buttonfindonvifdevice_clicked();
  void on_buttontest_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
