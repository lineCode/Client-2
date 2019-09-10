// manageonvifmediavideoencoderconfigurationswindow.h
//

#ifndef IDAN6E9YPHG45455555XQD6NB9SEGJCJYU
#define IDAN6E9YPHG45455555XQD6NB9SEGJCJYU

///// Includes /////

#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediavideoencoderconfigurationswindow.h"

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

class ManageONVIFMediaVideoEncoderConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaVideoEncoderConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaVideoEncoderConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFMediaVideoEncoderConfigurationsWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getvideoencoderconfigurationsconnection_;

 private slots:

  void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
