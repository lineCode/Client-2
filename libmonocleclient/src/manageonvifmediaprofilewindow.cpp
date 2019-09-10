// manageonvifmediaprofilewindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaprofilewindow.h"

#include <onvifclient/analyticsclient.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <onvifclient/ptzclient.hpp>
#include <onviftypes/onviftypes.hpp>
#include <QMessageBox>
#include <QProgressBar>
#include <QStatusBar>

#include "monocleclient/devicemgr.h"
#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ManageONVIFMediaProfileWindow::ManageONVIFMediaProfileWindow(QWidget* parent, const onvif::Capabilities devicecapabilities, const boost::shared_ptr<onvif::analytics::AnalyticsClient>& onvifanalytics, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const boost::shared_ptr<onvif::ptz::PTZClient>& onvifptz, const std::string& profiletoken) :
  QDialog(parent),
  statusbar_(new QStatusBar(this)),
  progressbar_(new QProgressBar(this)),
  onvifanalytics_(onvifanalytics),
  onvifdevice_(onvifdevice),
  onvifmedia_(onvifmedia),
  onvifptz_(onvifptz),
  profiletoken_(profiletoken),
  totalcount_(0)
{
  ui_.setupUi(this);

  progressbar_->setValue(0);
  progressbar_->setMinimum(0);
  progressbar_->setMaximum(100);
  progressbar_->setFormat("%p%");
  statusbar_->setSizeGripEnabled(false);
  statusbar_->insertPermanentWidget(0, progressbar_, 1);
  layout()->addWidget(statusbar_);

  const QSize sizehint = sizeHint();
  setMinimumHeight(sizehint.height());
  setMaximumHeight(sizehint.height());

  setWindowTitle(windowTitle() + QString(": ") + QString::fromStdString(profiletoken));

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);
  
  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(0);
  std::shared_ptr<onvif::Profile> profile = std::make_shared<onvif::Profile>();
  std::shared_ptr< std::vector<onvif::AudioDecoderConfiguration> > audiodecoderconfigurations = std::make_shared< std::vector<onvif::AudioDecoderConfiguration> >();
  std::shared_ptr< std::vector<onvif::AudioEncoderConfiguration> > audioencoderconfigurations = std::make_shared< std::vector<onvif::AudioEncoderConfiguration> >();
  std::shared_ptr< std::vector<onvif::AudioOutputConfiguration> > audiooutputconfigurations = std::make_shared< std::vector<onvif::AudioOutputConfiguration> >();
  std::shared_ptr< std::vector<onvif::AudioSourceConfiguration> > audiosourceconfigurations = std::make_shared< std::vector<onvif::AudioSourceConfiguration> >();
  std::shared_ptr< std::vector<onvif::MetadataConfiguration> > metadataconfigurations = std::make_shared< std::vector<onvif::MetadataConfiguration> >();
  std::shared_ptr< std::vector<onvif::PTZConfiguration> > ptzconfigurations = std::make_shared< std::vector<onvif::PTZConfiguration> >();
  std::shared_ptr< std::vector<onvif::VideoEncoderConfiguration> > videoencoderconfigurations = std::make_shared< std::vector<onvif::VideoEncoderConfiguration> >();
  std::shared_ptr< std::vector<onvif::VideoAnalyticsConfiguration> > videoanalyticsconfigurations = std::make_shared< std::vector<onvif::VideoAnalyticsConfiguration> >();
  std::shared_ptr< std::vector<onvif::VideoSourceConfiguration> > videosourceconfigurations = std::make_shared< std::vector<onvif::VideoSourceConfiguration> >();

  totalcount_ = 1;
  getprofileconnection_ = onvifmedia_->GetProfileCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetProfileResponse& getprofileresponse)
  {
    if (getprofileresponse.Error() || !getprofileresponse.profile_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getprofileresponse.Message()), QMessageBox::Ok);
      return;
    }

    *profile = *getprofileresponse.profile_;

    ++(*count);
    SetProgress(*count);
    if (*count == totalcount_)
    {
      Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

    }
  });

  if (!devicecapabilities.deviceio_.is_initialized() || !devicecapabilities.deviceio_->audiooutputs_.is_initialized() || (*devicecapabilities.deviceio_->audiooutputs_ != 0))
  {
    totalcount_ += 2;
    getcompatibleaudiodecoderconfigurationsconnection_ = onvifmedia_->GetCompatibleAudioDecoderConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleAudioDecoderConfigurationsResponse& getacompatibleudiodecoderconfigurationsresponse)
    {
      if (getacompatibleudiodecoderconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getacompatibleudiodecoderconfigurationsresponse.Message()));

      }

      *audiodecoderconfigurations = getacompatibleudiodecoderconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });

    getcompatibleaudiooutputconfigurationsconnection_ = onvifmedia_->GetCompatibleAudioOutputConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleAudioOutputConfigurationsResponse& getcompatibleaudiooutputconfigurationsresponse)
    {
      if (getcompatibleaudiooutputconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getcompatibleaudiooutputconfigurationsresponse.Message()));

      }

      *audiooutputconfigurations = getcompatibleaudiooutputconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });
  }

  if (!devicecapabilities.deviceio_.is_initialized() || !devicecapabilities.deviceio_->audiosources_.is_initialized() || (*devicecapabilities.deviceio_->audiosources_ != 0))
  {
    totalcount_ += 2;
    getcompatibleaudioencoderconfigurationsconnection_ = onvifmedia_->GetCompatibleAudioEncoderConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleAudioEncoderConfigurationsResponse& getacompatibleudioencoderconfigurationsresponse)
    {
      if (getacompatibleudioencoderconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getacompatibleudioencoderconfigurationsresponse.Message()));

      }

      *audioencoderconfigurations = getacompatibleudioencoderconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });

    getcompatibleaudiosourceconfigurationsconnection_ = onvifmedia_->GetCompatibleAudioSourceConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleAudioSourceConfigurationsResponse& getcompatibleaudiosourceconfigurationsresponse)
    {
      if (getcompatibleaudiosourceconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getcompatibleaudiosourceconfigurationsresponse.Message()));

      }

      *audiosourceconfigurations = getcompatibleaudiosourceconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });
  }

  ++totalcount_;
  getcompatiblemetadataconfigurationsconnection_ = onvifmedia_->GetCompatibleMetadataConfigurationsCallback(profiletoken, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleMetadataConfigurationsResponse& getmcompatibleetadataconfigurationsresponse)
  {
    if (getmcompatibleetadataconfigurationsresponse.Error())
    {
      LOG_GUI_WARNING(QString::fromStdString(getmcompatibleetadataconfigurationsresponse.Message()));

    }

    *metadataconfigurations = getmcompatibleetadataconfigurationsresponse.configurations_;

    ++(*count);
    SetProgress(*count);
    if (*count == totalcount_)
    {
      Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

    }
  });

  if (onvifptz_->IsInitialised())
  {
    ++totalcount_;
    getservicecapabilitiesconnection_ = onvifptz_->GetServiceCapabilitiesCallback([this, profiletoken, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::ptz::GetServiceCapabilitiesResponse& getservicecapabilitiesresponse)
    {
      if (!getservicecapabilitiesresponse.Error() && getservicecapabilitiesresponse.capabilities_.is_initialized() && getservicecapabilitiesresponse.capabilities_->SupportsGetCompatibleConfigurations())
      {
        getptzconfigurationsconnection_ = onvifptz_->GetCompatibleConfigurationsCallback(profiletoken, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::ptz::GetCompatibleConfigurationsResponse& getcompatibleconfigurationsresponse)
        {
          if (getcompatibleconfigurationsresponse.Error())
          {
            LOG_GUI_WARNING(QString::fromStdString(getcompatibleconfigurationsresponse.Message()));

          }

          *ptzconfigurations = getcompatibleconfigurationsresponse.ptzconfigurations_;

          ++(*count);
          SetProgress(*count);
          if (*count == totalcount_)
          {
            Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

          }
        });
      }
      else
      {
        getptzconfigurationsconnection_ = onvifptz_->GetConfigurationsCallback([this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::ptz::GetConfigurationsResponse& getconfigurationsresponse)
        {
          if (getconfigurationsresponse.Error())
          {
            LOG_GUI_WARNING(QString::fromStdString(getconfigurationsresponse.Message()));

          }

          *ptzconfigurations = getconfigurationsresponse.ptzconfiguration_;

          ++(*count);
          SetProgress(*count);
          if (*count == totalcount_)
          {
            Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

          }
        });
      }
    });
  }

  if (onvifanalytics_->IsInitialised())
  {
    ++totalcount_;
    getcompatiblevideoanalyticsconfigurationsconnection_ = onvifmedia_->GetCompatibleVideoAnalyticsConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleVideoAnalyticsConfigurationsResponse& getcompatiblevideoanalyticsconfigurationsresponse)
    {
      if (getcompatiblevideoanalyticsconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getcompatiblevideoanalyticsconfigurationsresponse.Message()));

      }

      *videoanalyticsconfigurations = getcompatiblevideoanalyticsconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });
  }

  if (!devicecapabilities.deviceio_.is_initialized() || !devicecapabilities.deviceio_->videosources_.is_initialized() || (*devicecapabilities.deviceio_->videosources_ != 0))
  {
    totalcount_ += 2;
    getcompatiblevideosourceconfigurationsconnection_ = onvifmedia_->GetCompatibleVideoSourceConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleVideoSourceConfigurationsResponse& getcompatiblevideosourceconfigurationsresponse)
    {
      if (getcompatiblevideosourceconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getcompatiblevideosourceconfigurationsresponse.Message()));

      }

      *videosourceconfigurations = getcompatiblevideosourceconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });

    getcompatiblevideoencoderconfigurationsconnection_ = onvifmedia_->GetCompatibleVideoEncoderConfigurationsCallback(profiletoken_, [this, count, profile, audiodecoderconfigurations, audioencoderconfigurations, audiooutputconfigurations, audiosourceconfigurations, metadataconfigurations, ptzconfigurations, videoencoderconfigurations, videoanalyticsconfigurations, videosourceconfigurations](const onvif::media::GetCompatibleVideoEncoderConfigurationsResponse& getcompatiblevideoencoderconfigurationsresponse)
    {
      if (getcompatiblevideoencoderconfigurationsresponse.Error())
      {
        LOG_GUI_WARNING(QString::fromStdString(getcompatiblevideoencoderconfigurationsresponse.Message()));

      }

      *videoencoderconfigurations = getcompatiblevideoencoderconfigurationsresponse.configurations_;

      ++(*count);
      SetProgress(*count);
      if (*count == totalcount_)
      {
        Setup(*profile, *audiodecoderconfigurations, *audioencoderconfigurations, *audiooutputconfigurations, *audiosourceconfigurations, *metadataconfigurations, *ptzconfigurations, *videoencoderconfigurations, *videoanalyticsconfigurations, *videosourceconfigurations);

      }
    });
  }
}

ManageONVIFMediaProfileWindow::~ManageONVIFMediaProfileWindow()
{
  QApplication::restoreOverrideCursor();

}

void ManageONVIFMediaProfileWindow::Setup(const onvif::Profile& profile, const std::vector<onvif::AudioDecoderConfiguration>& audiodecoderconfigurations, const std::vector<onvif::AudioEncoderConfiguration>& audioencoderconfigurations, const std::vector<onvif::AudioOutputConfiguration>& audiooutputconfigurations, const std::vector<onvif::AudioSourceConfiguration>& audiosourceconfigurations, const std::vector<onvif::MetadataConfiguration>& metadataconfigurations, const std::vector<onvif::PTZConfiguration>& ptzconfigurations, const std::vector<onvif::VideoEncoderConfiguration>& videoencoderconfigurations, const std::vector<onvif::VideoAnalyticsConfiguration>& videoanalyticsconfigurations, const std::vector<onvif::VideoSourceConfiguration>& videosourceconfigurations)
{
  profile_ = profile;

  // Fill in widgets
  if (videosourceconfigurations.size())
  {
    ui_.combovideosource->setEnabled(true);

    for (const auto& configuration : videosourceconfigurations)
    {
      if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid video source configuration"));
        continue;
      }

      ui_.combovideosource->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
    }
  }

  if (videoencoderconfigurations.size())
  {
    ui_.combovideoencoder->setEnabled(true);

    for (const auto& configuration : videoencoderconfigurations)
    {
      if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid video encoder configuration"));
        continue;
      }

      ui_.combovideoencoder->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
    }
  }

  for (const auto& configuration : audiosourceconfigurations)
  {
    ui_.comboaudiosource->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid audio source configuration"));
      continue;
    }

    ui_.comboaudiosource->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  for (const auto& configuration : audioencoderconfigurations)
  {
    ui_.comboaudioencoder->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid audio encoder configuration"));
      continue;
    }

    ui_.comboaudioencoder->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  for (const auto& configuration : videoanalyticsconfigurations)
  {
    ui_.combovideoanalytics->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid video analytics configuration"));
      continue;
    }

    ui_.combovideoanalytics->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  for (const auto& configuration : ptzconfigurations)
  {
    ui_.comboptz->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid ptz configuration"));
      continue;
    }

    ui_.comboptz->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  for (const auto& configuration : metadataconfigurations)
  {
    ui_.combometadata->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid metadata configuration"));
      continue;
    }

    ui_.combometadata->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  for (const auto& configuration : audiooutputconfigurations)
  {
    ui_.comboaudiooutput->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid audio output configuration"));
      continue;
    }

    ui_.comboaudiooutput->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  for (const auto& configuration : audiodecoderconfigurations)
  {
    ui_.comboaudiodecoder->setEnabled(true);

    if (!configuration.token_.is_initialized() || !configuration.name_.is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid audio decoder configuration"));
      continue;
    }

    ui_.comboaudiodecoder->addItem(QString::fromStdString(*configuration.name_) + QString("(") + QString::fromStdString(*configuration.token_) + QString(")"), QString::fromStdString(*configuration.token_));
  }

  ui_.buttonok->setEnabled(true);

  // Select items in widgets
  if (profile.videosourceconfiguration_.is_initialized() && profile.videosourceconfiguration_->token_.is_initialized())
  {
    Select(ui_.combovideosource, *profile.videosourceconfiguration_->token_);

  }

  if (profile.videoencoderconfiguration_.is_initialized() && profile.videoencoderconfiguration_->token_.is_initialized())
  {
    Select(ui_.combovideoencoder, *profile.videoencoderconfiguration_->token_);

  }

  if (profile.audiosourceconfiguration_.is_initialized() && profile.audiosourceconfiguration_->token_.is_initialized())
  {
    Select(ui_.comboaudiosource, *profile.audiosourceconfiguration_->token_);

  }

  if (profile.audioencoderconfiguration_.is_initialized() && profile.audioencoderconfiguration_->token_.is_initialized())
  {
    Select(ui_.comboaudioencoder, *profile.audioencoderconfiguration_->token_);

  }

  if (profile.videoanalyticsconfiguration_.is_initialized() && profile.videoanalyticsconfiguration_->token_.is_initialized())
  {
    Select(ui_.combovideoanalytics, *profile.videoanalyticsconfiguration_->token_);

  }

  if (profile.ptzconfiguration_.is_initialized() && profile.ptzconfiguration_->token_.is_initialized())
  {
    Select(ui_.comboptz, *profile.ptzconfiguration_->token_);

  }

  if (profile.metadataconfiguration_.is_initialized() && profile.metadataconfiguration_->token_.is_initialized())
  {
    Select(ui_.combometadata, *profile.metadataconfiguration_->token_);

  }

  if (profile.audiooutputconfiguration_.is_initialized() && profile.audiooutputconfiguration_->token_.is_initialized())
  {
    Select(ui_.comboaudiooutput, *profile.audiooutputconfiguration_->token_);

  }

  if (profile.audiodecoderconfiguration_.is_initialized() && profile.audiodecoderconfiguration_->token_.is_initialized())
  {
    Select(ui_.comboaudiodecoder, *profile.audiodecoderconfiguration_->token_);

  }
}

void ManageONVIFMediaProfileWindow::Select(QComboBox* combo, const std::string& token)
{
  for (int i = 0; i < combo->count(); ++i)
  {
    if (combo->itemData(i).toString() == QString::fromStdString(token))
    {
      combo->setCurrentIndex(i);
      return;
    }
  }
}

void ManageONVIFMediaProfileWindow::Accept(const std::string& audiodecoderconfigurationmessage, const std::string& audioencoderconfigurationmessage, const std::string& audiooutputconfigurationmessage, const std::string& audiosourceconfigurationmessage, const std::string& metadataconfigurationmessage, const std::string& ptzconfigurationmessage, const std::string& videoencoderconfigurationmessage, const std::string& videoanalyticsconfigurationmessage, const std::string& videosourceconfigurationmessage)
{
  QApplication::restoreOverrideCursor();
  ui_.buttonok->setEnabled(true);

  std::vector<std::string> messages;
  if (audiodecoderconfigurationmessage.size())
  {
    messages.push_back(audiodecoderconfigurationmessage);

  }

  if (audioencoderconfigurationmessage.size())
  {
    messages.push_back(audioencoderconfigurationmessage);

  }

  if (audiooutputconfigurationmessage.size())
  {
    messages.push_back(audiooutputconfigurationmessage);

  }

  if (audiosourceconfigurationmessage.size())
  {
    messages.push_back(audiosourceconfigurationmessage);

  }

  if (metadataconfigurationmessage.size())
  {
    messages.push_back(metadataconfigurationmessage);

  }

  if (ptzconfigurationmessage.size())
  {
    messages.push_back(ptzconfigurationmessage);

  }

  if (videoencoderconfigurationmessage.size())
  {
    messages.push_back(videoencoderconfigurationmessage);

  }

  if (videoanalyticsconfigurationmessage.size())
  {
    messages.push_back(videoanalyticsconfigurationmessage);

  }

  if (videosourceconfigurationmessage.size())
  {
    messages.push_back(videosourceconfigurationmessage);

  }

  std::string message = boost::join(messages, "\n");
  if (message.size())
  {
    QMessageBox::warning(this, tr("Error"), QString::fromStdString(message), QMessageBox::Ok);
    return;
  }

  accept();
}

void ManageONVIFMediaProfileWindow::SetProgress(unsigned int currentcount)
{
  if (currentcount == totalcount_)
  {
    statusbar_->hide();
    progressbar_->hide();
    const QSize sizehint = sizeHint();
    setMinimumHeight(sizehint.height());
    setMaximumHeight(sizehint.height());
  }
  else
  {
    statusbar_->show();
    progressbar_->show();
    progressbar_->setValue((currentcount * 100) / totalcount_);
  }
}

void ManageONVIFMediaProfileWindow::on_buttonok_clicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui_.buttonok->setEnabled(false);

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(0);
  std::shared_ptr<std::string> audiodecoderconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> audioencoderconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> audiooutputconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> audiosourceconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> metadataconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> ptzconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> videoanalyticsconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> videoencoderconfigurationmessage = std::make_shared<std::string>();
  std::shared_ptr<std::string> videosourceconfigurationmessage = std::make_shared<std::string>();
  
  totalcount_ = 0;
  if (ui_.comboaudiodecoder->isEnabled())
  {
    if (profile_.audiodecoderconfiguration_.is_initialized() && profile_.audiodecoderconfiguration_->token_.is_initialized() && (ui_.comboaudiodecoder->currentIndex() == 0))
    {
      ++totalcount_;
      audiodecoderconfigurationconnection_ = onvifmedia_->RemoveAudioDecoderConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveAudioDecoderConfigurationResponse& removeaudiodecoderconfigurationresponse)
      {
        *audiodecoderconfigurationmessage = removeaudiodecoderconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.audiodecoderconfiguration_.is_initialized() && profile_.audiodecoderconfiguration_->token_.is_initialized() && (ui_.comboaudiodecoder->currentData().toString() != QString::fromStdString(*profile_.audiodecoderconfiguration_->token_))) || ((!profile_.audiodecoderconfiguration_.is_initialized() || !profile_.audiodecoderconfiguration_->token_.is_initialized()) && (ui_.comboaudiodecoder->currentIndex() != 0)))
    {
      ++totalcount_;
      audiodecoderconfigurationconnection_ = onvifmedia_->AddAudioDecoderConfigurationCallback(profiletoken_, ui_.comboaudiodecoder->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddAudioDecoderConfigurationResponse& addaudiodecoderconfigurationresponse)
      {
        *audiodecoderconfigurationmessage = addaudiodecoderconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.comboaudioencoder->isEnabled())
  {
    if (profile_.audioencoderconfiguration_.is_initialized() && profile_.audioencoderconfiguration_->token_.is_initialized() && (ui_.comboaudioencoder->currentIndex() == 0))
    {
      ++totalcount_;
      audioencoderconfigurationconnection_ = onvifmedia_->RemoveAudioEncoderConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveAudioEncoderConfigurationResponse& removeaudioencoderconfigurationresponse)
      {
        *audioencoderconfigurationmessage = removeaudioencoderconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.audioencoderconfiguration_.is_initialized() && profile_.audioencoderconfiguration_->token_.is_initialized() && (ui_.comboaudioencoder->currentData().toString() != QString::fromStdString(*profile_.audioencoderconfiguration_->token_))) || ((!profile_.audioencoderconfiguration_.is_initialized() || !profile_.audioencoderconfiguration_->token_.is_initialized()) && (ui_.comboaudioencoder->currentIndex() != 0)))
    {
      ++totalcount_;
      audioencoderconfigurationconnection_ = onvifmedia_->AddAudioEncoderConfigurationCallback(profiletoken_, ui_.comboaudioencoder->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddAudioEncoderConfigurationResponse& addaudioencoderconfigurationresponse)
      {
        *audioencoderconfigurationmessage = addaudioencoderconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.comboaudiooutput->isEnabled())
  {
    if (profile_.audiooutputconfiguration_.is_initialized() && profile_.audiooutputconfiguration_->token_.is_initialized() && (ui_.comboaudiooutput->currentIndex() == 0))
    {
      ++totalcount_;
      audiooutputconfigurationconnection_ = onvifmedia_->RemoveAudioOutputConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveAudioOutputConfigurationResponse& removeaudiooutputconfigurationresponse)
      {
        *audiooutputconfigurationmessage = removeaudiooutputconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.audiooutputconfiguration_.is_initialized() && profile_.audiooutputconfiguration_->token_.is_initialized() && (ui_.comboaudiooutput->currentData().toString() != QString::fromStdString(*profile_.audiooutputconfiguration_->token_))) || ((!profile_.audiooutputconfiguration_.is_initialized() || !profile_.audiooutputconfiguration_->token_.is_initialized()) && (ui_.comboaudiooutput->currentIndex() != 0)))
    {
      ++totalcount_;
      audiooutputconfigurationconnection_ = onvifmedia_->AddAudioOutputConfigurationCallback(profiletoken_, ui_.comboaudiooutput->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddAudioOutputConfigurationResponse& addaudiooutputconfigurationresponse)
      {
        *audiooutputconfigurationmessage = addaudiooutputconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.comboaudiosource->isEnabled())
  {
    if (profile_.audiosourceconfiguration_.is_initialized() && profile_.audiosourceconfiguration_->token_.is_initialized() && (ui_.comboaudiosource->currentIndex() == 0))
    {
      ++totalcount_;
      audiosourceconfigurationconnection_ = onvifmedia_->RemoveAudioSourceConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveAudioSourceConfigurationResponse& removeaudiosourceconfigurationresponse)
      {
        *audiosourceconfigurationmessage = removeaudiosourceconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.audiosourceconfiguration_.is_initialized() && profile_.audiosourceconfiguration_->token_.is_initialized() && (ui_.comboaudiosource->currentData().toString() != QString::fromStdString(*profile_.audiosourceconfiguration_->token_))) || ((!profile_.audiosourceconfiguration_.is_initialized() || !profile_.audiosourceconfiguration_->token_.is_initialized()) && (ui_.comboaudiosource->currentIndex() != 0)))
    {
      ++totalcount_;
      audiosourceconfigurationconnection_ = onvifmedia_->AddAudioSourceConfigurationCallback(profiletoken_, ui_.comboaudiosource->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddAudioSourceConfigurationResponse& addaudiosourceconfigurationresponse)
      {
        *audiosourceconfigurationmessage = addaudiosourceconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.combometadata->isEnabled())
  {
    if (profile_.metadataconfiguration_.is_initialized() && profile_.metadataconfiguration_->token_.is_initialized() && (ui_.combometadata->currentIndex() == 0))
    {
      ++totalcount_;
      metadataconfigurationconnection_ = onvifmedia_->RemoveMetadataConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveMetadataConfigurationResponse& removemetadataconfigurationresponse)
      {
        *metadataconfigurationmessage = removemetadataconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.metadataconfiguration_.is_initialized() && profile_.metadataconfiguration_->token_.is_initialized() && (ui_.combometadata->currentData().toString() != QString::fromStdString(*profile_.metadataconfiguration_->token_))) || ((!profile_.metadataconfiguration_.is_initialized() || !profile_.metadataconfiguration_->token_.is_initialized()) && (ui_.combometadata->currentIndex() != 0)))
    {
      ++totalcount_;
      metadataconfigurationconnection_ = onvifmedia_->AddMetadataConfigurationCallback(profiletoken_, ui_.combometadata->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddMetadataConfigurationResponse& addmetadataconfigurationresponse)
      {
        *metadataconfigurationmessage = addmetadataconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.comboptz->isEnabled())
  {
    if (profile_.ptzconfiguration_.is_initialized() && profile_.ptzconfiguration_->token_.is_initialized() && (ui_.comboptz->currentIndex() == 0))
    {
      ++totalcount_;
      ptzconfigurationconnection_ = onvifmedia_->RemovePTZConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemovePTZConfigurationResponse& removeptzconfigurationresponse)
      {
        *ptzconfigurationmessage = removeptzconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.ptzconfiguration_.is_initialized() && profile_.ptzconfiguration_->token_.is_initialized() && (ui_.comboptz->currentData().toString() != QString::fromStdString(*profile_.ptzconfiguration_->token_))) || ((!profile_.ptzconfiguration_.is_initialized() || !profile_.ptzconfiguration_->token_.is_initialized()) && (ui_.comboptz->currentIndex() != 0)))
    {
      ++totalcount_;
      ptzconfigurationconnection_ = onvifmedia_->AddPTZConfigurationCallback(profiletoken_, ui_.comboptz->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddPTZConfigurationResponse& addptzconfigurationresponse)
      {
        *ptzconfigurationmessage = addptzconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.combovideoencoder->isEnabled())
  {
    if (profile_.videoencoderconfiguration_.is_initialized() && profile_.videoencoderconfiguration_->token_.is_initialized() && (ui_.combovideoencoder->currentIndex() == 0))
    {
      ++totalcount_;
      videoencoderconfigurationconnection_ = onvifmedia_->RemoveVideoEncoderConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveVideoEncoderConfigurationResponse& removevideoencoderconfigurationresponse)
      {
        *videoencoderconfigurationmessage = removevideoencoderconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.videoencoderconfiguration_.is_initialized() && profile_.videoencoderconfiguration_->token_.is_initialized() && (ui_.combovideoencoder->currentData().toString() != QString::fromStdString(*profile_.videoencoderconfiguration_->token_))) || ((!profile_.videoencoderconfiguration_.is_initialized() || !profile_.videoencoderconfiguration_->token_.is_initialized()) && (ui_.combovideoencoder->currentIndex() != 0)))
    {
      ++totalcount_;
      videoencoderconfigurationconnection_ = onvifmedia_->AddVideoEncoderConfigurationCallback(profiletoken_, ui_.combovideoencoder->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddVideoEncoderConfigurationResponse& addvideoencoderconfigurationresponse)
      {
        *videoencoderconfigurationmessage = addvideoencoderconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.combovideoanalytics->isEnabled())
  {
    if (profile_.videoanalyticsconfiguration_.is_initialized() && profile_.videoanalyticsconfiguration_->token_.is_initialized() && (ui_.combovideoanalytics->currentIndex() == 0))
    {
      ++totalcount_;
      videoanalyticsconfigurationconnection_ = onvifmedia_->RemoveVideoAnalyticsConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveVideoAnalyticsConfigurationResponse& removevideoanalyticsconfigurationresponse)
      {
        *videoanalyticsconfigurationmessage = removevideoanalyticsconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.videoanalyticsconfiguration_.is_initialized() && profile_.videoanalyticsconfiguration_->token_.is_initialized() && (ui_.combovideoanalytics->currentData().toString() != QString::fromStdString(*profile_.videoanalyticsconfiguration_->token_))) || ((!profile_.videoanalyticsconfiguration_.is_initialized() || !profile_.videoanalyticsconfiguration_->token_.is_initialized()) && (ui_.combovideoanalytics->currentIndex() != 0)))
    {
      ++totalcount_;
      videoanalyticsconfigurationconnection_ = onvifmedia_->AddVideoAnalyticsConfigurationCallback(profiletoken_, ui_.combovideoanalytics->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddVideoAnalyticsConfigurationResponse& addvideoanalyticsconfigurationresponse)
      {
        *videoanalyticsconfigurationmessage = addvideoanalyticsconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (ui_.combovideosource->isEnabled())
  {
    if (profile_.videosourceconfiguration_.is_initialized() && profile_.videosourceconfiguration_->token_.is_initialized() && (ui_.combovideosource->currentIndex() == 0))
    {
      ++totalcount_;
      videosourceconfigurationconnection_ = onvifmedia_->RemoveVideoSourceConfigurationCallback(profiletoken_, [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::RemoveVideoSourceConfigurationResponse& removevideosourceconfigurationresponse)
      {
        *videosourceconfigurationmessage = removevideosourceconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
    else if ((profile_.videosourceconfiguration_.is_initialized() && profile_.videosourceconfiguration_->token_.is_initialized() && (ui_.combovideosource->currentData().toString() != QString::fromStdString(*profile_.videosourceconfiguration_->token_))) || ((!profile_.videosourceconfiguration_.is_initialized() || !profile_.videosourceconfiguration_->token_.is_initialized()) && (ui_.combovideosource->currentIndex() != 0)))
    {
      ++totalcount_;
      videosourceconfigurationconnection_ = onvifmedia_->AddVideoSourceConfigurationCallback(profiletoken_, ui_.combovideosource->currentData().toString().toStdString(), [this, count, videosourceconfigurationmessage, videoencoderconfigurationmessage, audiosourceconfigurationmessage, audioencoderconfigurationmessage, videoanalyticsconfigurationmessage, ptzconfigurationmessage, metadataconfigurationmessage, audiooutputconfigurationmessage, audiodecoderconfigurationmessage](const onvif::media::AddVideoSourceConfigurationResponse& addvideosourceconfigurationresponse)
      {
        *videosourceconfigurationmessage = addvideosourceconfigurationresponse.Message();

        ++(*count);
        SetProgress(*count);
        if (*count == totalcount_)
        {
          Accept(*audiodecoderconfigurationmessage, *audioencoderconfigurationmessage, *audiooutputconfigurationmessage, *audiosourceconfigurationmessage, *metadataconfigurationmessage, *ptzconfigurationmessage, *videoencoderconfigurationmessage, *videoanalyticsconfigurationmessage, *videosourceconfigurationmessage);

        }
      });
    }
  }

  if (totalcount_ == 0)
  {
    accept();

  }
}

}
