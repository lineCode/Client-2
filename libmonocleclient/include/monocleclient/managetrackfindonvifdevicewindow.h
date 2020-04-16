// managetrackfindonvifdevicewindow.h
//

#ifndef ID2VVCASAASDJSADISJADAALAJD9WKDN9G
#define ID2VVCASAASDJSADISJADAALAJD9WKDN9G

///// Includes /////

#include <boost/optional.hpp>
#include <memory>
#include <monocleprotocol/client/connection.hpp>
#include <onvifclient/connection.hpp>
#include <socket/connection.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QString>

#include "ui_managetrackfindonvifdevicewindow.h"

///// Declarations /////

namespace onvif
{
namespace device { class DeviceClient; }
namespace media { class MediaClient; }
class Profile;
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

  ManageTrackFindONVIFDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QString& mediauri, const QString& username, const QString& password);
  ~ManageTrackFindONVIFDeviceWindow();

  QString uri_;
  QString username_;
  QString password_;
  QString profile_;

 protected:

  virtual void timerEvent(QTimerEvent*) override;

 private:

  void SetEnabled(const bool enabled);

  Ui::ManageTrackFindONVIFDeviceWindow ui_;

  const boost::shared_ptr<Device> device_;

  onvif::Connection onvifconnection_;

  boost::shared_ptr<onvif::device::DeviceClient> deviceclient_;
  boost::shared_ptr<onvif::media::MediaClient> mediaclient_;

 private slots:

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
