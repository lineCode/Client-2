// ipv4networkinterface.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPv4NetworkInterface::IPv4NetworkInterface()
{

}

IPv4NetworkInterface::IPv4NetworkInterface(const boost::optional<bool>& enabled, const boost::optional<IPv4Configuration>& config) :
  enabled_(enabled),
  config_(config)
{

}

IPv4NetworkInterface::IPv4NetworkInterface(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "Config")
    {
      config_ = IPv4Configuration(element);

    }
  }
}

std::string IPv4NetworkInterface::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Enabled", enabled_) + onvif::ToXmlClass("tt:Config", config_) + "</"+name+">");
}

bool IPv4NetworkInterface::operator==(const IPv4NetworkInterface& rhs) const
{
  return ((enabled_ == rhs.enabled_) && (config_ == rhs.config_));
}

}
