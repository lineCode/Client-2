// ipaddressfilter.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

IPAddressFilter::IPAddressFilter()
{

}

IPAddressFilter::IPAddressFilter(const boost::optional<IPADDRESSFILTERTYPE>& type, const std::vector<PrefixedIPv4Address>& ipv4address, const std::vector<PrefixedIPv6Address>& ipv6address) :
  type_(type),
  ipv4address_(ipv4address),
  ipv6address_(ipv6address)
{

}

IPAddressFilter::IPAddressFilter(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Type")
    {
      type_ = GetIPAddressFilterType(element.text().get());

    }
    else if (name == "IPv4Address")
    {
      ipv4address_.push_back(PrefixedIPv4Address(element));

    }
    else if (name == "IPv6Address")
    {
      ipv6address_.push_back(PrefixedIPv6Address(element));

    }
  }
}

std::string IPAddressFilter::ToXml(const std::string& name) const
{
  std::string ipv4addresses;
  for (const auto ipv4address : ipv4address_)
  {
    ipv4addresses += ipv4address.ToXml("tt:IPv4Address");
  
  }

  std::string ipv6addresses;
  for (const auto ipv6address : ipv6address_)
  {
    ipv6addresses += ipv6address.ToXml("tt:IPv6Address");
  
  }

  return ("<"+name+">" + onvif::ToXmlEnum("tt:Type", type_) + ipv4addresses + ipv6addresses + "</"+name+">");
}

bool IPAddressFilter::operator==(const IPAddressFilter& rhs) const
{
  return ((type_ == rhs.type_) && std::is_permutation(ipv4address_.begin(), ipv4address_.end(), rhs.ipv4address_.begin(), rhs.ipv4address_.end()) && std::is_permutation(ipv6address_.begin(), ipv6address_.end(), rhs.ipv6address_.begin(), rhs.ipv6address_.end()));
}

}
