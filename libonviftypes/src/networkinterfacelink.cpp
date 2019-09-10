// networkinterfacelink.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkInterfaceLink::NetworkInterfaceLink()
{

}

NetworkInterfaceLink::NetworkInterfaceLink(const boost::optional<NetworkInterfaceConnectionSetting> adminsettings, const boost::optional<NetworkInterfaceConnectionSetting>& opersettings, const boost::optional<int>& interfacetype) :
  adminsettings_(adminsettings),
  opersettings_(opersettings),
  interfacetype_(interfacetype)
{

}

NetworkInterfaceLink::NetworkInterfaceLink(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AdminSettings")
    {
      adminsettings_ = NetworkInterfaceConnectionSetting(element);

    }
    else if (name == "OperSettings")
    {
      opersettings_ = NetworkInterfaceConnectionSetting(element);

    }
    else if (name == "InterfaceType")
    {
      interfacetype_ = element.text().as_int();

    }
  }
}

std::string NetworkInterfaceLink::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:AdminSettings", adminsettings_) + onvif::ToXmlClass("tt:OperSettings", opersettings_) + onvif::ToXml("tt:InterfaceType", interfacetype_) + "</"+name+">");
}

bool NetworkInterfaceLink::operator==(const NetworkInterfaceLink& rhs) const
{
  return ((adminsettings_ == rhs.adminsettings_) && (opersettings_ == rhs.opersettings_) && (interfacetype_ == rhs.interfacetype_));
}

}
