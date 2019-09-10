// manageonvifmediavideosourceconfigurationswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediavideosourceconfigurationswindow.h"

#include <onvifclient/mediaclient.hpp>
#include <QListWidget>
#include <QMessageBox>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifmediavideosourceconfigurationwindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFMediaVideoSourceConfigurationsWindow::ManageONVIFMediaVideoSourceConfigurationsWindow(QWidget* parent, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia) :
  QDialog(parent),
  onvifmedia_(onvifmedia)
{
  ui_.setupUi(this);
  
  connect(ui_.buttonclose, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getvideosourceconfigurationsconnection_ = onvifmedia_->GetVideoSourceConfigurationsCallback([this](const onvif::media::GetVideoSourceConfigurationsResponse& getvideosourceconfigurationsresponse)
  {
    SetEnabled(true);

    if (getvideosourceconfigurationsresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getvideosourceconfigurationsresponse.Message()), QMessageBox::Ok);
      return;
    }

    for (const auto& videosourceconfiguration : getvideosourceconfigurationsresponse.configurations_)
    {
      if (!videosourceconfiguration.name_.is_initialized() || !videosourceconfiguration.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid configuration"));
        continue;
      }
      QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*videosourceconfiguration.name_) + QString("(") + QString::fromStdString(*videosourceconfiguration.token_) + (QString(")")));
      item->setData(Qt::UserRole, QString::fromStdString(*videosourceconfiguration.token_));
      ui_.listvideosourceconfigurations->addItem(item);
    }
    ui_.listvideosourceconfigurations->setCurrentRow(0);
  });
}

ManageONVIFMediaVideoSourceConfigurationsWindow::~ManageONVIFMediaVideoSourceConfigurationsWindow()
{

}

void ManageONVIFMediaVideoSourceConfigurationsWindow::SetEnabled(bool enabled)
{
  ui_.listvideosourceconfigurations->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaVideoSourceConfigurationsWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listvideosourceconfigurations->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaVideoSourceConfigurationWindow dlg(this, onvifmedia_, item->data(Qt::UserRole).toString().toStdString());
  if (dlg.exec() == QDialog::Accepted)
  {
    item->setText(dlg.GetName() + QString("(") + item->data(Qt::UserRole).toString() + QString(")"));

  }
}

}

QT_END_NAMESPACE
