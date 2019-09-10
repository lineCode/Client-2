// managemetadataconfigurationswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediametadataconfigurationswindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifmediametadataconfigurationwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaMetadataConfigurationsWindow::ManageONVIFMediaMetadataConfigurationsWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::event::EventClient>& onvifevent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifevent_(onvifevent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonclose, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getmetadataconfigurationsconnection_ = onvifmedia_->GetMetadataConfigurationsCallback([this](const onvif::media::GetMetadataConfigurationsResponse& getmetadataconfigurationsresponse)
  {
    SetEnabled(true);

    if (getmetadataconfigurationsresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getmetadataconfigurationsresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& metadataconfiguration : getmetadataconfigurationsresponse.configurations_)
    {
      if (!metadataconfiguration.name_.is_initialized() || !metadataconfiguration.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("MetadataConfiguration::token_ not initialised"));
        continue;
      }
      
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*metadataconfiguration.name_) + QString("(") + QString::fromStdString(*metadataconfiguration.token_) + (QString(")")));
      item->setData(Qt::UserRole, QString::fromStdString(*metadataconfiguration.token_));
      ui_.listmetadataconfigurations->addItem(item);
    }
    ui_.listmetadataconfigurations->setCurrentRow(0);
  });
}

ManageONVIFMediaMetadataConfigurationsWindow::~ManageONVIFMediaMetadataConfigurationsWindow()
{

}

void ManageONVIFMediaMetadataConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listmetadataconfigurations->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaMetadataConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listmetadataconfigurations->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaMetadataConfigurationWindow dlg(this, devicecapabilities_, onvifevent_, onvifmedia_, item->data(Qt::UserRole).toString().toStdString());
  if (dlg.exec() == QDialog::Accepted)
  {
    item->setText(dlg.GetName() + QString("(") + item->data(Qt::UserRole).toString() + (QString(")")));

  }
}

}

QT_END_NAMESPACE
