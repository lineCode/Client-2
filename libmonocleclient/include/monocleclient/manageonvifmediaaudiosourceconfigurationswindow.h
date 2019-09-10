// manageonvifmediaaudiosourceconfigurationwindow.h
//

#ifndef IDRN4P4QVV7X76767IWKMDIETS9WZOBLVF
#define IDRN4P4QVV7X76767IWKMDIETS9WZOBLVF

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediaaudiosourceconfigurationswindow.h"

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

class ManageONVIFMediaAudioSourceConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaAudioSourceConfigurationsWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaAudioSourceConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFMediaAudioSourceConfigurationsWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getaudiosourceconfigurationsconnection_;

 private slots:

   void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
