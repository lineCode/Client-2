// managetrackfindonvifdevicewindow.cpp
//

///// Includes /////

#include "monocleclient/managetrackfindonvifdevicewindow.h"

#include <network/uri.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QMessageBox>
#include <QTimer>
#include <QUrl>

#include "monocleclient/device.h"
#include "monocleclient/mainwindow.h"
#include "monocleclient/receiver.h"
#include "monocleclient/managetrackfindonvifdevicediscoverytreeitem.h"

///// Namespaces /////

QT_BEGIN_NAMESPACE

namespace client
{

///// Methods /////

ManageTrackFindONVIFDeviceWindow::ManageTrackFindONVIFDeviceWindow(QWidget* parent, const boost::shared_ptr<Device>& device, const QString& mediauri, const QString& username, const QString& password) :
  QDialog(parent),
  device_(device)
{
  ui_.setupUi(this);

  connect(ui_.buttoncancel, &QPushButton::clicked, this, &QDialog::reject);

  ui_.treediscovery->Init(device_, username.toStdString(), password.toStdString());

  // Setup
  ui_.edituri->setText(mediauri);
  ui_.editusername->setText(username);
  ui_.editpassword->setText(password);

  startTimer(std::chrono::milliseconds(40));
}

ManageTrackFindONVIFDeviceWindow::~ManageTrackFindONVIFDeviceWindow()
{
  onvifconnection_.Close();

  if (mediaclient_)
  {
    mediaclient_->Destroy();
    mediaclient_.reset();
  }

  if (deviceclient_)
  {
    deviceclient_->Destroy();
    deviceclient_.reset();
  }
}

void ManageTrackFindONVIFDeviceWindow::timerEvent(QTimerEvent*)
{
  if (deviceclient_ && deviceclient_->IsInitialised())
  {
    deviceclient_->Update();

  }

  if (mediaclient_ && mediaclient_->IsInitialised())
  {
    mediaclient_->Update();

  }
}

void ManageTrackFindONVIFDeviceWindow::SetEnabled(const bool enabled)
{
  ui_.edituri->setEnabled(enabled);
  ui_.editusername->setEnabled(enabled);
  ui_.editpassword->setEnabled(enabled);
  ui_.buttonok->setEnabled(enabled);
}

void ManageTrackFindONVIFDeviceWindow::on_edittextfilter_textChanged()
{
  ui_.treediscovery->SetTextFilter(ui_.edittextfilter->text());

}

void ManageTrackFindONVIFDeviceWindow::on_checkipv4_clicked()
{
  ui_.treediscovery->ShowIpv4(ui_.checkipv4->isChecked());

}

void ManageTrackFindONVIFDeviceWindow::on_checkipv6_clicked()
{
  ui_.treediscovery->ShowIpv6(ui_.checkipv6->isChecked());

}

void ManageTrackFindONVIFDeviceWindow::on_checklinklocal_clicked()
{
  ui_.treediscovery->ShowLinkLocal(ui_.checklinklocal->isChecked());

}

void ManageTrackFindONVIFDeviceWindow::on_buttondiscoverrefresh_clicked()
{
  ui_.treediscovery->SetUserPass(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());
  ui_.treediscovery->Refresh();
}

void ManageTrackFindONVIFDeviceWindow::on_treediscovery_itemPressed(QTreeWidgetItem* item, int column)
{
  if (item && (item->data(0, Qt::UserRole) == RECEIVERDISCOVERYITEM_DEVICE))
  {
    ui_.edituri->setText(item->data(0, ADDRESS_ROLE).toString());
    profile_.clear();
  }
  else if (item && (item->data(0, Qt::UserRole) == RECEIVERDISCOVERYITEM_PROFILE))
  {
    ui_.edituri->setText(item->data(0, ADDRESS_ROLE).toString());
    profile_ = item->data(0, PROFILE_TOKEN_ROLE).toString();
  }
}

void ManageTrackFindONVIFDeviceWindow::on_buttontest_clicked()
{
  ui_.labeltestoutput->clear();

  onvifconnection_.Close();
  if (mediaclient_)
  {
    mediaclient_->Destroy();
    mediaclient_.reset();
  }
  if (deviceclient_)
  {
    deviceclient_->Destroy();
    deviceclient_.reset();
  }

  network::uri uri;
  try
  {
    uri = network::uri(ui_.edituri->text().toStdString());

  }
  catch (...)
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + "</font><br/>");
    return;
  }

  if (!uri.has_scheme())
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no schema present</font><br/>");
    return;
  }

  if (!boost::algorithm::iequals(uri.scheme().to_string(), "http") && !boost::algorithm::iequals(uri.scheme().to_string(), "https"))
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI schema: " + QString::fromStdString(uri.scheme().to_string()) + "</font><br/>");
    return;
  }

  if (!uri.has_host())
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI: " + ui_.edituri->text() + " no host present</font><br/>");
    return;
  }

  uint16_t port = 80;
  if (uri.has_port())
  {
    try
    {
      port = boost::lexical_cast<uint16_t>(uri.port().to_string());

    }
    catch (...)
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Invalid URI port: " + QString::fromStdString(uri.port().to_string()) + "</font><br/>");
      return;
    }
  }

  deviceclient_ = boost::make_shared<onvif::device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
  mediaclient_ = boost::make_shared<onvif::media::MediaClient>(boost::make_shared<std::recursive_mutex>());
  if (deviceclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), ui_.edituri->text().toStdString(), ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString(), 1, false, true))
  {
    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Failed to initialise ONVIF Device Client</font><br/>");
    return;
  }

  ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving System Date and Time</font><br/>");
  onvifconnection_ = deviceclient_->GetSystemDateAndTimeCallback([this](const onvif::device::GetSystemDateAndTimeResponse& getsystemdateandtimeresponse)
  {
    if (getsystemdateandtimeresponse.Message() == onvif::UNABLETOCONNECT)
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetSystemDateAndTime failed: " + QString::fromStdString(getsystemdateandtimeresponse.Message()) + "</font><br/>");
      return;
    }
    else if (getsystemdateandtimeresponse.Error())
    {
      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"orange\">GetSystemDateAndTime failed: " + QString::fromStdString(getsystemdateandtimeresponse.Message()) + "</font><br/>");

    }

    if (getsystemdateandtimeresponse.systemdatetime_.is_initialized())
    {
      deviceclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      mediaclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
    }

    ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving Capabilities</font><br/>");
    onvifconnection_ = deviceclient_->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY::CAPABILITYCATEGORY_ALL, [this](const onvif::device::GetCapabilitiesResponse& getcapabilitiesresponse)
    {
      if (getcapabilitiesresponse.Error())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities failed: " + QString::fromStdString(getcapabilitiesresponse.Message()) + "</font><br/>");
        return;
      }

      if (!getcapabilitiesresponse.capabilities_.is_initialized())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities invalid response no capabilities found</font><br/>");
        return;
      }

      if (!getcapabilitiesresponse.capabilities_->media_.is_initialized())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities invalid response no media capabilities</font><br/>");
        return;
      }

      const std::string mediaxaddr = getcapabilitiesresponse.capabilities_->media_->GetXAddr();
      if (mediaxaddr.empty())
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetCapabilities invalid response no media XADDR</font><br/>");
        return;
      }

      if (mediaclient_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), mediaxaddr, deviceclient_->GetUsername(), deviceclient_->GetPassword(), 1, false, true))
      {
        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">Failed to initialise Media client</font><br/>");
        return;
      }

      ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Retrieving Profiles</font><br/>");
      onvifconnection_ = mediaclient_->GetProfilesCallback([this](const onvif::media::GetProfilesResponse& getprofilesresponse)
      {
        if (getprofilesresponse.Error())
        {
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">GetProfiles failed: " + QString::fromStdString(getprofilesresponse.Message()) + "</font><br/>");
          return;
        }

        if (getprofilesresponse.profiles_.empty())
        {
          ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"red\">No profiles found</font><br/>");
          return;
        }

        ui_.labeltestoutput->setText(ui_.labeltestoutput->text() + "<font color=\"green\">Success: " + QString::number(getprofilesresponse.profiles_.size()) + " Profiles found</font><br/>");
      });
    });
  });
}

void ManageTrackFindONVIFDeviceWindow::on_editusername_editingFinished()
{
  ui_.treediscovery->SetUserPass(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

}

void ManageTrackFindONVIFDeviceWindow::on_editpassword_editingFinished()
{
  ui_.treediscovery->SetUserPass(ui_.editusername->text().toStdString(), ui_.editpassword->text().toStdString());

}

void ManageTrackFindONVIFDeviceWindow::on_buttonok_clicked()
{
  network::uri uri;
  try
  {
    uri = network::uri(ui_.edituri->text().toStdString());

  }
  catch (...)
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI: " + ui_.edituri->text(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!uri.has_scheme())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI: " + ui_.edituri->text(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!boost::algorithm::iequals(uri.scheme().to_string(), "http") && !boost::algorithm::iequals(uri.scheme().to_string(), "https"))
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI schema: " + QString::fromStdString(uri.scheme().to_string()), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  if (!uri.has_host())
  {
    QMessageBox(QMessageBox::Warning, tr("Error"), "Invalid URI: " + ui_.edituri->text(), QMessageBox::Ok, nullptr, Qt::MSWindowsFixedSizeDialogHint).exec();
    return;
  }

  uri_ = ui_.edituri->text();
  username_ = ui_.editusername->text();
  password_ = ui_.editpassword->text();
  accept();
}

}

QT_END_NAMESPACE
