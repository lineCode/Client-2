// ipv6networkinterface.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPv6NetworkInterface::IPv6NetworkInterface()
{

}

IPv6NetworkInterface::IPv6NetworkInterface(const boost::optional<bool>& enabled, const boost::optional<IPv6Configuration>& config) :
  enabled_(enabled),
  config_(config)
{

}

IPv6NetworkInterface::IPv6NetworkInterface(const pugi::xml_node& node)
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
      config_ = IPv6Configuration(element);

    }
  }
}

std::string IPv6NetworkInterface::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Enabled", enabled_) + onvif::ToXmlClass("tt:Config", config_) + "</"+name+">");
}

bool IPv6NetworkInterface::operator==(const IPv6NetworkInterface& rhs) const
{
  return ((enabled_ == rhs.enabled_) && (config_ == rhs.config_));
}

}
