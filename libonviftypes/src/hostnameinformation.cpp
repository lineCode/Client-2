// hostnameinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

HostnameInformation::HostnameInformation()
{

}

HostnameInformation::HostnameInformation(const boost::optional<bool>& fromdhcp, const boost::optional<std::string>& name) :
  fromdhcp_(fromdhcp),
  name_(name)
{

}

HostnameInformation::HostnameInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FromDHCP")
    {
      fromdhcp_ = element.text().as_bool();

    }
    else if (name == "Name")
    {
      name_ = element.text().get();

    }
  }
}

std::string HostnameInformation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:FromDHCP", fromdhcp_) + onvif::ToXml("tt:Name", name_) + "</"+name+">");
}

bool HostnameInformation::operator==(const HostnameInformation& rhs) const
{
  return ((fromdhcp_ == rhs.fromdhcp_) && (name_ == rhs.name_));
}

}
