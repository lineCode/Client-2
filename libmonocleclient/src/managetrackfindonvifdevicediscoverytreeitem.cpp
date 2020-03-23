// managetrackfindonvifdevicediscoverytreeitem.cpp
//

///// Includes /////

#include "monocleclient/managetrackfindonvifdevicediscoverytreeitem.h"

#include <boost/algorithm/string/join.hpp>
#include <onvifclient/deviceclient.hpp>
#include <onvifclient/mediaclient.hpp>
#include <QString>

#include "monocleclient/device.h"

///// Namespaces /////

namespace client
{

///// Methods /////

ManageTrackFindONVIFDeviceDiscoveryTreeItem::ManageTrackFindONVIFDeviceDiscoveryTreeItem(const boost::shared_ptr<Device>& device, const std::vector<std::string>& names, const std::vector<std::string>& locations, const std::string& address, const std::string& username, const std::string& password) :
  QTreeWidgetItem(QStringList({ QString::fromStdString(boost::algorithm::join(names, ", ")), QString::fromStdString(boost::algorithm::join(locations, ", ")), QString::fromStdString(address) })),
  device_(device),
  address_(address),
  username_(username),
  password_(password)
{
  setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);

}

ManageTrackFindONVIFDeviceDiscoveryTreeItem::~ManageTrackFindONVIFDeviceDiscoveryTreeItem()
{
  if (onvifmedia_)
  {
    onvifmedia_->Destroy();
    onvifmedia_.reset();
  }
  if (onvifdevice_)
  {
    onvifdevice_->Destroy();
    onvifdevice_.reset();
  }
}

void ManageTrackFindONVIFDeviceDiscoveryTreeItem::SetUserPass(const std::string& username, const std::string& password)
{
  username_ = username;
  password_ = password;

  if (!isExpanded())
  {

    return;
  }

  GetProfiles();
}

void ManageTrackFindONVIFDeviceDiscoveryTreeItem::Collapsed()
{
  connection_.Close();
  if (onvifmedia_)
  {
    onvifmedia_->Destroy();
    onvifmedia_.reset();
  }
  if (onvifdevice_)
  {
    onvifdevice_->Destroy();
    onvifdevice_.reset();
  }
  RemoveChildren();
}

void ManageTrackFindONVIFDeviceDiscoveryTreeItem::Expanded()
{
  GetProfiles();

}

int ManageTrackFindONVIFDeviceDiscoveryTreeItem::Update()
{
  if (onvifdevice_ && onvifdevice_->IsInitialised())
  {
    onvifdevice_->Update();

  }
  if (onvifmedia_ && onvifmedia_->IsInitialised())
  {
    onvifmedia_->Update();

  }
  return 0;
}

void ManageTrackFindONVIFDeviceDiscoveryTreeItem::RemoveChildren()
{
  for (QTreeWidgetItem* child : takeChildren())
  {
    delete child;

  }
}

void ManageTrackFindONVIFDeviceDiscoveryTreeItem::GetProfiles()
{
  connection_.Close();
  if (onvifmedia_)
  {
    onvifmedia_->Destroy();
    onvifmedia_.reset();
  }
  if (onvifdevice_)
  {
    onvifdevice_->Destroy();
    onvifdevice_.reset();
  }
  RemoveChildren();

  onvifdevice_ = boost::make_shared<onvif::device::DeviceClient>(mutex_);
  onvifmedia_ = boost::make_shared<onvif::media::MediaClient>(mutex_);
  if (onvifdevice_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), address_, username_, password_, 1 , false, true))
  {
    SetChildText("Unable to initialise device service");
    return;
  }

  SetChildText("Loading...");

  connection_ = onvifdevice_->GetSystemDateAndTimeCallback([this](const onvif::device::GetSystemDateAndTimeResponse& getsystemdateandtimeresponse)
  {
    if (!onvifdevice_ || !onvifmedia_) // Not strictly necessary but lets be careful...
    {
      // Ignore
      return;
    }

    if (getsystemdateandtimeresponse.Error())
    {
      if (getsystemdateandtimeresponse.Message() == onvif::UNABLETOCONNECT)
      {
        SetChildText(QString::fromStdString("GetSystemDateAndTime error"));
        return;
      }
      else
      {
        // We ignore this error because some cameras will always error on this, and then proceed to not care about time at all
        //SetChildText(QString::fromStdString("GetSystemDateAndTime error: " + getsystemdateandtimeresponse.Message()));
        //return;
      }
    }

    if (getsystemdateandtimeresponse.systemdatetime_.is_initialized())
    {
      onvifdevice_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
      onvifmedia_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
    }

    connection_ = onvifdevice_->GetCapabilitiesCallback(onvif::CAPABILITYCATEGORY_ALL, [this](const onvif::device::GetCapabilitiesResponse& getcapabilitiesresponse)
    {
      if (!onvifdevice_ || !onvifmedia_)
      {
        // Ignore
        return;
      }

      if (getcapabilitiesresponse.Error())
      {
        SetChildText(QString::fromStdString("GetCapabilities error"));
        return;
      }

      if (!getcapabilitiesresponse.capabilities_.is_initialized())
      {
        SetChildText("GetCapabilitiesResponse::capabilities_ not initialised");
        return;
      }

      if (!getcapabilitiesresponse.capabilities_->media_.is_initialized())
      {
        SetChildText("GetCapabilitiesResponse::capabilities_::onvifmedia_ not initialised");
        return;
      }

      if (onvifmedia_->Init(sock::ProxyParams(sock::PROXYTYPE_HTTP, device_->GetAddress().toStdString(), device_->GetPort(), true, device_->GetUsername().toStdString(), device_->GetPassword().toStdString()), getcapabilitiesresponse.capabilities_->media_->GetXAddr(), onvifdevice_->GetUsername(), onvifdevice_->GetPassword(), 1, false, true))
      {
        SetChildText("Unable to initialise media service");
        return;
      }

      connection_ = onvifmedia_->GetProfilesCallback([this](const onvif::media::GetProfilesResponse& getprofilesresponse)
      {
        if (getprofilesresponse.Error())
        {
          SetChildText(QString::fromStdString("GetProfiles error"));
          return;
        }

        RemoveChildren();
        for (const onvif::Profile& profile : getprofilesresponse.profiles_)
        {
          if (!profile.token_.is_initialized())
          {
            // Ignore
            continue;
          }

          QString name;
          if (profile.name_.is_initialized())
          {
            name = " " + QString::fromStdString(*profile.name_);

          }

          QTreeWidgetItem* profileitem = new QTreeWidgetItem(this, { QString::fromStdString(*profile.token_) + name });
          profileitem->setData(0, Qt::UserRole, RECEIVERDISCOVERYITEM_PROFILE);
          profileitem->setData(0, Qt::UserRole + 1, QString::fromStdString(address_));
          profileitem->setData(0, Qt::UserRole + 2, QString::fromStdString(*profile.token_));
          QStringList listitems;
          if (profile.videosourceconfiguration_.is_initialized())
          {
            if (profile.videosourceconfiguration_->token_.is_initialized())
            {
              listitems.push_back("VideoSource Token: " + QString::fromStdString(*profile.videosourceconfiguration_->token_));

            }
            if (profile.videosourceconfiguration_->name_.is_initialized())
            {
              listitems.push_back("VideoSource Name: " + QString::fromStdString(*profile.videosourceconfiguration_->name_));

            }
            if (profile.videosourceconfiguration_->sourcetoken_.is_initialized())
            {
              listitems.push_back("VideoSource Source Token: " + QString::fromStdString(*profile.videosourceconfiguration_->sourcetoken_));

            }
            if (profile.videosourceconfiguration_->rotate_.is_initialized() && profile.videosourceconfiguration_->rotate_->rotatemode_.is_initialized() && profile.videosourceconfiguration_->rotate_->degree_.is_initialized())
            {
              listitems.push_back("VideoSource Rotation: " + QString::number(*profile.videosourceconfiguration_->rotate_->degree_));

            }
          }
          if (profile.audiosourceconfiguration_.is_initialized())
          {
            if (profile.audiosourceconfiguration_->token_.is_initialized())
            {
              listitems.push_back("AudioSource Token: " + QString::fromStdString(*profile.audiosourceconfiguration_->token_));

            }
            if (profile.audiosourceconfiguration_->name_.is_initialized())
            {
              listitems.push_back("AudioSource Name: " + QString::fromStdString(*profile.audiosourceconfiguration_->name_));

            }
            if (profile.audiosourceconfiguration_->sourcetoken_.is_initialized())
            {
              listitems.push_back("AudioSource Source Token: " + QString::fromStdString(*profile.audiosourceconfiguration_->sourcetoken_));

            }
          }
          if (profile.audioencoderconfiguration_.is_initialized())
          {
            if (profile.audioencoderconfiguration_->token_.is_initialized())
            {
              listitems.push_back("AudioEncoder Token: " + QString::fromStdString(*profile.audioencoderconfiguration_->token_));

            }
            if (profile.audioencoderconfiguration_->name_.is_initialized())
            {
              listitems.push_back("AudioEncoder Name: " + QString::fromStdString(*profile.audioencoderconfiguration_->name_));

            }
            if (profile.audioencoderconfiguration_->encoding_.is_initialized())
            {
              listitems.push_back("AudioEncoder Encoding: " + QString::fromStdString(onvif::ToString(*profile.audioencoderconfiguration_->encoding_)));

            }
            if (profile.audioencoderconfiguration_->bitrate_.is_initialized())
            {
              listitems.push_back("AudioEncoder Bitrate: " + QString::fromStdString(onvif::ToString(*profile.audioencoderconfiguration_->bitrate_)));

            }
            if (profile.audioencoderconfiguration_->samplerate_.is_initialized())
            {
              listitems.push_back("AudioEncoder Samplerate: " + QString::fromStdString(onvif::ToString(*profile.audioencoderconfiguration_->samplerate_)));

            }
          }
          if (profile.videoencoderconfiguration_.is_initialized())
          {
            if (profile.videoencoderconfiguration_->token_.is_initialized())
            {
              listitems.push_back("VideoEncoder Token: " + QString::fromStdString(*profile.videoencoderconfiguration_->token_));

            }
            if (profile.videoencoderconfiguration_->name_.is_initialized())
            {
              listitems.push_back("AudioEncoder Name: " + QString::fromStdString(*profile.videoencoderconfiguration_->name_));

            }
            if (profile.videoencoderconfiguration_->encoding_.is_initialized())
            {
              listitems.push_back("VideoEncoder Encoding: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->encoding_)));

            }
            if (profile.videoencoderconfiguration_->resolution_.is_initialized() && profile.videoencoderconfiguration_->resolution_->width_.is_initialized() && profile.videoencoderconfiguration_->resolution_->height_.is_initialized())
            {
              listitems.push_back("VideoEncoder dimensions: " + QString::number(*profile.videoencoderconfiguration_->resolution_->width_) + "x" + QString::number(*profile.videoencoderconfiguration_->resolution_->height_));

            }
            if (profile.videoencoderconfiguration_->quality_.is_initialized())
            {
              listitems.push_back("VideoEncoder Quality: " + QString::number(*profile.videoencoderconfiguration_->quality_));

            }
            if (profile.videoencoderconfiguration_->mpeg4_.is_initialized())
            {
              if (profile.videoencoderconfiguration_->mpeg4_->mpeg4profile_.is_initialized())
              {
                listitems.push_back("VideoEncoder MPEG4 Profile: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->mpeg4_->mpeg4profile_)));

              }
              if (profile.videoencoderconfiguration_->mpeg4_->govlength_.is_initialized())
              {
                listitems.push_back("VideoEncoder GOV Length: " + QString::number(*profile.videoencoderconfiguration_->mpeg4_->govlength_));

              }
            }
            if (profile.videoencoderconfiguration_->h264_.is_initialized())
            {
              if (profile.videoencoderconfiguration_->h264_->h264profile_.is_initialized())
              {
                listitems.push_back("VideoEncoder MPEG4 Profile: " + QString::fromStdString(onvif::ToString(*profile.videoencoderconfiguration_->h264_->h264profile_)));

              }
              if (profile.videoencoderconfiguration_->h264_->govlength_.is_initialized())
              {
                listitems.push_back("VideoEncoder GOV Length: " + QString::number(*profile.videoencoderconfiguration_->h264_->govlength_));

              }
            }
          }
          if (profile.ptzconfiguration_.is_initialized())
          {
            if (profile.ptzconfiguration_->token_.is_initialized())
            {
              listitems.push_back("PTZ Token: " + QString::fromStdString(*profile.ptzconfiguration_->token_));

            }
            if (profile.ptzconfiguration_->name_.is_initialized())
            {
              listitems.push_back("PTZ Name: " + QString::fromStdString(*profile.ptzconfiguration_->name_));

            }
          }
          if (profile.metadataconfiguration_.is_initialized())
          {
            if (profile.metadataconfiguration_->token_.is_initialized())
            {
              listitems.push_back("Metadata Token: " + QString::fromStdString(*profile.metadataconfiguration_->token_));

            }
            if (profile.metadataconfiguration_->name_.is_initialized())
            {
              listitems.push_back("Metadata Name: " + QString::fromStdString(*profile.metadataconfiguration_->name_));

            }
          }
          profileitem->setData(0, Qt::UserRole + 3, listitems);
          addChild(profileitem);
        }

        if (childCount() == 0)
        {
          SetChildText(QString::fromStdString("No profiles found"));

        }
      });
    });
  });
}

void ManageTrackFindONVIFDeviceDiscoveryTreeItem::SetChildText(const QString& text)
{
  RemoveChildren();
  addChild(new QTreeWidgetItem(this, { QString(text) }));
}

}
