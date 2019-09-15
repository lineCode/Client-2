// systemcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SystemCapabilities::SystemCapabilities()
{
  
}

SystemCapabilities::SystemCapabilities(const boost::optional<bool>& discoveryresolve, const boost::optional<bool>& discoverybye, const boost::optional<bool>& remotediscovery, const boost::optional<bool>& systembackup, const boost::optional<bool>& systemlogging, const boost::optional<bool>& firmwareupgrade, const std::vector<OnvifVersion>& supportedversions, const boost::optional<bool>& httpfirmwareupgrade, const boost::optional<bool>& httpsystembackup, const boost::optional<bool>& httpsystemlogging, const boost::optional<bool>& httpsupportinformation) :
  discoveryresolve_(discoveryresolve),
  discoverybye_(discoverybye),
  remotediscovery_(remotediscovery),
  systembackup_(systembackup),
  systemlogging_(systemlogging),
  firmwareupgrade_(firmwareupgrade),
  supportedversions_(supportedversions),
  httpfirmwareupgrade_(httpfirmwareupgrade),
  httpsystembackup_(httpsystembackup),
  httpsystemlogging_(httpsystemlogging),
  httpsupportinformation_(httpsupportinformation)
{
  
}

SystemCapabilities::SystemCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "DiscoveryResolve")
    {
      discoveryresolve_ = element.text().as_bool();

    }
    else if (name == "DiscoveryBye")
    {
      discoverybye_ = element.text().as_bool();

    }
    else if (name == "RemoteDiscovery")
    {
      remotediscovery_ = element.text().as_bool();

    }
    else if (name == "SystemBackup")
    {
      systembackup_ = element.text().as_bool();

    }
    else if (name == "SystemLogging")
    {
      systemlogging_ = element.text().as_bool();

    }
    else if (name == "FirmwareUpgrade")
    {
      firmwareupgrade_ = element.text().as_bool();

    }
    else if (name == "SupportedVersions")
    {
      supportedversions_.push_back(OnvifVersion(element));

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "HttpFirmwareUpgrade")
        {
          httpfirmwareupgrade_ = extension.text().as_bool();

        }
        else if (name == "HttpSystemBackup")
        {
          httpsystembackup_ = extension.text().as_bool();

        }
        else if (name == "HttpSystemLogging")
        {
          httpsystemlogging_ = extension.text().as_bool();

        }
        else if (name == "HttpSupportInformation")
        {
          httpsupportinformation_ = extension.text().as_bool();

        }
      }
    }
  }
}

std::string SystemCapabilities::ToXml(const std::string& name) const
{
  std::string extension;
  if (httpfirmwareupgrade_.is_initialized() || httpsystembackup_.is_initialized() || httpsystemlogging_.is_initialized() || httpsupportinformation_.is_initialized())
  {
    extension = std::string("<tt:Extension>") + onvif::ToXml("tt:HttpFirmwareUpgrade", httpfirmwareupgrade_) + onvif::ToXml("tt:HttpSystemBackup", httpsystembackup_) + onvif::ToXml("tt:HttpSystemLogging", httpsystemlogging_) + onvif::ToXml("tt:HttpSupportInformation", httpsupportinformation_) + "</tt:Extension>";

  }

  return ("<"+name+">" + onvif::ToXml("tt:DiscoveryResolve", discoveryresolve_) + onvif::ToXml("tt:DiscoveryBye", discoverybye_) + onvif::ToXml("tt:RemoteDiscovery", remotediscovery_) + onvif::ToXml("tt:SystemBackup", systembackup_) + onvif::ToXml("tt:SystemLogging", systemlogging_) + onvif::ToXml("tt:FirmwareUpgrade", firmwareupgrade_) + onvif::ToXml("tt:SupportedVersions", supportedversions_) + extension + "</"+name+">");
}

bool SystemCapabilities::operator==(const SystemCapabilities& rhs) const
{
  return ((discoveryresolve_ == rhs.discoveryresolve_) && (discoverybye_ == rhs.discoverybye_) && (remotediscovery_ == rhs.remotediscovery_) && (systembackup_ == rhs.systembackup_) && (systemlogging_ == rhs.systemlogging_) && (firmwareupgrade_ == rhs.firmwareupgrade_) && std::is_permutation(supportedversions_.begin(), supportedversions_.end(), rhs.supportedversions_.begin(), rhs.supportedversions_.end()) && (httpfirmwareupgrade_ == rhs.httpfirmwareupgrade_) && (httpsystembackup_ == rhs.httpsystembackup_) && (httpsystemlogging_ == rhs.httpsystemlogging_) && (httpsupportinformation_ == rhs.httpsupportinformation_));
}

}
