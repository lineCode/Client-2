// manageonvifmediavideosourceconfigurationswindow.h
//

#ifndef IDT33KKLT486566666666IQRHSY1YS3A7I
#define IDT33KKLT486566666666IQRHSY1YS3A7I

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <QDialog>

#include "ui_manageonvifmediavideosourceconfigurationswindow.h"

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

class ManageONVIFMediaVideoSourceConfigurationsWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaVideoSourceConfigurationsWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia);
  virtual ~ManageONVIFMediaVideoSourceConfigurationsWindow();

 private:

  void SetEnabled(bool enabled);

  Ui::ManageONVIFMediaVideoSourceConfigurationsWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getvideosourceconfigurationsconnection_;

 private slots:

  void on_buttonmanage_clicked();

};

}

QT_END_NAMESPACE

#endif
