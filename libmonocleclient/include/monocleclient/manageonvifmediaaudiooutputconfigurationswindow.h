// manageonvifmediaaudiooutputconfigurationswindow.h
//

#ifndef IDIYLURZGS8U375656566Z0QUWM7WS3OK3
#define IDIYLURZGS8U375656566Z0QUWM7WS3OK3

///// Includes /////

#include <onvifclient/scopedconnection.hpp>
#include <QDialog>

#include "ui_manageonvifmediaaudiooutputconfigurationswindow.h"

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

class ManageONVIFMediaAudioOutputConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaAudioOutputConfigurationsWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaAudioOutputConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFMediaAudioOutputConfigurationsWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getaudiooutputconfigurationsconnection_;

 private slots:

   void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
