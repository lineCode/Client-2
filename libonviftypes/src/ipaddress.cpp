// ipaddress.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPAddress::IPAddress()
{

}

IPAddress::IPAddress(const boost::optional<IPTYPE>& type, const boost::optional<std::string>& ipv4address, const boost::optional<std::string>& ipv6address) :
  type_(type),
  ipv4address_(ipv4address),
  ipv6address_(ipv6address)
{

}

IPAddress::IPAddress(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Type")
    {
      type_ = GetIpType(element.text().get());

    }
    else if (name == "IPv4Address")
    {
      ipv4address_ = element.text().get();

    }
    else if (name == "IPv6Address")
    {
      ipv6address_ = element.text().get();

    }
  }
}

std::string IPAddress::GetAddress() const
{
  if (!IsValid())
  {

    return std::string();
  }

  switch (*type_)
  {
    case onvif::IPTYPE_IPV4:
    {

      return (*ipv4address_);
    }
    case onvif::IPTYPE_IPV6:
    {

      return (*ipv6address_);
    }
    default:
    {

      return std::string();
    }
  }
}

bool IPAddress::IsValid() const
{
  if (!type_.is_initialized())
  {

    return false;
  }

  if (*type_ == onvif::IPTYPE_IPV4 && !ipv4address_.is_initialized())
  {

    return false;
  }
  else if (*type_ == onvif::IPTYPE_IPV6 && !ipv6address_.is_initialized())
  {

    return false;
  }

  return true;
}

std::string IPAddress::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Type", type_) + onvif::ToXml("tt:IPv4Address", ipv4address_) + onvif::ToXml("tt:IPv6Address", ipv6address_) + "</"+name+">");
}

bool IPAddress::operator==(const IPAddress& rhs) const
{
  return ((type_ == rhs.type_) && (ipv4address_ == rhs.ipv4address_) && (ipv6address_ == rhs.ipv6address_));
}

}
