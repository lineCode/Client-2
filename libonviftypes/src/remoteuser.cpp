// remoteuser.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RemoteUser::RemoteUser()
{

}

RemoteUser::RemoteUser(const boost::optional<std::string>& username, const boost::optional<std::string>& password, const boost::optional<bool>& usederivedpassword) :
  username_(username),
  password_(password),
  usederivedpassword_(usederivedpassword)
{

}

RemoteUser::RemoteUser(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Username")
    {
      username_ = element.text().get();

    }
    else if (name == "Password")
    {
      password_ = element.text().get();

    }
    else if (name == "UseDerivedPassword")
    {
      usederivedpassword_ = element.text().as_bool();

    }
  }
}

std::string RemoteUser::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Username", username_) + onvif::ToXml("tt:Password", password_) + onvif::ToXml("tt:UseDerivedPassword", usederivedpassword_) + "</"+name+">");
}

bool RemoteUser::operator==(const RemoteUser& rhs) const
{
  return ((username_ == rhs.username_) && (password_ == rhs.password_) && (usederivedpassword_ == rhs.usederivedpassword_));
}

}
