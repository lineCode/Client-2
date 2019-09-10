// multicastconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

MulticastConfiguration::MulticastConfiguration()
{

}

MulticastConfiguration::MulticastConfiguration(const boost::optional<IPAddress>& address, const boost::optional<int>& port, const boost::optional<int>& ttl, const boost::optional<bool>& autostart) :
  address_(address),
  port_(port),
  ttl_(ttl),
  autostart_(autostart)
{

}

MulticastConfiguration::MulticastConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Address")
    {
      address_ = IPAddress(element);

    }
    else if (name == "Port")
    {
      port_ = element.text().as_int();

    }
    else if (name == "TTL")
    {
      ttl_ = element.text().as_int();

    }
    else if (name == "AutoStart")
    {
      autostart_ = element.text().as_bool();

    }
  }
}

std::string MulticastConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Address", address_) + onvif::ToXml("tt:Port", port_) + onvif::ToXml("tt:TTL", ttl_) + onvif::ToXml("tt:AutoStart", autostart_) + "</"+name+">");
}

bool MulticastConfiguration::operator==(const MulticastConfiguration& rhs) const
{
  return ((address_ == rhs.address_) && (port_ == rhs.port_) && (ttl_ == rhs.ttl_) && (autostart_ == rhs.autostart_));
}

}
