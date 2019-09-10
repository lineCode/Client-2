// mpeg4configuration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

Mpeg4Configuration::Mpeg4Configuration()
{

}

Mpeg4Configuration::Mpeg4Configuration(const boost::optional<int>& govlength, const boost::optional<MPEG4PROFILE>& mpeg4profile) :
  govlength_(govlength),
  mpeg4profile_(mpeg4profile)
{

}

Mpeg4Configuration::Mpeg4Configuration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "GovLength")
    {
      govlength_ = element.text().as_int();

    }
    else if (name == "Mpeg4Profile")
    {
      mpeg4profile_ = GetMpeg4Profile(element.text().get());

    }
  }
}

std::string Mpeg4Configuration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:GovLength", govlength_) + onvif::ToXmlEnum("tt:Mpeg4Profile", mpeg4profile_) + "</"+name+">");
}

bool Mpeg4Configuration::operator==(const Mpeg4Configuration& rhs) const
{
  return ((govlength_ == rhs.govlength_) && (mpeg4profile_ == rhs.mpeg4profile_));
}

}
