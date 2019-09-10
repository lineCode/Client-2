// manageonvifmediaaudioencoderconfigurationswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaaudioencoderconfigurationswindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QListWidgetItem>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifmediaaudioencoderconfigurationwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaAudioEncoderConfigurationsWindow::ManageONVIFMediaAudioEncoderConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonclose, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getaudioencoderconfigurationsconnection_ = onvifmedia_->GetAudioEncoderConfigurationsCallback([this](const onvif::media::GetAudioEncoderConfigurationsResponse& getaudioencoderconfigurationsresponse)
  {
    SetEnabled(true);

    if (getaudioencoderconfigurationsresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudioencoderconfigurationsresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& audiosourceconfiguration : getaudioencoderconfigurationsresponse.configurations_)
    {
      if (!audiosourceconfiguration.name_.is_initialized() || !audiosourceconfiguration.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid configuration"));
        continue;
      }
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*audiosourceconfiguration.name_) + QString("(") + QString::fromStdString(*audiosourceconfiguration.token_) + (QString(")")));
      item->setData(Qt::UserRole, QString::fromStdString(*audiosourceconfiguration.token_));
      ui_.listaudioencoderconfigurations->addItem(item);
    }
    ui_.listaudioencoderconfigurations->setCurrentRow(0);
  });
}

ManageONVIFMediaAudioEncoderConfigurationsWindow::~ManageONVIFMediaAudioEncoderConfigurationsWindow()
{

}

void ManageONVIFMediaAudioEncoderConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listaudioencoderconfigurations->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaAudioEncoderConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listaudioencoderconfigurations->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaAudioEncoderConfigurationWindow dlg(this, devicecapabilities_, onvifmedia_, item->data(Qt::UserRole).toString().toStdString());
  if (dlg.exec() == QDialog::Accepted)
  {
    item->setText(dlg.GetName() + QString("(") + item->data(Qt::UserRole).toString() + (QString(")")));

  }
}

}

QT_END_NAMESPACE
