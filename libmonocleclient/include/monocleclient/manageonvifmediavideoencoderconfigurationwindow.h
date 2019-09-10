// manageonvifmediavideoencoderconfigurationwindow.h
//

#ifndef IDD9CEGUI9544545555555MWGHUFD5JFBZ
#define IDD9CEGUI9544545555555MWGHUFD5JFBZ

///// Includes /////

#include <QDialog>
#include <string>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

#include "ui_manageonvifmediavideoencoderconfigurationwindow.h"

///// Declarations /////

namespace onvif
{
namespace media
{
class MediaClient;
}
}

///// Namespaces /////

namespace client
{

///// Classes /////

class ManageONVIFMediaVideoEncoderConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaVideoEncoderConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token);
  virtual ~ManageONVIFMediaVideoEncoderConfigurationWindow();

  const QString& GetName() const { return name_; }

 protected:



 private:

  void InitDialog(const onvif::VideoEncoderConfiguration& videoencoderconfiguration);
  void InitJpeg();
  void InitMpeg4();
  void InitH264();

  const std::string token_;

  Ui::ManageONVIFMediaVideoEncoderConfigurationWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getvideoencoderconfigurationconnection_;
  onvif::ScopedConnection getvideoencoderconfigurationoptionsconnection_;
  onvif::ScopedConnection setvideoencoderconfigurationconnection_;

  onvif::VideoEncoderConfigurationOptions videoencoderconfigurationoptions_;
 
  QString name_;

 private slots:
 
  void on_sliderquality_valueChanged(int value);
  void on_slidermaxframerate_valueChanged(int value);
  void on_sliderencodinginterval_valueChanged(int value);
  void on_sliderbitrate_valueChanged(int value);
  void on_slidergovlength_valueChanged(int value);
  void on_combocodec_currentTextChanged(const QString& codec);
  void on_buttonok_clicked();

};

}

#endif
