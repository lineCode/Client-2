// networkinterfacesetconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkInterfaceSetConfiguration::NetworkInterfaceSetConfiguration()
{

}

NetworkInterfaceSetConfiguration::NetworkInterfaceSetConfiguration(const boost::optional<bool>& enabled, const boost::optional<NetworkInterfaceConnectionSetting>& link, const boost::optional<int>& mtu, const boost::optional<IPv4NetworkInterfaceSetConfiguration>& ipv4, const boost::optional<IPv6NetworkInterfaceSetConfiguration>& ipv6, const std::vector<Dot11Configuration>& dot11) :
  enabled_(enabled),
  link_(link),
  mtu_(mtu),
  ipv4_(ipv4),
  ipv6_(ipv6),
  dot11_(dot11)
{

}

NetworkInterfaceSetConfiguration::NetworkInterfaceSetConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "Link")
    {
      link_ = NetworkInterfaceConnectionSetting(element);

    }
    else if (name == "MTU")
    {
      mtu_ = element.text().as_int();

    }
    else if (name == "IPv4")
    {
      ipv4_ = IPv4NetworkInterfaceSetConfiguration(element);

    }
    else if (name == "IPv6")
    {
      ipv6_ = IPv6NetworkInterfaceSetConfiguration(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Dot11")
        {
          dot11_.push_back(Dot11Configuration(extension));

        }
      }
    }
  }
}

std::string NetworkInterfaceSetConfiguration::ToXml(const std::string& name) const
{
  std::string dot11s;
  for (const auto& dot11 : dot11_)
  {
    dot11s += dot11.ToXml("tt:Dot11");

  }
  
  return ("<"+name+">" + onvif::ToXml("tt:Enabled", enabled_) + onvif::ToXmlClass("tt:Link", link_) + onvif::ToXml("tt:MTU", mtu_) + onvif::ToXmlClass("tt:IPv4", ipv4_) + onvif::ToXmlClass("tt:IPv6", ipv6_) + std::string("<tt:Extension>") + dot11s + "</tt:Extension></"+name+">");
}

bool NetworkInterfaceSetConfiguration::operator==(const NetworkInterfaceSetConfiguration& rhs) const
{
  return ((enabled_ == rhs.enabled_) && (link_ == rhs.link_) && (mtu_ == rhs.mtu_) && (ipv4_ == rhs.ipv4_) && (ipv6_ == rhs.ipv6_) && std::is_permutation(dot11_.begin(), dot11_.end(), rhs.dot11_.begin(), rhs.dot11_.end()));
}

}
