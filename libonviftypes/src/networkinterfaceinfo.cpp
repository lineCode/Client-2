// networkinterfaceinfo.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

NetworkInterfaceInfo::NetworkInterfaceInfo()
{

}

NetworkInterfaceInfo::NetworkInterfaceInfo(const boost::optional<std::string>& name, const boost::optional<std::string>& hwaddress, const boost::optional<int>& mtu) :
  name_(name),
  hwaddress_(hwaddress),
  mtu_(mtu)
{

}

NetworkInterfaceInfo::NetworkInterfaceInfo(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "HwAddress")
    {
      hwaddress_ = element.text().get();

    }
    else if (name == "MTU")
    {
      mtu_ = element.text().as_int();

    }
  }
}

std::string NetworkInterfaceInfo::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:HwAddress", hwaddress_) + onvif::ToXml("tt:MTU", mtu_) + "</"+name+">");
}

bool NetworkInterfaceInfo::operator==(const NetworkInterfaceInfo& rhs) const
{
  return ((name_ == rhs.name_) && (hwaddress_ == rhs.hwaddress_) && (mtu_ == rhs.mtu_));
}

}
