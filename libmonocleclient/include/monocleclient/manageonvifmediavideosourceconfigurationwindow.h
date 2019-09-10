// manageonvifmediavideosourceconfigurationwindow.h
//

#ifndef IDT3H7MEVZJ434344441ZUAR4592SLWSLC
#define IDT3H7MEVZJ434344441ZUAR4592SLWSLC

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <QDialog>
#include <string>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

#include "ui_manageonvifmediavideosourceconfigurationwindow.h"

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

class ManageONVIFMediaVideoSourceConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaVideoSourceConfigurationWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token);
  virtual ~ManageONVIFMediaVideoSourceConfigurationWindow();

  const QString GetName() const { return name_; }

 protected:



 private:

  void InitDialog(const onvif::VideoSourceConfiguration& videosourceconfiguration);

  const std::string token_;

  Ui::ManageONVIFMediaVideoSourceConfigurationWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getvideosourceconfigurationconnection_;
  onvif::ScopedConnection getvideosourceconfigurationoptionsconnection_;
  onvif::ScopedConnection setvideosourceconfigurationconnection_;

  onvif::VideoSourceConfigurationOptions videosourceconfigurationoptions_;

  QString name_;

 private slots:
 
  void on_buttonok_clicked();

};

}

#endif
