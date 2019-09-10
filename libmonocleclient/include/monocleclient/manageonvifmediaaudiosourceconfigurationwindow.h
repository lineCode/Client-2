// manageonvifmediaaudiosourceconfigurationwindow.h
//

#ifndef IDM9VVG9PH6SSS66566666666RQX7O960M
#define IDM9VVG9PH6SSS66566666666RQX7O960M

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediaaudiosourceconfigurationwindow.h"

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

class ManageONVIFMediaAudioSourceConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaAudioSourceConfigurationWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token);
  virtual ~ManageONVIFMediaAudioSourceConfigurationWindow();

  const QString& GetName() const { return name_; }

 private:
  
  void SetEnabled(bool enabled);
  void Init(const onvif::AudioSourceConfiguration& audiosourceconfiguration, const std::vector<onvif::AudioSource>& audiosources);

  Ui::ManageONVIFMediaAudioSourceConfigurationWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getaudiosourceconfigurationconnection_;
  onvif::ScopedConnection getaudiosourcesconnection_;
  onvif::ScopedConnection setaudiosourceconfigurationconnection_;

  QString name_;

 private slots:
  
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
