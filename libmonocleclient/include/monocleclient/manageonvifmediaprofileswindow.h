// manageonvifmediaprofileswindow.h
//

#ifndef ID2VV12ASMNSDLJ56565656655S3434N9G
#define ID2VV12ASMNSDLJ56565656655S3434N9G

///// Includes /////

#include "ui_manageonvifmediaprofileswindow.h"

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>
#include <QObject>
#include <QString>

///// Declarations /////

namespace onvif
{
class Capabilities;
class Profile;

namespace analytics
{
class AnalyticsClient;
};
namespace device
{
class DeviceClient;
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

class ManageONVIFMediaProfilesWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaProfilesWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::analytics::AnalyticsClient>& onvifanalytics, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const boost::shared_ptr<onvif::ptz::PTZClient>& onvifptz);
  ~ManageONVIFMediaProfilesWindow();

 protected:



 private:

  void SetEnabled(bool enabled);
  void AddItem(const onvif::Profile& profile);

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::analytics::AnalyticsClient> onvifanalytics_;
  const boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;
  const boost::shared_ptr<onvif::ptz::PTZClient> onvifptz_;

  Ui::ManageONVIFMediaWindow ui_;

  onvif::ScopedConnection getprofilesconnection_;
  onvif::ScopedConnection deleteprofilesconnection_;

 private slots:

  void on_buttoncreate_clicked();
  void on_buttondelete_clicked();
  void on_buttonmanage_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
