// eapmethodconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

EAPMethodConfiguration::EAPMethodConfiguration()
{

}

EAPMethodConfiguration::EAPMethodConfiguration(const boost::optional<TLSConfiguration>& tlsconfiguration, const boost::optional<std::string>& password) :
  tlsconfiguration_(tlsconfiguration),
  password_(password)
{

}

EAPMethodConfiguration::EAPMethodConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TLSConfigurations")
    {
      tlsconfiguration_ = TLSConfiguration(element);

    }
    else if (name == "Password")
    {
      password_ = element.text().get();

    }
  }
}

std::string EAPMethodConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:TLSConfigurations", tlsconfiguration_) + onvif::ToXml("tt:Password", password_) + "</"+name+">");
}

bool EAPMethodConfiguration::operator==(const EAPMethodConfiguration& rhs) const
{
  return ((tlsconfiguration_ == rhs.tlsconfiguration_) && (password_ == rhs.password_));
}

}
