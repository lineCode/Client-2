// manageonvifmediaaudiosourceconfigurationwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaaudiosourceconfigurationwindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <vector>

#include "monocleclient/mainwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaAudioSourceConfigurationWindow::ManageONVIFMediaAudioSourceConfigurationWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const std::string& token) :
  QDialog(parent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  std::shared_ptr<unsigned int> count = std::make_shared<unsigned int>(2);
  std::shared_ptr<onvif::AudioSourceConfiguration> configuration = std::make_shared<onvif::AudioSourceConfiguration>();
  std::shared_ptr< std::vector<onvif::AudioSource> > audiosources = std::make_shared< std::vector<onvif::AudioSource> >();
  getaudiosourceconfigurationconnection_ = onvifmedia_->GetAudioSourceConfigurationCallback(token, [this, count, configuration, audiosources](const onvif::media::GetAudioSourceConfigurationResponse& getaudiosourceconfigurationresponse)
  {
    if (getaudiosourceconfigurationresponse.Error() || !getaudiosourceconfigurationresponse.configuration_.is_initialized())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudiosourceconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    *configuration = *getaudiosourceconfigurationresponse.configuration_;

    if ((--(*count)) == 0)
    {
      Init(*configuration, *audiosources);

    }
  });

  getaudiosourcesconnection_ = onvifmedia_->GetAudioSourcesCallback([this, count, configuration, audiosources](const onvif::media::GetAudioSourcesResponse& getaudiosourcesresponse)
  {
    if (getaudiosourcesresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudiosourcesresponse.Message()), QMessageBox::Ok);
      return;
    }

    *audiosources = getaudiosourcesresponse.audiosources_;

    if ((--(*count)) == 0)
    {
      Init(*configuration, *audiosources);

    }
  });
}

ManageONVIFMediaAudioSourceConfigurationWindow::~ManageONVIFMediaAudioSourceConfigurationWindow()
{
  if (setaudiosourceconfigurationconnection_.IsConnected())
  {
    QApplication::restoreOverrideCursor();

  }
}

void ManageONVIFMediaAudioSourceConfigurationWindow::SetEnabled(bool enabled)
{
  ui_.editname->setEnabled(enabled);
  ui_.comboaudiosources->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageONVIFMediaAudioSourceConfigurationWindow::Init(const onvif::AudioSourceConfiguration& audiosourceconfiguration, const std::vector<onvif::AudioSource>& audiosources)
{
  SetEnabled(true);

  if (audiosourceconfiguration.name_.is_initialized())
  {
    ui_.editname->setText(QString::fromStdString(*audiosourceconfiguration.name_));

  }

  if (audiosourceconfiguration.token_.is_initialized())
  {
    ui_.edittoken->setText(QString::fromStdString(*audiosourceconfiguration.token_));

  }

  if (audiosourceconfiguration.usecount_.is_initialized())
  {
    ui_.editusecount->setText(QString::number(*audiosourceconfiguration.usecount_));

  }

  for (const auto& audiosource : audiosources)
  {
    if (!audiosource.token_.is_initialized())
    {

      continue;
    }

    ui_.comboaudiosources->addItem(QString::fromStdString(*audiosource.token_));
  }

  if (audiosourceconfiguration.sourcetoken_.is_initialized())
  {
    ui_.comboaudiosources->setCurrentText(QString::fromStdString(*audiosourceconfiguration.sourcetoken_));

  }
}

void ManageONVIFMediaAudioSourceConfigurationWindow::on_buttonok_clicked()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SetEnabled(false);

  const onvif::AudioSourceConfiguration audiosourceconfiguration(ui_.editname->text().toStdString(), ui_.editusecount->text().toInt(), ui_.edittoken->text().toStdString(), ui_.comboaudiosources->currentText().toStdString());
  setaudiosourceconfigurationconnection_ = onvifmedia_->SetAudioSourceConfigurationCallback(audiosourceconfiguration, [this](const onvif::media::SetAudioSourceConfigurationResponse& setaudiosourceconfigurationresponse)
  {
    QApplication::restoreOverrideCursor();
    SetEnabled(true);

    if (setaudiosourceconfigurationresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(setaudiosourceconfigurationresponse.Message()), QMessageBox::Ok);
      return;
    }

    name_ = ui_.editname->text();
    accept();
  });
}

}

QT_END_NAMESPACE
