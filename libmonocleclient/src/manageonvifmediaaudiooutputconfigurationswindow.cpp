// manageaudiooutputconfigurationswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaaudiooutputconfigurationswindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QListWidgetItem>
#include <QMessageBox>

#include "monocleclient/manageonvifmediaaudiooutputconfigurationwindow.h"
#include "monocleclient/mainwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaAudioOutputConfigurationsWindow::ManageONVIFMediaAudioOutputConfigurationsWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonclose, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getaudiooutputconfigurationsconnection_ = onvifmedia_->GetAudioOutputConfigurationsCallback([this](const onvif::media::GetAudioOutputConfigurationsResponse& getaudiooutputconfigurationsresponse)
  {
    SetEnabled(true);

    if (getaudiooutputconfigurationsresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getaudiooutputconfigurationsresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& audiosourceconfiguration : getaudiooutputconfigurationsresponse.configurations_)
    {
      if (!audiosourceconfiguration.name_.is_initialized() || !audiosourceconfiguration.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid configuration"));
        continue;
      }
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*audiosourceconfiguration.name_) + QString("(") + QString::fromStdString(*audiosourceconfiguration.token_) + (QString(")")));
      item->setData(Qt::UserRole, QString::fromStdString(*audiosourceconfiguration.token_));
      ui_.listaudiooutputconfigurations->addItem(item);
    }
    ui_.listaudiooutputconfigurations->setCurrentRow(0);
  });
}

ManageONVIFMediaAudioOutputConfigurationsWindow::~ManageONVIFMediaAudioOutputConfigurationsWindow()
{

}

void ManageONVIFMediaAudioOutputConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listaudiooutputconfigurations->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaAudioOutputConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listaudiooutputconfigurations->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaAudioOutputConfigurationWindow dlg(this, onvifmedia_, item->data(Qt::UserRole).toString().toStdString());
  if (dlg.exec() == QDialog::Accepted)
  {
    item->setText(dlg.GetName() + QString("(") + item->data(Qt::UserRole).toString() + (QString(")")));

  }
}

}

QT_END_NAMESPACE
