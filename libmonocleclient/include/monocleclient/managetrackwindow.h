// managetrackwindow.h
//

#ifndef ID2VV1211212SADSADASDASDDD1212DN9G
#define ID2VV1211212SADSADASDASDDD1212DN9G

///// Includes /////

#include "ui_managetrackwindow.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <monocleprotocol/client/connection.hpp>
#include <mutex>
#include <onvifclient/connection.hpp>
#include <rtsp/client/client.hpp>
#include <rtsp/client/connection.hpp>
#include <QCompleter>
#include <QSharedPointer>
#include <QStringListModel>
#include <vector>

#include "monocleclient/file.h"

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
class RecordingJobSource;
class RecordingJobSourceTrack;
class RecordingTrack;

///// Classes /////

class ManageTrackWindow : public QDialog, public boost::enable_shared_from_this<ManageTrackWindow>
{
 Q_OBJECT

 public:

  ManageTrackWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QSharedPointer<Recording>& recording, const QSharedPointer<RecordingJob>& recordingjob, const QSharedPointer<RecordingJobSource>& recordingjobsource, const QSharedPointer<RecordingJobSourceTrack>& recordingjobsourcetrack, const QSharedPointer<RecordingTrack>& recordingtrack, const QString& uri);
  ~ManageTrackWindow();

  std::recursive_mutex& GetMutex() { return mutex_; }

 protected:

  void timerEvent(QTimerEvent*) override;

 private:

  QSharedPointer<RecordingJob> GetJob() const;
  void DisableSource();
  void AddProfile(const onvif::Profile& profile);
  void AddMediaDescription(const rtsp::sdp::MediaDescription& mediadescription);
  void GetProfileCallback(const onvif::Profile& profile);
  void RTSPCallback(const std::string& uri, const std::string& host, const uint16_t port);
  void SetTrack(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t objectdetectortrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken);
  void SetEnabled(const bool enabled);

  boost::shared_ptr<std::recursive_mutex> mutex_;

  Ui::ManageTrackWindow ui_;

  QStringListModel* profilemodel_;
  QStringListModel* sourcetagmodel_;
  QCompleter* profilecompleter_;
  QCompleter* sourcetagcompleter_;

  boost::shared_ptr<Device> device_;
  QSharedPointer<Recording> recording_;
  QSharedPointer<RecordingJob> recordingjob_;
  QSharedPointer<RecordingJobSource> recordingjobsource_;
  QSharedPointer<RecordingJobSourceTrack> recordingjobsourcetrack_;
  QSharedPointer<RecordingTrack> recordingtrack_;

  boost::shared_ptr<onvif::device::DeviceClient> deviceclient_;
  boost::shared_ptr<onvif::media::MediaClient> mediaclient_;
  boost::shared_ptr< rtsp::Client<ManageTrackWindow> > rtspclient_;

  std::vector< QSharedPointer<File> > files_;

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

  void TrackRemoved(const uint32_t id);
  void on_combodevice_currentIndexChanged(const QString&);
  void on_edituri_textChanged(const QString&);
  void on_checkfixedfiles_stateChanged(int);
  void on_checkobjectdetector_stateChanged(int);
  void on_buttonobjectdetectorsettings_clicked();
  void on_buttonfiles_clicked();
  void on_buttonfindonvifdevice_clicked();
  void on_buttontest_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
