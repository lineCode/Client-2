// systemcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Classes /////

SystemCapabilities::SystemCapabilities()
{

}

SystemCapabilities::SystemCapabilities(const boost::optional<bool>& discoveryresolve, const boost::optional<bool>& discoverybye, const boost::optional<bool>& remotediscovery, const boost::optional<bool>& systembackup, const boost::optional<bool>& systemlogging, const boost::optional<bool>& firmwareupgrade, const boost::optional<bool>& httpfirmwareupgrade, const boost::optional<bool>& httpsystembackup, const boost::optional<bool>& httpsystemlogging, const boost::optional<bool>& httpsupportinformation, const boost::optional<bool>& storageconfiguration) :
  discoveryresolve_(discoveryresolve),
  discoverybye_(discoverybye),
  remotediscovery_(remotediscovery),
  systembackup_(systembackup),
  systemlogging_(systemlogging),
  firmwareupgrade_(firmwareupgrade),
  httpfirmwareupgrade_(httpfirmwareupgrade),
  httpsystembackup_(httpsystembackup),
  httpsystemlogging_(httpsystemlogging),
  httpsupportinformation_(httpsupportinformation),
  storageconfiguration_(storageconfiguration)
{

}

SystemCapabilities::SystemCapabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "DiscoveryResolve")
    {
      discoveryresolve_ = attribute.as_bool();

    }
    else if (name == "DiscoveryBye")
    {
      discoverybye_ = attribute.as_bool();

    }
    else if (name == "RemoteDiscovery")
    {
      remotediscovery_ = attribute.as_bool();

    }
    else if (name == "SystemBackup")
    {
      systembackup_ = attribute.as_bool();

    }
    else if (name == "SystemLogging")
    {
      systemlogging_ = attribute.as_bool();

    }
    else if (name == "FirmwareUpgrade")
    {
      firmwareupgrade_ = attribute.as_bool();

    }
    else if (name == "HttpFirmwareUpgrade")
    {
      httpfirmwareupgrade_ = attribute.as_bool();

    }
    else if (name == "HttpSystemBackup")
    {
      httpsystembackup_ = attribute.as_bool();

    }
    else if (name == "HttpSystemLogging")
    {
      httpsystemlogging_ = attribute.as_bool();

    }
    else if (name == "HttpSupportInformation")
    {
      httpsupportinformation_ = attribute.as_bool();

    }
    else if (name == "StorageConfiguration")
    {
      storageconfiguration_ = attribute.as_bool();

    }
  }
}

std::string SystemCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name + onvif::ToXmlAttribute("DiscoveryResolve", discoveryresolve_) + onvif::ToXmlAttribute("DiscoveryBye", discoverybye_) + onvif::ToXmlAttribute("RemoteDiscovery", remotediscovery_) + onvif::ToXmlAttribute("SystemBackup", systembackup_) + onvif::ToXmlAttribute("SystemLogging", systemlogging_) + onvif::ToXmlAttribute("FirmwareUpgrade", firmwareupgrade_) + onvif::ToXmlAttribute("HttpFirmwareUpgrade", httpfirmwareupgrade_) + onvif::ToXmlAttribute("HttpSystemBackup", httpsystembackup_) + onvif::ToXmlAttribute("HttpSystemLogging", httpsystemlogging_) + onvif::ToXmlAttribute("HttpSupportInformation", httpsupportinformation_) + onvif::ToXmlAttribute("StorageConfiguration", storageconfiguration_) + "/>");
}

Element SystemCapabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (discoveryresolve_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DiscoveryResolve"), ToString(*discoveryresolve_)));

  }

  if (discoverybye_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("DiscoveryBye"), ToString(*discoverybye_)));

  }

  if (remotediscovery_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("RemoteDiscovery"), ToString(*remotediscovery_)));

  }

  if (systembackup_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("SystemBackup"), ToString(*systembackup_)));

  }

  if (systemlogging_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("SystemLogging"), ToString(*systemlogging_)));

  }

  if (firmwareupgrade_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("FirmwareUpgrade"), ToString(*firmwareupgrade_)));

  }

  if (httpfirmwareupgrade_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("HttpFirmwareUpgrade"), ToString(*httpfirmwareupgrade_)));

  }

  if (httpsystembackup_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("HttpSystemBackup"), ToString(*httpsystembackup_)));

  }

  if (httpsystemlogging_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("HttpSystemLogging"), ToString(*httpsystemlogging_)));

  }

  if (httpsupportinformation_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("HttpSupportInformation"), ToString(*httpsupportinformation_)));

  }

  if (storageconfiguration_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("StorageConfiguration"), ToString(*storageconfiguration_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool SystemCapabilities::operator==(const SystemCapabilities& rhs) const
{
  return ((discoveryresolve_ == rhs.discoveryresolve_) && (discoverybye_ == rhs.discoverybye_) && (remotediscovery_ == rhs.remotediscovery_) && (systembackup_ == rhs.systembackup_) && (systemlogging_ == rhs.systemlogging_) && (firmwareupgrade_ == rhs.firmwareupgrade_) && (httpfirmwareupgrade_ == rhs.httpfirmwareupgrade_) && (httpsystembackup_ == rhs.httpsystembackup_) && (httpsystemlogging_ == rhs.httpsystemlogging_) && (httpsupportinformation_ == rhs.httpsupportinformation_) && (storageconfiguration_ == rhs.storageconfiguration_));
}

}

}
