// ipv6networkinterfacesetconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPv6NetworkInterfaceSetConfiguration::IPv6NetworkInterfaceSetConfiguration()
{

}

IPv6NetworkInterfaceSetConfiguration::IPv6NetworkInterfaceSetConfiguration(const boost::optional<bool>& enabled, const boost::optional<bool>& acceptrouteradvert, const std::vector<PrefixedIPv6Address>& manual, const boost::optional<IPV6DHCPCONFIGURATION>& dhcp) :
  enabled_(enabled),
  acceptrouteradvert_(acceptrouteradvert),
  manual_(manual),
  dhcp_(dhcp)
{

}

IPv6NetworkInterfaceSetConfiguration::IPv6NetworkInterfaceSetConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "AcceptRouterAdvert")
    {
      acceptrouteradvert_ = element.text().as_bool();

    }
    else if (name == "Manual")
    {
      manual_.push_back(PrefixedIPv6Address(element));

    }
    else if (name == "DHCP")
    {
      dhcp_ = GetIPv6DHCPConfiguration(element.text().get());

    }
  }
}

std::string IPv6NetworkInterfaceSetConfiguration::ToXml(const std::string& name) const
{
  std::string manuals;
  for (const auto& manual : manual_)
  {
    manuals += manual.ToXml("tt:Manual");

  }
  
  return ("<"+name+">" + onvif::ToXml("tt:Enabled", enabled_) + onvif::ToXml("tt:AcceptRouterAdvert", acceptrouteradvert_) + manuals + onvif::ToXmlEnum("tt:DHCP", dhcp_) + "</"+name+">");
}

bool IPv6NetworkInterfaceSetConfiguration::operator==(const IPv6NetworkInterfaceSetConfiguration& rhs) const
{
  return ((enabled_ == rhs.enabled_) && (acceptrouteradvert_ == rhs.acceptrouteradvert_) && std::is_permutation(manual_.begin(), manual_.end(), rhs.manual_.begin(), rhs.manual_.end()) && (dhcp_ == rhs.dhcp_));
}

}
