// manageonvifmediavideoencoderconfigurationwindow.cpp
//

///// Defines /////

#define NOMINMAX

///// Includes /////

#include "monocleclient/manageonvifmediavideoencoderconfigurationwindow.h"

#include <boost/asio.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ManageONVIFMediaVideoEncoderConfigurationWindow::ManageONVIFMediaVideoEncoderConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token) :
  QDialog(parent),
  token_(token),
  devicecapabilities_(devicecapabilities),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);

  connect(ui_.combocodec, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), this, &ManageONVIFMediaVideoEncoderConfigurationWindow::on_combocodec_currentTextChanged);
  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editport->setValidator(new QIntValidator(1, 65535, this));

  ui_.edittoken->setText(QString::fromStdString(token));
  
  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(2);
  std::shared_ptr<onvif::VideoEncoderConfiguration> videoencoderconfiguration = std::make_shared<onvif::VideoEncoderConfiguration>();
  getvideoencoderconfigurationconnection_ = onvifmedia_->GetVideoEncoderConfigurationCallback(token, [this, count, videoencoderconfiguration](const onvif::media::GetVideoEncoderConfigurationResponse& getvideoencoderconfigurationresponse)
  {
    if (getvideoencoderconfigurationresponse.Error() || !getvideoencoderconfigurationresponse.videoencoderconfiguration_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getvideoencoderconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    *videoencoderconfiguration = *getvideoencoderconfigurationresponse.videoencoderconfiguration_;

    if ((--(*count)) == 0)
    {
      InitDialog(*videoencoderconfiguration);

    }
  });
  
  getvideoencoderconfigurationoptionsconnection_ = onvifmedia_->GetVideoEncoderConfigurationOptionsCallback(token_, boost::none, [this, count, videoencoderconfiguration](const onvif::media::GetVideoEncoderConfigurationOptionsResponse& getvideoencoderconfigurationoptionsresponse)
  {
    if (getvideoencoderconfigurationoptionsresponse.Error() || !getvideoencoderconfigurationoptionsresponse.videoencoderconfigurationoptions_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getvideoencoderconfigurationoptionsresponse.Message()), QMessageBox::Ok);
      return;
    }

    videoencoderconfigurationoptions_ = *getvideoencoderconfigurationoptionsresponse.videoencoderconfigurationoptions_;

    if ((--(*count)) == 0)
    {
      InitDialog(*videoencoderconfiguration);

    }
  });
}

ManageONVIFMediaVideoEncoderConfigurationWindow::~ManageONVIFMediaVideoEncoderConfigurationWindow()
{
  if (setvideoencoderconfigurationconnection_.IsConnected() || getvideoencoderconfigurationoptionsconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaVideoEncoderConfigurationWindow::InitDialog(const onvif::VideoEncoderConfiguration& videoencoderconfiguration)
{
  // Error check
  if (!videoencoderconfigurationoptions_.jpeg_.is_initialized() && !videoencoderconfigurationoptions_.mpeg4_.is_initialized() && !videoencoderconfigurationoptions_.h264_.is_initialized())
  {
    QMessageBox::warning(this, tr("Error"), tr("No valid video encoders"), QMessageBox::Ok);
    return;
  }
  
  ui_.combocodec->setEnabled(true);
  if (videoencoderconfigurationoptions_.jpeg_.is_initialized())
  {
    ui_.combocodec->addItem(QString::fromStdString(onvif::ToString(onvif::VIDEOENCODING_JPEG)));

  }

  if (videoencoderconfigurationoptions_.mpeg4_.is_initialized())
  {
    ui_.combocodec->addItem(QString::fromStdString(onvif::ToString(onvif::VIDEOENCODING_MPEG4)));

  }

  if (videoencoderconfigurationoptions_.h264_.is_initialized())
  {
    ui_.combocodec->addItem(QString::fromStdString(onvif::ToString(onvif::VIDEOENCODING_H264)));

  }

  // Details
  ui_.editname->setEnabled(true);

  if (videoencoderconfiguration.name_.is_initialized())
  {
    setWindowTitle(windowTitle() + QString(": ") + QString::fromStdString(*videoencoderconfiguration.name_));
    ui_.editname->setText(QString::fromStdString(*videoencoderconfiguration.name_));
  }

  if (videoencoderconfiguration.usecount_.is_initialized())
  {
    ui_.editusecount->setText(QString::number(*videoencoderconfiguration.usecount_));
    
  }
  
  ui_.comboaddresstype->setCurrentIndex(ui_.comboaddresstype->findText(QString::fromStdString(onvif::ToString(*videoencoderconfiguration.multicast_->address_->type_))));

  // Multicast
  if (devicecapabilities_.SupportsRTPMulticast())
  {
    // Fill in the dialog as best as we can
    if (videoencoderconfiguration.multicast_.is_initialized())
    {
      if (videoencoderconfiguration.multicast_->address_.is_initialized() && videoencoderconfiguration.multicast_->address_->type_.is_initialized())
      {
        if ((*videoencoderconfiguration.multicast_->address_->type_ == onvif::IPTYPE_IPV4) && videoencoderconfiguration.multicast_->address_->ipv4address_.is_initialized())
        {
          ui_.editaddress->setText(QString::fromStdString(*videoencoderconfiguration.multicast_->address_->ipv4address_));

        }
        else if ((*videoencoderconfiguration.multicast_->address_->type_ == onvif::IPTYPE_IPV6) && videoencoderconfiguration.multicast_->address_->ipv6address_.is_initialized())
        {
          ui_.editaddress->setText(QString::fromStdString(*videoencoderconfiguration.multicast_->address_->ipv6address_));

        }
      }

      if (videoencoderconfiguration.multicast_->port_.is_initialized())
      {
        ui_.editport->setText(QString::number(*videoencoderconfiguration.multicast_->port_));

      }

      if (videoencoderconfiguration.multicast_->ttl_.is_initialized())
      {
        ui_.spinttl->setValue(*videoencoderconfiguration.multicast_->ttl_);


      }
    
      if (videoencoderconfiguration.multicast_->autostart_.is_initialized())
      {
        ui_.checkautostart->setChecked(*videoencoderconfiguration.multicast_->autostart_);

      }
    }
    
    // Enable all multicast controls
    ui_.comboaddresstype->setEnabled(true);
    ui_.editaddress->setEnabled(true);
    ui_.editport->setEnabled(true);
    ui_.spinttl->setEnabled(true);
    ui_.checkautostart->setEnabled(true);
  }
  
  // Quality
  if (videoencoderconfigurationoptions_.qualityrange_.is_initialized())
  {
    ui_.sliderquality->setEnabled(true);
    if (videoencoderconfigurationoptions_.qualityrange_->min_.is_initialized() && videoencoderconfigurationoptions_.qualityrange_->max_.is_initialized())
    {
      ui_.sliderquality->setRange(*videoencoderconfigurationoptions_.qualityrange_->min_, *videoencoderconfigurationoptions_.qualityrange_->max_);

    }
    else
    {
      ui_.labelquality->setVisible(false);
      ui_.sliderquality->setVisible(false);
      ui_.labelqualityvalue->setVisible(false);
    }
  }

  // Encoder
  if (videoencoderconfiguration.encoding_.is_initialized())
  {
    if ((*videoencoderconfiguration.encoding_ == onvif::VIDEOENCODING_JPEG) && (videoencoderconfigurationoptions_.jpeg_.is_initialized()))
    {
      InitJpeg();
      
      // Select settings
      ui_.combocodec->setCurrentIndex(ui_.combocodec->findText(QString::fromStdString(onvif::ToString(onvif::VIDEOENCODING_JPEG))));
    }
    else if ((*videoencoderconfiguration.encoding_ == onvif::VIDEOENCODING_MPEG4) && (videoencoderconfigurationoptions_.mpeg4_.is_initialized()))
    {
      InitMpeg4();

      // Select settings
      ui_.combocodec->setCurrentIndex(ui_.combocodec->findText(QString::fromStdString(onvif::ToString(onvif::VIDEOENCODING_MPEG4))));

      if (videoencoderconfiguration.mpeg4_->govlength_.is_initialized())
      {
        ui_.slidergovlength->setValue(static_cast<int>(*videoencoderconfiguration.mpeg4_->govlength_));
        on_slidergovlength_valueChanged(static_cast<int>(*videoencoderconfiguration.mpeg4_->govlength_));
      }
      
      if (videoencoderconfiguration.mpeg4_->mpeg4profile_.is_initialized())
      {
        ui_.comboprofile->setCurrentIndex(ui_.comboprofile->findText(QString::fromStdString(onvif::ToString(*videoencoderconfiguration.mpeg4_->mpeg4profile_))));

      }
    }
    else if ((*videoencoderconfiguration.encoding_ == onvif::VIDEOENCODING_H264) && (videoencoderconfigurationoptions_.h264_.is_initialized()))
    {
      InitH264();

      // Select settings
      ui_.combocodec->setCurrentIndex(ui_.combocodec->findText(QString::fromStdString(onvif::ToString(onvif::VIDEOENCODING_H264))));

      if (videoencoderconfiguration.h264_->govlength_.is_initialized())
      {
        ui_.slidergovlength->setValue(static_cast<int>(*videoencoderconfiguration.h264_->govlength_));
        on_slidergovlength_valueChanged(static_cast<int>(*videoencoderconfiguration.h264_->govlength_));
      }
      
      if (videoencoderconfiguration.h264_->h264profile_.is_initialized())
      {
        ui_.comboprofile->setCurrentIndex(ui_.comboprofile->findText(QString::fromStdString(onvif::ToString(*videoencoderconfiguration.h264_->h264profile_))));

      }
    }
    else
    {
      // Select the first codec to prepare the dialog anyway
      ui_.combocodec->setCurrentIndex(0);
      on_combocodec_currentTextChanged(ui_.combocodec->currentText());
      return;
    }
  }

  // Select the settings which are generic to all encoder types
  if (videoencoderconfiguration.quality_.is_initialized())
  {
    ui_.sliderquality->setValue(static_cast<int>(*videoencoderconfiguration.quality_));
    on_sliderquality_valueChanged(static_cast<int>(*videoencoderconfiguration.quality_));
  }

  if (videoencoderconfiguration.resolution_.is_initialized() && videoencoderconfiguration.resolution_->width_.is_initialized() && videoencoderconfiguration.resolution_->height_.is_initialized())
  {
    ui_.comboresolution->setCurrentIndex(ui_.comboresolution->findText(QString::number(*videoencoderconfiguration.resolution_->width_) + "x" + QString::number(*videoencoderconfiguration.resolution_->height_)));

  }

  if (videoencoderconfiguration.ratecontrol_.is_initialized())
  {
    if (videoencoderconfiguration.ratecontrol_->frameratelimit_.is_initialized())
    {
      ui_.slidermaxframerate->setValue(static_cast<int>(*videoencoderconfiguration.ratecontrol_->frameratelimit_));
      on_slidermaxframerate_valueChanged(static_cast<int>(*videoencoderconfiguration.ratecontrol_->frameratelimit_));
    }
        
    if (videoencoderconfiguration.ratecontrol_->encodinginterval_.is_initialized())
    {
      ui_.sliderencodinginterval->setValue(static_cast<int>(*videoencoderconfiguration.ratecontrol_->encodinginterval_));
      on_sliderencodinginterval_valueChanged(static_cast<int>(*videoencoderconfiguration.ratecontrol_->encodinginterval_));
    }
        
    if (videoencoderconfiguration.ratecontrol_->bitratelimit_.is_initialized())
    {
      ui_.sliderbitrate->setValue(*videoencoderconfiguration.ratecontrol_->bitratelimit_);
      on_sliderbitrate_valueChanged(static_cast<int>(*videoencoderconfiguration.ratecontrol_->bitratelimit_));
    }
  }

  ui_.buttonok->setEnabled(true);
}

void ManageONVIFMediaVideoEncoderConfigurationWindow::InitJpeg()
{
  // Resolution
  ui_.comboresolution->clear();
  for (const auto& resolution : videoencoderconfigurationoptions_.jpeg_->resolutionsavailable_)
  {
    if (resolution.width_.is_initialized() && resolution.height_.is_initialized())
    {
      ui_.comboresolution->addItem(QString::number(*resolution.width_) + "x" + QString::number(*resolution.height_), QSize(*resolution.width_, *resolution.height_));

    }
  }

  if (ui_.comboresolution->count())
  {
    ui_.comboresolution->setEnabled(true);

  }
  else

  {
    ui_.comboresolution->setEnabled(false);
  }

  // Max frame rate
  if (videoencoderconfigurationoptions_.jpeg_->frameraterange_.is_initialized() && videoencoderconfigurationoptions_.jpeg_->frameraterange_->min_.is_initialized() && videoencoderconfigurationoptions_.jpeg_->frameraterange_->max_.is_initialized())
  {
    ui_.slidermaxframerate->setEnabled(true);
    ui_.slidermaxframerate->setRange(*videoencoderconfigurationoptions_.jpeg_->frameraterange_->min_, *videoencoderconfigurationoptions_.jpeg_->frameraterange_->max_);
  }
  else
  {
    ui_.slidermaxframerate->setEnabled(false);

  }

  // Encoding interval
  if (videoencoderconfigurationoptions_.jpeg_->encodingintervalrange_.is_initialized() && videoencoderconfigurationoptions_.jpeg_->encodingintervalrange_->min_.is_initialized() && videoencoderconfigurationoptions_.jpeg_->encodingintervalrange_->max_.is_initialized())
  {
    ui_.sliderencodinginterval->setEnabled(true);
    ui_.sliderencodinginterval->setRange(*videoencoderconfigurationoptions_.jpeg_->encodingintervalrange_->min_, *videoencoderconfigurationoptions_.jpeg_->encodingintervalrange_->max_);
  }
  else
  {
    ui_.sliderencodinginterval->setEnabled(false);

  }

  // Bitrate
  if (videoencoderconfigurationoptions_.extensionjpeg_.is_initialized() && videoencoderconfigurationoptions_.extensionjpeg_->bitraterange_.is_initialized() && videoencoderconfigurationoptions_.extensionjpeg_->bitraterange_->min_.is_initialized() && videoencoderconfigurationoptions_.extensionjpeg_->bitraterange_->max_.is_initialized())
  {
    ui_.sliderbitrate->setEnabled(true);
    ui_.sliderbitrate->setRange(*videoencoderconfigurationoptions_.extensionjpeg_->bitraterange_->min_, *videoencoderconfigurationoptions_.extensionjpeg_->bitraterange_->max_);
  }
  else
  {
    ui_.sliderbitrate->setEnabled(false);

  }

  // Gov Length
  ui_.slidergovlength->setEnabled(false);

  // Profile
  ui_.comboprofile->setEnabled(false);
}

void ManageONVIFMediaVideoEncoderConfigurationWindow::InitMpeg4()
{
  // Resolution
  ui_.comboresolution->clear();
  for (const auto& resolution : videoencoderconfigurationoptions_.mpeg4_->resolutionsavailable_)
  {
    if (resolution.width_.is_initialized() && resolution.height_.is_initialized())
    {
      ui_.comboresolution->addItem(QString::number(*resolution.width_) + "x" + QString::number(*resolution.height_), QSize(*resolution.width_, *resolution.height_));

    }
  }

  if (ui_.comboresolution->count())
  {
    ui_.comboresolution->setEnabled(true);
  }
  else
  {
    ui_.comboresolution->setEnabled(false);
  }

  // Max frame rate
  if (videoencoderconfigurationoptions_.mpeg4_->frameraterange_.is_initialized() && videoencoderconfigurationoptions_.mpeg4_->frameraterange_->min_.is_initialized() && videoencoderconfigurationoptions_.mpeg4_->frameraterange_->max_.is_initialized())
  {
    ui_.slidermaxframerate->setEnabled(true);
    ui_.slidermaxframerate->setRange(*videoencoderconfigurationoptions_.mpeg4_->frameraterange_->min_, *videoencoderconfigurationoptions_.mpeg4_->frameraterange_->max_);
  }
  else
  {
    ui_.slidermaxframerate->setEnabled(false);
  }

  // Encoding interval
  if (videoencoderconfigurationoptions_.mpeg4_->encodingintervalrange_.is_initialized() && videoencoderconfigurationoptions_.mpeg4_->encodingintervalrange_->min_.is_initialized() && videoencoderconfigurationoptions_.mpeg4_->encodingintervalrange_->max_.is_initialized())
  {
    ui_.sliderencodinginterval->setEnabled(true);
    ui_.sliderencodinginterval->setRange(*videoencoderconfigurationoptions_.mpeg4_->encodingintervalrange_->min_, *videoencoderconfigurationoptions_.mpeg4_->encodingintervalrange_->max_);
  }
  else
  {
    ui_.sliderencodinginterval->setEnabled(false);

  }

  // Bitrate
  if (videoencoderconfigurationoptions_.extensionmpeg4_.is_initialized() && videoencoderconfigurationoptions_.extensionmpeg4_->bitraterange_.is_initialized() && videoencoderconfigurationoptions_.extensionmpeg4_->bitraterange_->min_.is_initialized() && videoencoderconfigurationoptions_.extensionmpeg4_->bitraterange_->max_.is_initialized())
  {
    ui_.sliderbitrate->setEnabled(true);
    ui_.sliderbitrate->setRange(*videoencoderconfigurationoptions_.extensionmpeg4_->bitraterange_->min_, *videoencoderconfigurationoptions_.extensionmpeg4_->bitraterange_->max_);
  }
  else
  {
    ui_.sliderbitrate->setEnabled(false);

  }

  // Gov Length
  if (videoencoderconfigurationoptions_.mpeg4_->govlengthrange_.is_initialized() && videoencoderconfigurationoptions_.mpeg4_->govlengthrange_->min_.is_initialized() && videoencoderconfigurationoptions_.mpeg4_->govlengthrange_->max_.is_initialized())
  {
    ui_.slidergovlength->setEnabled(true);
    ui_.slidergovlength->setRange(*videoencoderconfigurationoptions_.mpeg4_->govlengthrange_->min_, *videoencoderconfigurationoptions_.mpeg4_->govlengthrange_->max_);
  }
  else
  {
    ui_.slidergovlength->setEnabled(false);

  }

  // Profile
  ui_.comboprofile->clear();
  for (const auto profile : videoencoderconfigurationoptions_.mpeg4_->mpeg4profilessupported_)
  {
    ui_.comboprofile->addItem(QString::fromStdString(onvif::ToString(profile)));

  }

  if (ui_.comboprofile->count())
  {
    ui_.comboprofile->setEnabled(true);

  }
  else
  {
    ui_.comboprofile->setEnabled(false);

  }
}

void ManageONVIFMediaVideoEncoderConfigurationWindow::InitH264()
{
  // Resolution
  ui_.comboresolution->clear();
  for (const auto& resolution : videoencoderconfigurationoptions_.h264_->resolutionsavailable_)
  {
    if (resolution.width_.is_initialized() && resolution.height_.is_initialized())
    {
      ui_.comboresolution->addItem(QString::number(*resolution.width_) + "x" + QString::number(*resolution.height_), QSize(*resolution.width_, *resolution.height_));

    }
  }

  if (ui_.comboresolution->count())
  {
    ui_.comboresolution->setEnabled(true);

  }
  else
  {
    ui_.comboresolution->setEnabled(false);

  }

  // Max frame rate
  if (videoencoderconfigurationoptions_.h264_->frameraterange_.is_initialized() && videoencoderconfigurationoptions_.h264_->frameraterange_->min_.is_initialized() && videoencoderconfigurationoptions_.h264_->frameraterange_->max_.is_initialized())
  {
    ui_.slidermaxframerate->setEnabled(true);
    ui_.slidermaxframerate->setRange(*videoencoderconfigurationoptions_.h264_->frameraterange_->min_, *videoencoderconfigurationoptions_.h264_->frameraterange_->max_);
  }
  else
  {
    ui_.slidermaxframerate->setEnabled(false);

  }

  // Encoding interval
  if (videoencoderconfigurationoptions_.h264_->encodingintervalrange_.is_initialized() && videoencoderconfigurationoptions_.h264_->encodingintervalrange_->min_.is_initialized() && videoencoderconfigurationoptions_.h264_->encodingintervalrange_->max_.is_initialized())
  {
    ui_.sliderencodinginterval->setEnabled(true);
    ui_.sliderencodinginterval->setRange(*videoencoderconfigurationoptions_.h264_->encodingintervalrange_->min_, *videoencoderconfigurationoptions_.h264_->encodingintervalrange_->max_);
  }
  else
  {
    ui_.sliderencodinginterval->setEnabled(false);
  }

  // Bitrate
  if (videoencoderconfigurationoptions_.extensionh264_.is_initialized() && videoencoderconfigurationoptions_.extensionh264_->bitraterange_.is_initialized() && videoencoderconfigurationoptions_.extensionh264_->bitraterange_->min_.is_initialized() && videoencoderconfigurationoptions_.extensionh264_->bitraterange_->max_.is_initialized())
  {
    ui_.sliderbitrate->setEnabled(true);
    ui_.sliderbitrate->setRange(*videoencoderconfigurationoptions_.extensionh264_->bitraterange_->min_, *videoencoderconfigurationoptions_.extensionh264_->bitraterange_->max_);
  }
  else
  {
    ui_.sliderbitrate->setEnabled(false);
  }

  // Gov Length
  if (videoencoderconfigurationoptions_.h264_->govlengthrange_.is_initialized() && videoencoderconfigurationoptions_.h264_->govlengthrange_->min_.is_initialized() && videoencoderconfigurationoptions_.h264_->govlengthrange_->max_.is_initialized())
  {
    ui_.slidergovlength->setEnabled(true);
    ui_.slidergovlength->setRange(*videoencoderconfigurationoptions_.h264_->govlengthrange_->min_, *videoencoderconfigurationoptions_.h264_->govlengthrange_->max_);
  }
  else
  {
    ui_.slidergovlength->setEnabled(false);
  }

  // Profile
  ui_.comboprofile->clear();
  for (const auto profile : videoencoderconfigurationoptions_.h264_->h264profilessupported_)
  {
    ui_.comboprofile->addItem(QString::fromStdString(onvif::ToString(profile)));

  }

  if (ui_.comboprofile->count())
  {
    ui_.comboprofile->setEnabled(true);

  }
  else
  {
    ui_.comboprofile->setEnabled(false);

  }
}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_sliderquality_valueChanged(int value)
{
  ui_.labelqualityvalue->setText(QString::number(value));

}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_slidermaxframerate_valueChanged(int value)
{
  ui_.labelmaxframeratevalue->setText(QString::number(value));

}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_sliderencodinginterval_valueChanged(int value)
{
  ui_.labelencodingintervalvalue->setText(QString::number(value));

}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_sliderbitrate_valueChanged(int value)
{
  ui_.labelbitratevalue->setText(QString::number(value));

}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_slidergovlength_valueChanged(int value)
{
  ui_.labelgovlengthvalue->setText(QString::number(value));

}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_combocodec_currentTextChanged(const QString& codec)
{
  switch (*onvif::GetVideoEncoding(codec.toStdString()))
  {
    case onvif::VIDEOENCODING_JPEG:
    {
      InitJpeg();
      break;
    }
    case onvif::VIDEOENCODING_MPEG4:
    {
      InitMpeg4();
      break;
    }
    case onvif::VIDEOENCODING_H264:
    {
      InitH264();
      break;
    }
    default:
    {

      break;
    }
  }
}

void ManageONVIFMediaVideoEncoderConfigurationWindow::on_buttonok_clicked()
{
  // Retrieve the settings from the dialog making sure they are valid
  if (ui_.editname->text().size() <= 0)
  {
    QMessageBox::warning(this, tr("Error"), tr("Invalid name"), QMessageBox::Ok);
    return;
  }

  // Multicast
  onvif::MulticastConfiguration multicastconfiguration;
  onvif::IPTYPE iptype = *onvif::GetIpType(ui_.comboaddresstype->currentText().toStdString());
  if (iptype == onvif::IPTYPE_IPV4)
  {
    if (!boost::asio::ip::address_v4::from_string(ui_.editaddress->text().toStdString()).is_multicast() && (ui_.editaddress->text() != QString("0.0.0.0")))
    {
      QMessageBox::warning(this, tr("Error"), tr("Please enter a multicast address"), QMessageBox::Ok);
      return;
    }

    multicastconfiguration = onvif::MulticastConfiguration(onvif::IPAddress(iptype, ui_.editaddress->text().toStdString(), boost::none), ui_.editport->text().toInt(), ui_.spinttl->value(), ui_.checkautostart->isChecked());
  }
  else if (iptype == onvif::IPTYPE_IPV4)
  {
    if (!boost::asio::ip::address_v6::from_string(ui_.editaddress->text().toStdString()).is_multicast() && (ui_.editaddress->text() != QString("::/0")))
    {
      QMessageBox::warning(this, tr("Error"), tr("Please enter a multicast address"), QMessageBox::Ok);
      return;
    }

    multicastconfiguration = onvif::MulticastConfiguration(onvif::IPAddress(iptype, boost::none, ui_.editaddress->text().toStdString()), ui_.editport->text().toInt(), ui_.spinttl->value(), ui_.checkautostart->isChecked());
  }
  else
  {
    QMessageBox::warning(this, tr("Error"), tr("Invalid type"), QMessageBox::Ok);
    return;
  }

  // Encoding
  const onvif::VIDEOENCODING encoding = *onvif::GetVideoEncoding(ui_.combocodec->currentText().toStdString());
  const QSize resolution = ui_.comboresolution->currentData().toSize();
  boost::optional<onvif::Mpeg4Configuration> mpeg4configuration;
  boost::optional<onvif::H264Configuration> h264configuration;
  if (encoding == onvif::VIDEOENCODING_JPEG)
  {
    // No jpeg related settings required
    
  }
  else if (encoding == onvif::VIDEOENCODING_MPEG4)
  {
    mpeg4configuration = onvif::Mpeg4Configuration(ui_.slidergovlength->value(), onvif::GetMpeg4Profile(ui_.comboprofile->currentText().toStdString()));

  }
  else if (encoding == onvif::VIDEOENCODING_H264)
  {
    h264configuration = onvif::H264Configuration(ui_.slidergovlength->value(), onvif::GetH264Profile(ui_.comboprofile->currentText().toStdString()));
    
  }
  else
  {
    QMessageBox::warning(this, tr("Error"), tr("Invalid encoding"), QMessageBox::Ok);
    return;
  }

  // Set this video encoder configuration
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui_.buttonok->setEnabled(false);

  const onvif::VideoEncoderConfiguration videoencoderconfiguration(ui_.editname->text().toStdString(),
                                                                   0,
                                                                   token_,
                                                                   encoding,
                                                                   onvif::VideoResolution(resolution.width(), resolution.height()),
                                                                   ui_.sliderquality->value(),
                                                                   onvif::VideoRateControl(ui_.slidermaxframerate->value(), ui_.sliderencodinginterval->value(), ui_.sliderbitrate->value()),
                                                                   mpeg4configuration,
                                                                   h264configuration,
                                                                   multicastconfiguration,
                                                                   onvif::Duration(0, 0, 0, 0, 0, 60));

  setvideoencoderconfigurationconnection_ = onvifmedia_->SetVideoEncoderConfigurationCallback(videoencoderconfiguration, [this](const onvif::media::SetVideoEncoderConfigurationResponse& setvideoencoderconfigurationresponse)
  {
    QApplication::restoreOverrideCursor();
    ui_.buttonok->setEnabled(true);

    if (setvideoencoderconfigurationresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setvideoencoderconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    name_ = ui_.editname->text();

    accept();
  });
}

}
