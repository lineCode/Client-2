// networkgateway.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkGateway::NetworkGateway()
{

}

NetworkGateway::NetworkGateway(const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address) :
  ipv4address_(ipv4address),
  ipv6address_(ipv6address)
{

}

NetworkGateway::NetworkGateway(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "IPv4Address")
    {
      ipv4address_.push_back(element.text().get());

    }
    else if (name == "IPv6Address")
    {
      ipv6address_.push_back(element.text().get());

    }
  }
}

std::string NetworkGateway::ToXml(const std::string& name) const
{
  std::string ipv4address;
  for (const auto& address : ipv4address_)
  {
    ipv4address += onvif::ToXml("tt:IPv4Address", boost::optional<std::string>(address));

  }
  
  std::string ipv6address;
  for (const auto& address : ipv6address_)
  {
    ipv6address += onvif::ToXml("tt:IPv6Address", boost::optional<std::string>(address));

  }
  
  return ("<"+name+">" + ipv4address + ipv6address + "</"+name+">");
}

bool NetworkGateway::operator==(const NetworkGateway& rhs) const
{
  return (std::is_permutation(ipv4address_.begin(), ipv4address_.end(), rhs.ipv4address_.begin(), rhs.ipv4address_.end()) && std::is_permutation(ipv6address_.begin(), ipv6address_.end(), rhs.ipv6address_.begin(), rhs.ipv6address_.end()));
}

}
