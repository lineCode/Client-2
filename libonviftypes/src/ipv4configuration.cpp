// ipv4configuration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPv4Configuration::IPv4Configuration()
{

}

IPv4Configuration::IPv4Configuration(const std::vector<PrefixedIPv4Address>& manual, const boost::optional<PrefixedIPv4Address>& linklocal, const boost::optional<PrefixedIPv4Address>& fromdhcp, const boost::optional<bool>& dhcp) :
  manual_(manual),
  linklocal_(linklocal),
  fromdhcp_(fromdhcp),
  dhcp_(dhcp)
{

}

IPv4Configuration::IPv4Configuration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Manual")
    {
      manual_.push_back(PrefixedIPv4Address(element));

    }
    else if (name == "LinkLocal")
    {
      linklocal_ = PrefixedIPv4Address(element);

    }
    else if (name == "FromDHCP")
    {
      fromdhcp_ = PrefixedIPv4Address(element);

    }
    else if (name == "DHCP")
    {
      dhcp_ = element.text().as_bool();

    }
  }
}

std::string IPv4Configuration::ToXml(const std::string& name) const
{
  std::string manuals;
  for (const auto manual : manual_)
  {
    manuals += manual.ToXml("tt:Manual");
  
  }

  return ("<"+name+">" + manuals + onvif::ToXmlClass("tt:LinkLocal", linklocal_) + onvif::ToXmlClass("tt:FromDHCP", fromdhcp_) + onvif::ToXml("tt:DHCP", dhcp_) + "</"+name+">");
}

bool IPv4Configuration::operator==(const IPv4Configuration& rhs) const
{
  return (std::is_permutation(manual_.begin(), manual_.end(), rhs.manual_.begin(), rhs.manual_.end()) && (linklocal_ == rhs.linklocal_) && (fromdhcp_ == rhs.fromdhcp_) && (dhcp_ == rhs.dhcp_));
}

}
