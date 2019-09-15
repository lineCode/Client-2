// h264configuration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

H264Configuration::H264Configuration()
{

}

H264Configuration::H264Configuration(const boost::optional<int>& govlength, const boost::optional<H264PROFILE>& h264profile) :
  govlength_(govlength),
  h264profile_(h264profile)
{

}

H264Configuration::H264Configuration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "GovLength")
    {
      govlength_ = element.text().as_int();

    }
    else if (name == "H264Profile")
    {
      h264profile_ = GetH264Profile(element.text().get());

    }
  }
}

std::string H264Configuration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:GovLength", govlength_) + onvif::ToXmlEnum("tt:H264Profile", h264profile_) + "</"+name+">");
}

bool H264Configuration::operator==(const H264Configuration& rhs) const
{
  return ((govlength_ == rhs.govlength_) && (h264profile_ == rhs.h264profile_));
}

}
