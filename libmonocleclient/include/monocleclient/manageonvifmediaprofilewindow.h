// manageonvifmediaprofilewindow.h
//

#ifndef IDH00PMDWR5O4CO4564545455453EIR53U
#define IDH00PMDWR5O4CO4564545455453EIR53U

///// Includes /////

#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>
#include <QSharedPointer>
#include <QString>

#include "ui_manageonvifmediaprofilewindow.h"

///// Declarations /////

class QProgressBar;
class QStatusBar;

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

namespace client
{
  
///// Classes /////

class ManageONVIFMediaProfileWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaProfileWindow(QWidget* parent, const onvif::Capabilities devicecapabilities, const boost::shared_ptr<onvif::analytics::AnalyticsClient>& onvifanalytics, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const boost::shared_ptr<onvif::ptz::PTZClient>& onvifptz, const std::string& profiletoken);
  virtual ~ManageONVIFMediaProfileWindow();

 protected:


 private:

  void Setup(const onvif::Profile& profile, const std::vector<onvif::AudioDecoderConfiguration>& audiodecoderconfigurations, const std::vector<onvif::AudioEncoderConfiguration>& audioencoderconfigurations, const std::vector<onvif::AudioOutputConfiguration>& audiooutputconfigurations, const std::vector<onvif::AudioSourceConfiguration>& audiosourceconfigurations, const std::vector<onvif::MetadataConfiguration>& metadataconfigurations, const std::vector<onvif::PTZConfiguration>& ptzconfigurations, const std::vector<onvif::VideoEncoderConfiguration>& videoencoderconfigurations, const std::vector<onvif::VideoAnalyticsConfiguration>& videoanalyticsconfigurations, const std::vector<onvif::VideoSourceConfiguration>& videosourceconfigurations);
  void Select(QComboBox* combo, const std::string& token);
  void Accept(const std::string& audiodecoderconfigurationmessage, const std::string& audioencoderconfigurationmessage, const std::string& audiooutputconfigurationmessage, const std::string& audiosourceconfigurationmessage, const std::string& metadataconfigurationmessage, const std::string& ptzconfigurationmessage, const std::string& videoencoderconfigurationmessage, const std::string& videoanalyticsconfigurationmessage, const std::string& videosourceconfigurationmessage);
  void SetProgress(unsigned int currentcount);

  const boost::shared_ptr<onvif::analytics::AnalyticsClient> onvifanalytics_;
  const boost::shared_ptr<onvif::device::DeviceClient> onvifdevice_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;
  const boost::shared_ptr<onvif::ptz::PTZClient> onvifptz_;
  const std::string profiletoken_;

  Ui::ManageONVIFMediaProfileWindow ui_;
  QStatusBar* statusbar_;
  QProgressBar* progressbar_;

  unsigned int totalcount_; // This counts the number of transactions we are waiting for completion, both in constructor and buttonok

  onvif::ScopedConnection getprofileconnection_;
  onvif::ScopedConnection getcompatibleaudiodecoderconfigurationsconnection_;
  onvif::ScopedConnection getcompatibleaudioencoderconfigurationsconnection_;
  onvif::ScopedConnection getcompatibleaudiooutputconfigurationsconnection_;
  onvif::ScopedConnection getcompatibleaudiosourceconfigurationsconnection_;
  onvif::ScopedConnection getcompatiblemetadataconfigurationsconnection_;
  onvif::ScopedConnection getptzconfigurationsconnection_;
  onvif::ScopedConnection getcompatiblevideoencoderconfigurationsconnection_;
  onvif::ScopedConnection getcompatiblevideoanalyticsconfigurationsconnection_;
  onvif::ScopedConnection getcompatiblevideosourceconfigurationsconnection_;
  onvif::ScopedConnection getservicecapabilitiesconnection_;

  onvif::ScopedConnection audiodecoderconfigurationconnection_;
  onvif::ScopedConnection audioencoderconfigurationconnection_;
  onvif::ScopedConnection audiooutputconfigurationconnection_;
  onvif::ScopedConnection audiosourceconfigurationconnection_;
  onvif::ScopedConnection metadataconfigurationconnection_;
  onvif::ScopedConnection ptzconfigurationconnection_;
  onvif::ScopedConnection videoencoderconfigurationconnection_;
  onvif::ScopedConnection videoanalyticsconfigurationconnection_;
  onvif::ScopedConnection videosourceconfigurationconnection_;

  onvif::Profile profile_;

 private slots:
 
  void on_buttonok_clicked();

};

}

#endif
