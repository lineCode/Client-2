// manageonvifmediaaudiooutputconfigurationwindow.h
//

#ifndef IDOHLWTTT6546666666XYPS9W5MEKD8023
#define IDOHLWTTT6546666666XYPS9W5MEKD8023

///// Includes /////

#include <boost/shared_ptr.hpp>
#include <onvifclient/scopedconnection.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QDialog>

#include "ui_manageonvifmediaaudiooutputconfigurationwindow.h"

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

class ManageONVIFMediaAudioOutputConfigurationWindow : public QDialog
{
 Q_OBJECT

 public:

  ManageONVIFMediaAudioOutputConfigurationWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token);
  virtual ~ManageONVIFMediaAudioOutputConfigurationWindow();

  const QString& GetName() const { return name_; }

 private:
  
  void SetEnabled(bool enabled);
  void Init(const onvif::AudioOutputConfiguration& configuration, onvif::AudioOutputConfigurationOptions& configurationoptions);

  Ui::ManageONVIFMediaAudioOutputConfigurationWindow ui_;

  const boost::shared_ptr<onvif::media::MediaClient> onvifmedia_;

  onvif::ScopedConnection getaudiooutputconfigurationconnection_;
  onvif::ScopedConnection getaudiooutputconfigurationoptionsconnection_;
  onvif::ScopedConnection setaudiooutputconfigurationconnection_;

  QString name_;

 private slots:
  
  void on_buttonok_clicked();

};

}

QT_END_NAMESPACE

#endif
