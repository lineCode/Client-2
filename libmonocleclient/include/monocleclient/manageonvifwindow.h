// manageonvifwindow.h
//

#ifndef ID2VV12ASMNDA4554545454544NDN2DN9G
#define ID2VV12ASMNDA4554545454544NDN2DN9G

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <mutex>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>
#include <QObject>
#include <QString>
#include <socket/connection.hpp>
#include <socket/proxyparams.hpp>

#include "ui_manageonvifwindow.h"

///// Declarations /////

namespace onvif
{
namespace analytics
{
class AnalyticsClient;
};
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

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class ManageONVIFWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFWindow(QWidget* parent, const sock::ProxyParams& proxyparams, const QString& mediauri, const QString& username, const QString& password);
  ~ManageONVIFWindow();

 protected:

  void timerEvent(QTimerEvent *event) override;

 private:

  std::recursive_mutex mutex_;
 
  Ui::ManageONVIFWindow ui_;

  sock::ProxyParams proxyparams_;
  QString mediauri_;
  QString username_;
  QString password_;

  boost::shared_ptr<onvif::analytics::AnalyticsClient> onvifanalytics_;
  boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;
  boost::shared_ptr<onvif::event::EventClient> onvifevent_;
  boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;
  boost::shared_ptr<onvif::ptz::PTZClient> onvifptz_;

  onvif::ScopedConnection onvifconnection_;

  onvif::Capabilities devicecapabilities_;

 private slots:

  void on_buttonmanageipv4networkinterfaces_clicked();
  void on_buttonmanagemediaprofiles_clicked();
  void on_buttonmanageaudioencoderconfigurations_clicked();
  void on_buttonmanageaudiooutputconfigurations_clicked();
  void on_buttonmanageaudiosourceconfigurations_clicked();
  void on_buttonmediamanagemetadataconfigurations_clicked();
  void on_buttonmanagevideoencoderconfigurations_clicked();
  void on_buttonmanagevideosourceconfigurations_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
