// manageonvifmediaaudiosourceconfigurationwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaaudiosourceconfigurationswindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QListWidgetItem>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifmediaaudiosourceconfigurationwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaAudioSourceConfigurationsWindow::ManageONVIFMediaAudioSourceConfigurationsWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonclose, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getaudiosourceconfigurationsconnection_ = onvifmedia_->GetAudioSourceConfigurationsCallback([this](const onvif::media::GetAudioSourceConfigurationsResponse& getaudiosourceconfigurationsresponse)
  {
    SetEnabled(true);

    if (getaudiosourceconfigurationsresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudiosourceconfigurationsresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& audiosourceconfiguration : getaudiosourceconfigurationsresponse.configurations_)
    {
      if (!audiosourceconfiguration.name_.is_initialized() || !audiosourceconfiguration.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid configuration"));
        continue;
      }
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*audiosourceconfiguration.name_) + QString("(") + QString::fromStdString(*audiosourceconfiguration.token_) + (QString(")")));
      item->setData(Qt::UserRole, QString::fromStdString(*audiosourceconfiguration.token_));
      ui_.listaudiosourceconfigurations->addItem(item);
    }
    ui_.listaudiosourceconfigurations->setCurrentRow(0);
  });
}

ManageONVIFMediaAudioSourceConfigurationsWindow::~ManageONVIFMediaAudioSourceConfigurationsWindow()
{

}

void ManageONVIFMediaAudioSourceConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listaudiosourceconfigurations->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaAudioSourceConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listaudiosourceconfigurations->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaAudioSourceConfigurationWindow dlg(this, onvifmedia_, item->data(Qt::UserRole).toString().toStdString());
  if (dlg.exec() == QDialog::Accepted)
  {
    item->setText(dlg.GetName() + QString("(") + item->data(Qt::UserRole).toString() + (QString(")")));

  }
}

}

QT_END_NAMESPACE
