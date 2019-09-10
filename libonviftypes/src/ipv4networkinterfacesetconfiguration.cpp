// ipv4networkinterfacesetconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPv4NetworkInterfaceSetConfiguration::IPv4NetworkInterfaceSetConfiguration()
{

}

IPv4NetworkInterfaceSetConfiguration::IPv4NetworkInterfaceSetConfiguration(const boost::optional<bool>& enabled, const std::vector<PrefixedIPv4Address>& manual, const boost::optional<bool>& dhcp) :
  enabled_(enabled),
  manual_(manual),
  dhcp_(dhcp)
{

}

IPv4NetworkInterfaceSetConfiguration::IPv4NetworkInterfaceSetConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "Manual")
    {
      manual_.push_back(PrefixedIPv4Address(element));

    }
    else if (name == "DHCP")
    {
      dhcp_ = element.text().as_bool();

    }
  }
}

std::string IPv4NetworkInterfaceSetConfiguration::ToXml(const std::string& name) const
{
  std::string manuals;
  for (const auto& manual : manual_)
  {
    manuals += manual.ToXml("tt:Manual");

  }
  
  return ("<"+name+">" + onvif::ToXml("tt:Enabled", enabled_) + manuals + onvif::ToXml("tt:DHCP", dhcp_) + "</"+name+">");
}

bool IPv4NetworkInterfaceSetConfiguration::operator==(const IPv4NetworkInterfaceSetConfiguration& rhs) const
{
  return ((enabled_ == rhs.enabled_) && std::is_permutation(manual_.begin(), manual_.end(), rhs.manual_.begin(), rhs.manual_.end()) && (dhcp_ == rhs.dhcp_));
}

}
