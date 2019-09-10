// networkprotocol.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkProtocol::NetworkProtocol()
{

}

NetworkProtocol::NetworkProtocol(const boost::optional<NETWORKPROTOCOLTYPE>& name, const boost::optional<bool>& enabled, const std::vector<int>& port) :
  name_(name),
  enabled_(enabled),
  port_(port)
{

}

NetworkProtocol::NetworkProtocol(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = GetNetworkProtocolType(element.text().get());

    }
    else if (name == "Enabled")
    {
      enabled_ = element.text().as_bool();

    }
    else if (name == "Port")
    {
      port_.push_back(element.text().as_int());

    }
  }
}

std::string NetworkProtocol::ToXml(const std::string& name) const
{
  std::string ports;
  for (int port : port_)
  {
    ports += onvif::ToXml("tt:Port", boost::optional<int>(port));
  
  }

  return ("<"+name+">" + onvif::ToXmlEnum("tt:Name", name_) + onvif::ToXml("tt:Enabled", enabled_) + ports + "</"+name+">");
}

bool NetworkProtocol::operator==(const NetworkProtocol& rhs) const
{
  return ((name_ == rhs.name_) && (enabled_ == rhs.enabled_) && std::is_permutation(port_.begin(), port_.end(), rhs.port_.begin(), rhs.port_.end()));
}

}
