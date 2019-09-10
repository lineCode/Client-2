// manageonvifmediavideoencoderconfigurationswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediavideoencoderconfigurationswindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifmediavideoencoderconfigurationwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaVideoEncoderConfigurationsWindow::ManageONVIFMediaVideoEncoderConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonclose, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getvideoencoderconfigurationsconnection_ = onvifmedia_->GetVideoEncoderConfigurationsCallback([this](const onvif::media::GetVideoEncoderConfigurationsResponse& getvideoencoderconfigurationsresponse)
  {
    SetEnabled(true);

    if (getvideoencoderconfigurationsresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getvideoencoderconfigurationsresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& videoencoderconfiguration : getvideoencoderconfigurationsresponse.configurations_)
    {
      if (!videoencoderconfiguration.name_.is_initialized() || !videoencoderconfiguration.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("VideoEncoderConfiguration::token_ not initialised"));
        continue;
      }
      
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*videoencoderconfiguration.name_) + QString("(") + QString::fromStdString(*videoencoderconfiguration.token_) + QString(")"));
      item->setData(Qt::UserRole, QString::fromStdString(*videoencoderconfiguration.token_));
      ui_.listvideoencoderconfigurations->addItem(item);
    }
    ui_.listvideoencoderconfigurations->setCurrentRow(0);
  });
}

ManageONVIFMediaVideoEncoderConfigurationsWindow::~ManageONVIFMediaVideoEncoderConfigurationsWindow()
{

}

void ManageONVIFMediaVideoEncoderConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listvideoencoderconfigurations->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaVideoEncoderConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listvideoencoderconfigurations->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaVideoEncoderConfigurationWindow dlg(this, devicecapabilities_, onvifmedia_, item->data(Qt::UserRole).toString().toStdString());
  if (dlg.exec() == QDialog::Accepted)
  {
    item->setText(dlg.GetName() + QString("(") + item->data(Qt::UserRole).toString() + QString(")"));

  }
}

}

QT_END_NAMESPACE
