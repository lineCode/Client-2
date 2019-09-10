// manageonvifmediametadataconfigurationswindow.h
//

#ifndef IDRAZCLE0JWG555555EFR423VRSB8VLOFY
#define IDRAZCLE0JWG555555EFR423VRSB8VLOFY

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediametadataconfigurationswindow.h"

///// Declarations /////

namespace onvif
{
namespace event
{
class EventClient;
}
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

class ManageONVIFMediaMetadataConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaMetadataConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::event::EventClient>& onvifevent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaMetadataConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFMediaMetadataConfigurationsWindow ui_;

  const onvif::Capabilities devicecapabilities_;
  const boost::shared_ptr<onvif::event::EventClient> onvifevent_;
  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getmetadataconfigurationsconnection_;

 private slots:

  void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
