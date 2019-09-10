// networkhost.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkHost::NetworkHost()
{

}

NetworkHost::NetworkHost(const boost::optional<NETWORKHOSTTYPE>& type, const boost::optional<std::string>& ipv4address, const boost::optional<std::string>& ipv6address, const boost::optional<std::string>& dnsname) :
  type_(type),
  ipv4address_(ipv4address),
  ipv6address_(ipv6address),
  dnsname_(dnsname)
{

}

NetworkHost::NetworkHost(NETWORKHOSTTYPE type, const std::string& address) :
  type_(type)
{
  switch (type)
  {
    case onvif::NETWORKHOSTTYPE_IPV4:
    {
      ipv4address_ = address;
      break;
    }
    case onvif::NETWORKHOSTTYPE_IPV6:
    {
      ipv6address_ = address;
      break;
    }
    case onvif::NETWORKHOSTTYPE_DNS:
    {
      dnsname_ = address;
      break;
    }
    default:
    {
      assert(false);
      break;
    }
  }
}

NetworkHost::NetworkHost(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Type")
    {
      type_ = GetNetworkHostType(element.text().get());

    }
    else if (name == "IPv4Address")
    {
      ipv4address_ = element.text().get();

    }
    else if (name == "IPv6Address")
    {
      ipv6address_ = element.text().get();

    }
    else if (name == "DNSname")
    {
      dnsname_ = element.text().get();

    }
  }
}

std::string NetworkHost::GetAddress() const
{
  if (!IsValid())
  {

    return std::string();
  }

  switch (*type_)
  {
    case onvif::NETWORKHOSTTYPE_IPV4:
    {

      return (*ipv4address_);
    }
    case onvif::NETWORKHOSTTYPE_IPV6:
    {

      return (*ipv6address_);
    }
    case onvif::NETWORKHOSTTYPE_DNS:
    {

      return (*dnsname_);
    }
    default:
    {

      return std::string();
    }
  }
}

bool NetworkHost::IsValid() const
{
  if (!type_.is_initialized())
  {

    return false;
  }

  if (*type_ == onvif::NETWORKHOSTTYPE_IPV4 && !ipv4address_.is_initialized())
  {

    return false;
  }
  else if (*type_ == onvif::NETWORKHOSTTYPE_IPV6 && !ipv6address_.is_initialized())
  {

    return false;
  }
  else if (*type_ == onvif::NETWORKHOSTTYPE_DNS && !dnsname_.is_initialized())
  {

    return false;
  }

  return true;
}

std::string NetworkHost::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Type", type_) + onvif::ToXml("tt:IPv4Address", ipv4address_) + onvif::ToXml("tt:IPv6Address", ipv6address_) + onvif::ToXml("tt:DNSname", dnsname_) + "</"+name+">");
}

bool NetworkHost::operator==(const NetworkHost& rhs) const
{
  return ((type_ == rhs.type_) && (ipv4address_ == rhs.ipv4address_) && (ipv6address_ == rhs.ipv6address_) && (dnsname_ == rhs.dnsname_));
}

}
