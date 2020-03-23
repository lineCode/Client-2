// manageonvifwindow.cpp
//

///// Includes /////

#include "monocleclient/manageonvifwindow.h"

#include <onvifclient/analyticsclient.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/eventclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <onvifclient/ptzclient.hpp>
#include <QMessageBox>
#include <QStringList>

#include "monocleclient/mainwindow.h"
#include "monocleclient/manageonvifdeviceipv4networkinterfaceconfigurationswindow.h"
#include "monocleclient/manageonvifmediaaudiooutputconfigurationswindow.h"
#include "monocleclient/manageonvifmediaaudioencoderconfigurationswindow.h"
#include "monocleclient/manageonvifmediaaudiosourceconfigurationswindow.h"
#include "monocleclient/manageonvifmediametadataconfigurationswindow.h"
#include "monocleclient/manageonvifmediaprofileswindow.h"
#include "monocleclient/manageonvifmediavideoencoderconfigurationswindow.h"
#include "monocleclient/manageonvifmediavideosourceconfigurationswindow.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageONVIFWindow::ManageONVIFWindow(QWidget* parent, const sock::ProxyParams& proxyparams, const QString& mediauri, const QString& username, const QString& password) :
  QDialog(parent),
  proxyparams_(proxyparams),
  mediauri_(mediauri),
  username_(username),
  password_(password),
  onvifanalytics_(boost::make_shared<onvif::analytics::AnalyticsClient>(mutex_)),
  onvifdevice_(boost::make_shared<onvif::device::DeviceClient>(mutex_)),
  onvifevent_(boost::make_shared<onvif::event::EventClient>(mutex_)),
  onvifmedia_(boost::make_shared<onvif::media::MediaClient>(mutex_)),
  onvifptz_(boost::make_shared<onvif::ptz::PTZClient>(mutex_))
{
  ui_.setupUi(this);

  if (onvifdevice_->Init(proxyparams, mediauri.toStdString(), username.toStdString(), password.toStdString(), 1, false, true))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to initialise ONVIF Device: ") + mediauri, QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }
  
  startTimer(80);

  onvifconnection_ = onvifdevice_->GetSystemDateAndTimeCallback([this](const onvif::device::GetSystemDateAndTimeResponse& getsystemdateandtimeresponse)
  {
    if (!getsystemdateandtimeresponse.systemdatetime_.is_initialized())
    {
      LOG_GUI_WARNING("Failed to retrieve system date and time");

    }
    else
    {
      onvifanalytics_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      onvifdevice_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      onvifevent_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      onvifmedia_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      onvifptz_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
    }

    onvifconnection_ = onvifdevice_->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY_ALL, [this](const onvif::device::GetCapabilitiesResponse& getcapabilitiesresponse)
    {
      if (getcapabilitiesresponse.Error())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Failed to get capabilities: ") + QString::fromStdString(getcapabilitiesresponse.Message()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }

      if (!getcapabilitiesresponse.capabilities_.is_initialized())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Invalid capabilities"), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }

      devicecapabilities_ = *getcapabilitiesresponse.capabilities_;

      ui_.buttonmanageipv4networkinterfaces->setEnabled(true);

      QStringList error;
      if (getcapabilitiesresponse.capabilities_->analytics_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->analytics_->xaddr_.is_initialized())
        {
          error += "Invalid ONVIF Analytics xaddr";

        }
        else
        {
          if (onvifanalytics_->Init(proxyparams_, *getcapabilitiesresponse.capabilities_->analytics_->xaddr_, username_.toStdString(), password_.toStdString(), 1, false, true))
          {
            error += tr("Failed to initialise ONVIF Analytics: ") + QString::fromStdString(*getcapabilitiesresponse.capabilities_->analytics_->xaddr_);

          }
        }
      }

      if (getcapabilitiesresponse.capabilities_->events_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->events_->xaddr_.is_initialized())
        {
          error += "Invalid ONVIF Event xaddr";

        }
        else
        {
          if (onvifevent_->Init(proxyparams_, *getcapabilitiesresponse.capabilities_->events_->xaddr_, username_.toStdString(), password_.toStdString(), 1, false, true))
          {
            error += tr("Failed to initialise ONVIF Event: ") + QString::fromStdString(*getcapabilitiesresponse.capabilities_->events_->xaddr_);

          }
        }
      }

      if (getcapabilitiesresponse.capabilities_->media_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->media_->xaddr_.is_initialized())
        {
          error += "Invalid ONVIF Media xaddr";

        }
        else
        {
          if (onvifmedia_->Init(proxyparams_, *getcapabilitiesresponse.capabilities_->media_->xaddr_, username_.toStdString(), password_.toStdString(), 1, false, true))
          {
            error += tr("Failed to initialise ONVIF Media: ") + QString::fromStdString(*getcapabilitiesresponse.capabilities_->media_->xaddr_);

          }
        }

        ui_.buttonmanagemediaprofiles->setEnabled(true);
        if (getcapabilitiesresponse.capabilities_->NumAudioOutputs())
        {
          ui_.buttonmanageaudiooutputconfigurations->setEnabled(true);

        }
        if (getcapabilitiesresponse.capabilities_->NumAudioSources())
        {
          ui_.buttonmanageaudiosourceconfigurations->setEnabled(true);
          ui_.buttonmanageaudioencoderconfigurations->setEnabled(true);
        }
        ui_.buttonmediamanagemetadataconfigurations->setEnabled(true);
        ui_.buttonmanagevideoencoderconfigurations->setEnabled(true);
        ui_.buttonmanagevideosourceconfigurations->setEnabled(true);
      }

      if (getcapabilitiesresponse.capabilities_->ptz_.is_initialized())
      {
        if (!getcapabilitiesresponse.capabilities_->ptz_->xaddr_.is_initialized())
        {
          error += "Invalid ONVIF PTZ xaddr";

        }
        else
        {
          if (onvifptz_->Init(proxyparams_, *getcapabilitiesresponse.capabilities_->ptz_->xaddr_, username_.toStdString(), password_.toStdString(), 1, false, true))
          {
            error += tr("Failed to initialise ONVIF PTZ: ") + QString::fromStdString(*getcapabilitiesresponse.capabilities_->media_->xaddr_);

          }
        }
      }

      if (error.size())
      {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Error retrieving capabilities: ") + error.join('\n'), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
        return;
      }
    });
  });
}

ManageONVIFWindow::~ManageONVIFWindow()
{
  onvifanalytics_->Destroy();
  onvifdevice_->Destroy();
  onvifevent_->Destroy();
  onvifmedia_->Destroy();
  onvifptz_->Destroy();
  onvifanalytics_.reset();
  onvifdevice_.reset();
  onvifevent_.reset();
  onvifmedia_.reset();
  onvifptz_.reset();
}

void ManageONVIFWindow::timerEvent(QTimerEvent *event)
{
  onvifanalytics_->Update();
  onvifdevice_->Update();
  onvifevent_->Update();
  onvifmedia_->Update();
  onvifptz_->Update();
}

void ManageONVIFWindow::on_buttonmanageipv4networkinterfaces_clicked()
{
  ManageONVIFDeviceIPv4NetworkInterfaceConfigurationsWindow(this, devicecapabilities_, onvifdevice_).exec();

}

void ManageONVIFWindow::on_buttonmanagemediaprofiles_clicked()
{
  ManageONVIFMediaProfilesWindow(this, devicecapabilities_, onvifanalytics_, onvifdevice_, onvifmedia_, onvifptz_).exec();

}

void ManageONVIFWindow::on_buttonmanageaudioencoderconfigurations_clicked()
{
  ManageONVIFMediaAudioEncoderConfigurationsWindow(this, devicecapabilities_, onvifmedia_).exec();

}

void ManageONVIFWindow::on_buttonmanageaudiooutputconfigurations_clicked()
{
  ManageONVIFMediaAudioOutputConfigurationsWindow(this, onvifmedia_).exec();

}

void ManageONVIFWindow::on_buttonmanageaudiosourceconfigurations_clicked()
{
  ManageONVIFMediaAudioSourceConfigurationsWindow(this, onvifmedia_).exec();

}

void ManageONVIFWindow::on_buttonmediamanagemetadataconfigurations_clicked()
{
  ManageONVIFMediaMetadataConfigurationsWindow(this, devicecapabilities_, onvifevent_, onvifmedia_).exec();

}

void ManageONVIFWindow::on_buttonmanagevideoencoderconfigurations_clicked()
{
  ManageONVIFMediaVideoEncoderConfigurationsWindow(this, devicecapabilities_, onvifmedia_).exec();

}

void ManageONVIFWindow::on_buttonmanagevideosourceconfigurations_clicked()
{
  ManageONVIFMediaVideoSourceConfigurationsWindow(this, onvifmedia_).exec();

}

void ManageONVIFWindow::on_buttonok_clicked()
{
  accept();
  
}

}

QT_END_NAMESPACE
