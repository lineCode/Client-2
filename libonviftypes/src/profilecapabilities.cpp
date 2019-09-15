// profilecapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ProfileCapabilities::ProfileCapabilities()
{

}

ProfileCapabilities::ProfileCapabilities(const boost::optional<int>& maximumnumberofprofiles) :
  maximumnumberofprofiles_(maximumnumberofprofiles)
{

}

ProfileCapabilities::ProfileCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "MaximumNumberOfProfiles")
    {
      maximumnumberofprofiles_ = element.text().as_int();

    }
  }
}

std::string ProfileCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:MaximumNumberOfProfiles", maximumnumberofprofiles_) + "</"+name+">");
}

bool ProfileCapabilities::operator==(const ProfileCapabilities& rhs) const
{
  return (maximumnumberofprofiles_ == rhs.maximumnumberofprofiles_);
}

}
