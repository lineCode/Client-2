// manageonvifmediaaudioencoderconfigurationswindow.h
//

#ifndef ID8Q7PFSP6U6GZOW54455555CKNNBU9WI7
#define ID8Q7PFSP6U6GZOW54455555CKNNBU9WI7

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediaaudioencoderconfigurationswindow.h"

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

class ManageONVIFMediaAudioEncoderConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaAudioEncoderConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaAudioEncoderConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFMediaAudioEncoderConfigurationsWindow ui_;

  const onvif::Capabilities devicecapabilities_;

  boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getaudioencoderconfigurationsconnection_;

 private slots:

  void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
