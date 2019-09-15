// prefixedipv6address.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

PrefixedIPv6Address::PrefixedIPv6Address()
{

}

PrefixedIPv6Address::PrefixedIPv6Address(const boost::optional<std::string>& address, const boost::optional<int>& prefixlength) :
  address_(address),
  prefixlength_(prefixlength)
{

}

PrefixedIPv6Address::PrefixedIPv6Address(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Address")
    {
      address_ = element.text().get();

    }
    else if (name == "PrefixLength")
    {
      prefixlength_ = element.text().as_int();

    }
  }
}

std::string PrefixedIPv6Address::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Address", address_) + onvif::ToXml("tt:PrefixLength", prefixlength_) + "</"+name+">");
}

bool PrefixedIPv6Address::operator==(const PrefixedIPv6Address& rhs) const
{
  return ((address_ == rhs.address_) && (prefixlength_ == rhs.prefixlength_));
}

}
