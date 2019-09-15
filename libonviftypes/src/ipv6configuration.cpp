// ipv6configuration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPv6Configuration::IPv6Configuration()
{

}

IPv6Configuration::IPv6Configuration(const boost::optional<bool>& acceptrouteradvert, const boost::optional<IPV6DHCPCONFIGURATION>& dhcp, const std::vector<PrefixedIPv6Address>& manual, const std::vector<PrefixedIPv6Address>& linklocal, const std::vector<PrefixedIPv6Address>& fromdhcp, const std::vector<PrefixedIPv6Address>& fromra) :
  acceptrouteradvert_(acceptrouteradvert),
  dhcp_(dhcp),
  manual_(manual),
  linklocal_(linklocal),
  fromdhcp_(fromdhcp),
  fromra_(fromra)
{

}

IPv6Configuration::IPv6Configuration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AcceptRouterAdvert")
    {
      acceptrouteradvert_ = element.text().as_bool();

    }
    else if (name == "DHCP")
    {
      dhcp_ = GetIPv6DHCPConfiguration(element.text().get());

    }
    else if (name == "Manual")
    {
      manual_.push_back(PrefixedIPv6Address(element));

    }
    else if (name == "LinkLocal")
    {
      linklocal_.push_back(PrefixedIPv6Address(element));

    }
    else if (name == "FromDHCP")
    {
      fromdhcp_.push_back(PrefixedIPv6Address(element));

    }
    else if (name == "FromRA")
    {
      fromra_.push_back(PrefixedIPv6Address(element));

    }
  }
}

std::string IPv6Configuration::ToXml(const std::string& name) const
{
  std::string manuals;
  for (const auto manual : manual_)
  {
    manuals += manual.ToXml("tt:Manual");
  
  }

  std::string linklocals;
  for (const auto linklocal : linklocal_)
  {
    linklocals += linklocal.ToXml("tt:LinkLocal");
  
  }

  std::string fromdhcps;
  for (const auto fromdhcp : fromdhcp_)
  {
    fromdhcps += fromdhcp.ToXml("tt:FromDHCP");
  
  }

  std::string fromras;
  for (const auto fromra : fromra_)
  {
    fromras += fromra.ToXml("tt:FromRA");
  
  }

  return ("<"+name+">" + onvif::ToXml("tt:AcceptRouterAdvert", acceptrouteradvert_) + onvif::ToXmlEnum("tt:DHCP", dhcp_) + manuals + linklocals + fromdhcps + fromras + "</"+name+">");
}

bool IPv6Configuration::operator==(const IPv6Configuration& rhs) const
{
  return ((acceptrouteradvert_ == rhs.acceptrouteradvert_) && (dhcp_ == rhs.dhcp_) && std::is_permutation(manual_.begin(), manual_.end(), rhs.manual_.begin(), rhs.manual_.end()) && std::is_permutation(linklocal_.begin(), linklocal_.end(), rhs.linklocal_.begin(), rhs.linklocal_.end()) && std::is_permutation(fromdhcp_.begin(), fromdhcp_.end(), rhs.fromdhcp_.begin(), rhs.fromdhcp_.end()) && std::is_permutation(fromra_.begin(), fromra_.end(), rhs.fromra_.begin(), rhs.fromra_.end()));
}

}
