// usercredential.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

UserCredential::UserCredential()
{

}

UserCredential::UserCredential(const boost::optional<std::string>& username, const boost::optional<std::string>& password) :
  username_(username),
  password_(password)
{

}

UserCredential::UserCredential(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "UserName")
    {
      username_ = element.text().get();

    }
    else if (name == "Password")
    {
      password_ = element.text().get();

    }
  }
}

std::string UserCredential::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:UserName", username_) + onvif::ToXml("tt:Password", password_) + "</"+name+">");
}

bool UserCredential::operator==(const UserCredential& rhs) const
{
  return ((username_ == rhs.username_) && (password_ == rhs.password_));
}

}
