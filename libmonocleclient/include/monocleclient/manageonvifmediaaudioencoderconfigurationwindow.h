// manageonvifmediaaudioencoderconfigurationwindow.h
//

#ifndef IDB5PMGTT9DGV6566663SYFQLP8GQB598F
#define IDB5PMGTT9DGV6566663SYFQLP8GQB598F

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediaaudioencoderconfigurationwindow.h"

///// Declarations /////

namespace onvif
{
namespace media
{
class MediaClient;
}
}

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Classes /////

class ManageONVIFMediaAudioEncoderConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaAudioEncoderConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token);
  virtual ~ManageONVIFMediaAudioEncoderConfigurationWindow();

  const QString& GetName() const { return name_; }

 private:
  
  void SetEnabled(bool enabled);
  void Init();
  void SelectEncoding(onvif::AUDIOENCODING encoding);

  Ui::ManageONVIFMediaAudioEncoderConfigurationWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getaudioencoderconfigurationconnection_;
  onvif::ScopedConnection getaudioencoderconfigurationoptionsconnection_;
  onvif::ScopedConnection setaudioencoderconfigurationconnection_;

  onvif::AudioEncoderConfiguration configuration_;
  std::vector<onvif::AudioEncoderConfigurationOption> options_;

  QString name_;

 private slots:
  
  void on_radiog711_clicked();
  void on_radiog726_clicked();
  void on_radioaac_clicked();
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
