// networkinterface.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkInterface::NetworkInterface()
{

}

NetworkInterface::NetworkInterface(const boost::optional<std::string>& token, const boost::optional<bool>& enabled, const boost::optional<NetworkInterfaceInfo>& info, const boost::optional<NetworkInterfaceLink>& link, const boost::optional<IPv4NetworkInterface>& ipv4, const boost::optional<IPv6NetworkInterface>& ipv6, const boost::optional<Dot11Configuration>& dot11) :
  token_(token),
  enabled_(enabled),
  info_(info),
  link_(link),
  ipv4_(ipv4),
  ipv6_(ipv6),
  dot11_(dot11)
{

}

NetworkInterface::NetworkInterface(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "Info")
    {
      info_ = NetworkInterfaceInfo(element);

    }
    else if (name == "Link")
    {
      link_ = NetworkInterfaceLink(element);

    }
    else if (name == "IPv4")
    {
      ipv4_ = IPv4NetworkInterface(element);

    }
    else if (name == "IPv6")
    {
      ipv6_ = IPv6NetworkInterface(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Dot11")
        {
          dot11_ = Dot11Configuration(extension);

        }
      }
    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }
}

std::string NetworkInterface::ToXml(const std::string& name) const
{
  std::string extension;
  if (dot11_.is_initialized())
  {
    extension = "<tt:Extension>" + ToXmlClass("tt:Dot11", dot11_) + "</tt:Extension>";

  }

  return ("<"+name + ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Enabled", enabled_) + onvif::ToXmlClass("tt:Info", info_) + onvif::ToXmlClass("tt:Link", link_) + onvif::ToXmlClass("tt:IPv4", ipv4_) + onvif::ToXmlClass("tt:IPv6", ipv6_) + extension + "</"+name+">");
}

bool NetworkInterface::operator==(const NetworkInterface& rhs) const
{
  return ((token_ == rhs.token_) && (enabled_ == rhs.enabled_) && (info_ == rhs.info_) && (link_ == rhs.link_) && (ipv4_ == rhs.ipv4_) && (ipv6_ == rhs.ipv6_) && (dot11_ == rhs.dot11_));
}

}
