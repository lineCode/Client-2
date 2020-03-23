// manageonvifmediaaudioencoderconfigurationwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaaudioencoderconfigurationwindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <QString>
#include <vector>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaAudioEncoderConfigurationWindow::ManageONVIFMediaAudioEncoderConfigurationWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(2);
  getaudioencoderconfigurationconnection_ = onvifmedia_->GetAudioEncoderConfigurationCallback(token, [this, count](const onvif::media::GetAudioEncoderConfigurationResponse& getaudioencoderconfigurationresponse)
  {
    if (getaudioencoderconfigurationresponse.Error() || !getaudioencoderconfigurationresponse.configuration_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudioencoderconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    configuration_ = *getaudioencoderconfigurationresponse.configuration_;
    if ((--(*count)) == 0)
    {
      Init();

    }
  });

  getaudioencoderconfigurationoptionsconnection_ = onvifmedia_->GetAudioEncoderConfigurationOptionsCallback(token, boost::none, [this, count](const onvif::media::GetAudioEncoderConfigurationOptionsResponse& getaudioencoderconfigurationoptionsresponse)
  {
    if (getaudioencoderconfigurationoptionsresponse.Error() || !getaudioencoderconfigurationoptionsresponse.options_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudioencoderconfigurationoptionsresponse.Message()), QMessageBox::Ok);
      return;
    }

    options_ = getaudioencoderconfigurationoptionsresponse.options_->options_;
    if ((--(*count)) == 0)
    {
      Init();

    }
  });
}

ManageONVIFMediaAudioEncoderConfigurationWindow::~ManageONVIFMediaAudioEncoderConfigurationWindow()
{
  if (setaudioencoderconfigurationconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaAudioEncoderConfigurationWindow::SetEnabled(bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.combobitrate->setEnabled(enabled);
  ui_.combosamplerate->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageONVIFMediaAudioEncoderConfigurationWindow::Init()
{
  SetEnabled(true);

  // Details
  if (configuration_.name_.is_initialized())
  {
    ui_.editname->setText(QString::fromStdString(*configuration_.name_));

  }

  if (configuration_.token_.is_initialized())
  {
    ui_.edittoken->setText(QString::fromStdString(*configuration_.token_));

  }

  if (configuration_.usecount_.is_initialized())
  {
    ui_.editusecount->setText(QString::number(*configuration_.usecount_));

  }

  // Multicast
  if (devicecapabilities_.SupportsRTPMulticast())
  {
    // Fill in the dialog as best as we can
    if (configuration_.multicast_.is_initialized())
    {
      if (configuration_.multicast_->address_.is_initialized() && configuration_.multicast_->address_->type_.is_initialized())
      {
        if ((*configuration_.multicast_->address_->type_ == onvif::IPTYPE_IPV4) && configuration_.multicast_->address_->ipv4address_.is_initialized())
        {
          ui_.editaddress->setText(QString::fromStdString(*configuration_.multicast_->address_->ipv4address_));

        }
        else if ((*configuration_.multicast_->address_->type_ == onvif::IPTYPE_IPV6) && configuration_.multicast_->address_->ipv6address_.is_initialized())
        {
          ui_.editaddress->setText(QString::fromStdString(*configuration_.multicast_->address_->ipv6address_));

        }
      }

      if (configuration_.multicast_->port_.is_initialized())
      {
        ui_.editport->setText(QString::number(*configuration_.multicast_->port_));

      }

      if (configuration_.multicast_->ttl_.is_initialized())
      {
        ui_.spinttl->setValue(*configuration_.multicast_->ttl_);


      }

      if (configuration_.multicast_->autostart_.is_initialized())
      {
        ui_.checkautostart->setChecked(*configuration_.multicast_->autostart_);

      }
    }

    // Enable all multicast controls
    ui_.comboaddresstype->setEnabled(true);
    ui_.editaddress->setEnabled(true);
    ui_.editport->setEnabled(true);
    ui_.spinttl->setEnabled(true);
    ui_.checkautostart->setEnabled(true);
  }


  // Configuration
  for (const auto& option : options_)
  {
    if (!option.encoding_.is_initialized())
    {

      continue;
    }

    switch (*option.encoding_)
    {
      case onvif::AUDIOENCODING_G711:
      {
        ui_.radiog711->setEnabled(true);
        break;
      }
      case onvif::AUDIOENCODING_G726:
      {
        ui_.radiog726->setEnabled(true);
        break;
      }
      case onvif::AUDIOENCODING_AAC:
      {
        ui_.radioaac->setEnabled(true);
        break;
      }
    }
  }

  if (configuration_.encoding_.is_initialized())
  {
    SelectEncoding(*configuration_.encoding_);

  }
}

void ManageONVIFMediaAudioEncoderConfigurationWindow::SelectEncoding(onvif::AUDIOENCODING encoding)
{
  ui_.combobitrate->clear();
  ui_.combosamplerate->clear();

  for (const auto& option : options_)
  {
    if (!option.encoding_.is_initialized() || (*option.encoding_ != encoding) || !option.bitratelist_.is_initialized() || !option.sampleratelist_.is_initialized())
    {

      continue;
    }

    for (int bitrate : option.bitratelist_->items_)
    {
      ui_.combobitrate->addItem(QString::number(bitrate));

    }

    for (int samplerate : option.sampleratelist_->items_)
    {
      ui_.combosamplerate->addItem(QString::number(samplerate));

    }
  }

  // If we selected the current settings, may as well highlight the current settings
  if (configuration_.encoding_.is_initialized() && (*configuration_.encoding_ == encoding) && configuration_.bitrate_.is_initialized() && configuration_.samplerate_.is_initialized())
  {
    ui_.radiog711->setChecked(*configuration_.encoding_ == onvif::AUDIOENCODING_G711);
    ui_.radiog726->setChecked(*configuration_.encoding_ == onvif::AUDIOENCODING_G726);
    ui_.radioaac->setChecked(*configuration_.encoding_ == onvif::AUDIOENCODING_AAC);

    ui_.combobitrate->setCurrentText(QString::number(*configuration_.bitrate_));
    ui_.combosamplerate->setCurrentText(QString::number(*configuration_.samplerate_));
  }
}

void ManageONVIFMediaAudioEncoderConfigurationWindow::on_radiog711_clicked()
{
  SelectEncoding(onvif::AUDIOENCODING_G711);

}

void ManageONVIFMediaAudioEncoderConfigurationWindow::on_radiog726_clicked()
{
  SelectEncoding(onvif::AUDIOENCODING_G726);

}

void ManageONVIFMediaAudioEncoderConfigurationWindow::on_radioaac_clicked()
{
  SelectEncoding(onvif::AUDIOENCODING_AAC);

}

void ManageONVIFMediaAudioEncoderConfigurationWindow::on_buttonok_clicked()
{
  // Encoding
  onvif::AUDIOENCODING encoding;
  if (ui_.radiog711->isChecked())
  {
    encoding = onvif::AUDIOENCODING_G711;

  }
  else if (ui_.radiog726->isChecked())
  {
    encoding = onvif::AUDIOENCODING_G726;

  }
  else if (ui_.radioaac->isChecked())
  {
    encoding = onvif::AUDIOENCODING_AAC;

  }
  else
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Invalid encoding"), QMessageBox::Ok).exec();
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
  else if (iptype == onvif::IPTYPE_IPV6)
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
    QMessageBox::warning(this, tr("Error"), tr("Invalid address type"), QMessageBox::Ok);
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  SetEnabled(false);
  ui_.radiog711->setEnabled(false);
  ui_.radiog726->setEnabled(false);
  ui_.radioaac->setEnabled(false);

  const onvif::AudioEncoderConfiguration configuration(ui_.editname->text().toStdString(), ui_.editusecount->text().toInt(), ui_.edittoken->text().toStdString(), encoding, ui_.combobitrate->currentText().toInt(), ui_.combosamplerate->currentText().toInt(), multicastconfiguration, onvif::Duration(0, 0, 0, 0, 0, 60));
  setaudioencoderconfigurationconnection_ = onvifmedia_->SetAudioEncoderConfigurationCallback(configuration, [this](const onvif::media::SetAudioEncoderConfigurationResponse& setaudioencoderconfigurationresponse)
  {
    QApplication::restoreOverrideCursor();
    SetEnabled(true);
    ui_.radiog711->setEnabled(true);
    ui_.radiog726->setEnabled(true);
    ui_.radioaac->setEnabled(true);

    if (setaudioencoderconfigurationresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setaudioencoderconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    name_ = ui_.editname->text();

    accept();
  });
}

}

QT_END_NAMESPACE
