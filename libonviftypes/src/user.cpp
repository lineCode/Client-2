// user.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

User::User()
{

}

User::User(const boost::optional<std::string>& username, const boost::optional<std::string>& password, const boost::optional<USERLEVEL>& userlevel) :
  username_(username),
  password_(password),
  userlevel_(userlevel)
{

}

User::User(const pugi::xml_node& node)
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
    else if (name == "UserLevel")
    {
      userlevel_ = GetUserLevel(element.text().get());

    }
  }
}

void User::Clear()
{
  username_.reset();
  password_.reset();
  userlevel_.reset();
}

std::string User::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Username", username_) + onvif::ToXml("tt:Password", password_) + onvif::ToXmlEnum("tt:UserLevel", userlevel_) + "</"+name+">");
}

bool User::operator==(const User& rhs) const
{
  return ((username_ == rhs.username_) && (password_ == rhs.password_) && (userlevel_ == rhs.userlevel_));
}

}
