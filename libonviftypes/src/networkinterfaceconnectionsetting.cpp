// networkinterfaceconnectionsetting.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkInterfaceConnectionSetting::NetworkInterfaceConnectionSetting()
{

}

NetworkInterfaceConnectionSetting::NetworkInterfaceConnectionSetting(const boost::optional<bool>& autonegotiation, const boost::optional<int>& speed, const boost::optional<DUPLEX>& duplex) :
  autonegotiation_(autonegotiation),
  speed_(speed),
  duplex_(duplex)
{

}

NetworkInterfaceConnectionSetting::NetworkInterfaceConnectionSetting(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AutoNegotiation")
    {
      autonegotiation_ = element.text().as_bool();

    }
    else if (name == "Speed")
    {
      speed_ = element.text().as_int();

    }
    else if (name == "Duplex")
    {
      duplex_ = GetDuplex(element.text().get());

    }
  }
}

std::string NetworkInterfaceConnectionSetting::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:AutoNegotiation", autonegotiation_) + onvif::ToXml("tt:Speed", speed_) + onvif::ToXmlEnum("tt:Duplex", duplex_) + "</"+name+">");
}

bool NetworkInterfaceConnectionSetting::operator==(const NetworkInterfaceConnectionSetting& rhs) const
{
  return ((autonegotiation_ == rhs.autonegotiation_) && (speed_ == rhs.speed_) && (duplex_ == rhs.duplex_));
}

}
