// manageonvifmediaprofileswindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifmediaprofileswindow.h"

#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <onvifclient/ptzclient.hpp>
#include <QMessageBox>
#include <QListWidget>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifmediacreateprofilewindow.h"
#include "monocleclient/manageonvifmediaprofilewindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Globals /////

const int ROLE_TOKEN = Qt::UserRole;
const int ROLE_FIXED = Qt::UserRole + 1;

///// Methods /////

ManageONVIFMediaProfilesWindow::ManageONVIFMediaProfilesWindow(QWidget* parent, const onvif::Capabilities& devicecapabilities, const boost::shared_ptr<onvif::analytics::AnalyticsClient>& onvifanalytics, const boost::shared_ptr<onvif::device::DeviceClient>& onvifdevice, const boost::shared_ptr<onvif::media::MediaClient>& onvifmedia, const boost::shared_ptr<onvif::ptz::PTZClient>& onvifptz) :
  QDialog(parent),
  devicecapabilities_(devicecapabilities),
  onvifanalytics_(onvifanalytics),
  onvifdevice_(onvifdevice),
  onvifmedia_(onvifmedia),
  onvifptz_(onvifptz)
{
  ui_.setupUi(this);

  connect(ui_.buttonok, &QPushButton::clicked, this, &QDialog::reject);

  SetEnabled(false);

  getprofilesconnection_ = onvifmedia_->GetProfilesCallback([this](const onvif::media::GetProfilesResponse& getprofilesresponse)
  {
    if (getprofilesresponse.Error())
    {
      QMessageBox::warning(this, tr("Error"), QString::fromStdString(getprofilesresponse.Message()), QMessageBox::Ok);
      return;
    }

    SetEnabled(true);

    for (const auto& profile : getprofilesresponse.profiles_)
    {
      if (!profile.name_.is_initialized() || !profile.token_.is_initialized())
      {
        LOG_GUI_WARNING(tr("Invalid profile"));
        continue;
      }

      AddItem(profile);
    }
    ui_.listprofiles->setCurrentRow(0);
  });
}

ManageONVIFMediaProfilesWindow::~ManageONVIFMediaProfilesWindow()
{

}

void ManageONVIFMediaProfilesWindow::SetEnabled(bool enabled)
{
  ui_.listprofiles->setEnabled(enabled);
  ui_.buttoncreate->setEnabled(enabled);
  ui_.buttondelete->setEnabled(enabled);
  ui_.buttonmanage->setEnabled(enabled);
}

void ManageONVIFMediaProfilesWindow::AddItem(const onvif::Profile& profile)
{
  QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(*profile.name_) + QString("(") + QString::fromStdString(*profile.token_) + QString(")"));
  item->setData(ROLE_TOKEN, QString::fromStdString(*profile.token_));
  item->setData(ROLE_FIXED, profile.fixed_.is_initialized() ? *profile.fixed_ : false);
  ui_.listprofiles->addItem(item);
}

void ManageONVIFMediaProfilesWindow::on_buttondelete_clicked()
{
  QListWidgetItem* item = ui_.listprofiles->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  if (ui_.listprofiles->currentItem()->data(ROLE_FIXED).toBool())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("Can not delete fixed profile"), QMessageBox::Ok).exec();
    return;
  }

  if (QMessageBox::question(this, tr("Remove"), tr("Are you sure?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
  {
    SetEnabled(false);

    deleteprofilesconnection_ = onvifmedia_->DeleteProfileCallback(ui_.listprofiles->currentItem()->data(ROLE_TOKEN).toString().toStdString(), [this](const onvif::media::DeleteProfileResponse& deleteprofileresponse)
    {
      SetEnabled(true);

      if (deleteprofileresponse.Error())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), QString::fromStdString(deleteprofileresponse.Message()), QMessageBox::Ok).exec();
        return;
      }

      for (int i = 0; i < ui_.listprofiles->count(); ++i)
      {
        if (ui_.listprofiles->item(i)->data(ROLE_TOKEN).toString().toStdString() == deleteprofileresponse.token_)
        {
          delete ui_.listprofiles->takeItem(i);
          break;
        }
      }
    });
  }
}

void ManageONVIFMediaProfilesWindow::on_buttoncreate_clicked()
{
  ManageONVIFMediaCreateProfileWindow dlg(this, onvifmedia_);
  if (dlg.exec() == QDialog::Accepted)
  {
    if (!dlg.GetProfile().is_initialized())
    {
      LOG_GUI_WARNING(tr("Invalid profile"));
      return;
    }
    AddItem(*dlg.GetProfile());
  }
}

void ManageONVIFMediaProfilesWindow::on_buttonmanage_clicked()
{
  QListWidgetItem* item = ui_.listprofiles->currentItem();
  if (!item)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), QString("No item currently selected"), QMessageBox::Ok).exec();
    return;
  }

  ManageONVIFMediaProfileWindow(this, devicecapabilities_, onvifanalytics_, onvifdevice_, onvifmedia_, onvifptz_, item->data(Qt::UserRole).toString().toStdString()).exec();
}

void ManageONVIFMediaProfilesWindow::on_buttonok_clicked()
{
  accept();
  
}

}

QT_END_NAMESPACE
