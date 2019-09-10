// manageonvifmediaaudiooutputconfigurationwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaaudiooutputconfigurationwindow.h"

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

ManageONVIFMediaAudioOutputConfigurationWindow::ManageONVIFMediaAudioOutputConfigurationWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token) :
  QDialog(parent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.editoutputlevel->setValidator(new QIntValidator(0, 999999, this));

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(2);
  std::shared_ptr<onvif::AudioOutputConfiguration> configuration = std::make_shared<onvif::AudioOutputConfiguration>();
  std::shared_ptr<onvif::AudioOutputConfigurationOptions> configurationoptions = std::make_shared<onvif::AudioOutputConfigurationOptions>();
  getaudiooutputconfigurationconnection_ = onvifmedia_->GetAudioOutputConfigurationCallback(token, [this, count, configuration, configurationoptions](const onvif::media::GetAudioOutputConfigurationResponse& getaudiooutputconfigurationresponse)
  {
    if (getaudiooutputconfigurationresponse.Error() || !getaudiooutputconfigurationresponse.configuration_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudiooutputconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    *configuration = *getaudiooutputconfigurationresponse.configuration_;

    if ((--(*count)) == 0)
    {
      Init(*configuration, *configurationoptions);

    }
  });

  getaudiooutputconfigurationoptionsconnection_ = onvifmedia_->GetAudioOutputConfigurationOptionsCallback(token, boost::none, [this, count, configuration, configurationoptions](const onvif::media::GetAudioOutputConfigurationOptionsResponse& getaudiooutputconfigurationoptionsresponse)
  {
    if (getaudiooutputconfigurationoptionsresponse.Error() || !getaudiooutputconfigurationoptionsresponse.options_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudiooutputconfigurationoptionsresponse.Message()), QMessageBox::Ok);
      return;
    }

    *configurationoptions = *getaudiooutputconfigurationoptionsresponse.options_;

    if ((--(*count)) == 0)
    {
      Init(*configuration, *configurationoptions);

    }
  });
}

ManageONVIFMediaAudioOutputConfigurationWindow::~ManageONVIFMediaAudioOutputConfigurationWindow()
{
  if (setaudiooutputconfigurationconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaAudioOutputConfigurationWindow::SetEnabled(bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.combooutputtoken->setEnabled(enabled);
  ui_.combosendprimacy->setEnabled(enabled);
  ui_.editoutputlevel->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageONVIFMediaAudioOutputConfigurationWindow::Init(const onvif::AudioOutputConfiguration& configuration, onvif::AudioOutputConfigurationOptions& configurationoptions)
{
  SetEnabled(true);

  // Details
  if (configuration.name_.is_initialized())
  {
    ui_.editname->setText(QString::fromStdString(*configuration.name_));

  }

  if (configuration.token_.is_initialized())
  {
    ui_.edittoken->setText(QString::fromStdString(*configuration.token_));

  }

  if (configuration.usecount_.is_initialized())
  {
    ui_.editusecount->setText(QString::number(*configuration.usecount_));

  }

  // Configuration
  for (const auto& outputtoken : configurationoptions.outputtokensavailable_)
  {
    ui_.combooutputtoken->addItem(QString::fromStdString(outputtoken));

  }

  if (configuration.outputtoken_.is_initialized())
  {
    // Select the current token(if it exists)
    const int item = ui_.combooutputtoken->findText(QString::fromStdString(*configuration.outputtoken_));
    if (item == -1)
    {
      LOG_GUI_WARNING(QString("Output token not found: ") + QString::fromStdString(*configuration.outputtoken_));

    }
    else
    {
      ui_.combooutputtoken->setCurrentIndex(item);

    }
  }

  for (const auto& sendprimacy : configurationoptions.sendprimacyoptions_)
  {
    ui_.combosendprimacy->addItem(QString::fromStdString(sendprimacy));

  }

  if (configuration.sendprimacy_.is_initialized())
  {
    // Select the current token(if it exists)
    const int item = ui_.combosendprimacy->findText(QString::fromStdString(*configuration.sendprimacy_));
    if (item == -1)
    {
      LOG_GUI_WARNING(QString("Send Primacy not found: ") + QString::fromStdString(*configuration.sendprimacy_));

    }
    else
    {
      ui_.combosendprimacy->setCurrentIndex(item);

    }
  }

  if (configurationoptions.outputlevelrange_.is_initialized() && configurationoptions.outputlevelrange_->min_.is_initialized() && configurationoptions.outputlevelrange_->max_.is_initialized())
  {
    ui_.editoutputlevel->setValidator(new QIntValidator(*configurationoptions.outputlevelrange_->min_, *configurationoptions.outputlevelrange_->max_, this));
    ui_.labeloutputlevel->setText(QString("Output Level(") + QString::number(*configurationoptions.outputlevelrange_->min_) + QString("-") + QString::number(*configurationoptions.outputlevelrange_->max_) + QString("):"));
  }

  if (configuration.outputlevel_.is_initialized())
  {
    ui_.editoutputlevel->setText(QString::number(*configuration.outputlevel_));

  }
}

void ManageONVIFMediaAudioOutputConfigurationWindow::on_buttonok_clicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SetEnabled(false);

  const onvif::AudioOutputConfiguration configuration(ui_.editname->text().toStdString(), ui_.editusecount->text().toInt(), ui_.edittoken->text().toStdString(), ui_.combooutputtoken->currentText().toStdString(), ui_.combosendprimacy->currentText().toStdString(), ui_.editoutputlevel->text().toInt());
  setaudiooutputconfigurationconnection_ = onvifmedia_->SetAudioOutputConfigurationCallback(configuration, [this](const onvif::media::SetAudioOutputConfigurationResponse& setaudiooutputconfigurationresponse)
  {
    QApplication::restoreOverrideCursor();
    SetEnabled(true);

    if (setaudiooutputconfigurationresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setaudiooutputconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    name_ = ui_.editname->text();

    accept();
  });
}

}

QT_END_NAMESPACE
