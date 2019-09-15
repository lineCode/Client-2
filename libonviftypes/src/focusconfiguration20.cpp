// focusconfiguration20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

FocusConfiguration20::FocusConfiguration20()
{

}

FocusConfiguration20::FocusConfiguration20(const boost::optional<AUTOFOCUSMODE>& autofocusmode, const boost::optional<float>& defaultspeed, const boost::optional<float>& nearlimit, const boost::optional<float>& farlimit) :
  autofocusmode_(autofocusmode),
  defaultspeed_(defaultspeed),
  nearlimit_(nearlimit),
  farlimit_(farlimit)
{

}

FocusConfiguration20::FocusConfiguration20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AutoFocusMode")
    {
      autofocusmode_ = GetAutoFocusMode(element.text().get());

    }
    else if (name == "DefaultSpeed")
    {
      defaultspeed_ = element.text().as_float();

    }
    else if (name == "NearLimit")
    {
      nearlimit_ = element.text().as_float();

    }
    else if (name == "FarLimit")
    {
      farlimit_ = element.text().as_float();

    }
  }
}

std::string FocusConfiguration20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:AutoFocusMode", autofocusmode_) + onvif::ToXml("tt:DefaultSpeed", defaultspeed_) + onvif::ToXml("tt:NearLimit", nearlimit_) + onvif::ToXml("tt:FarLimit", farlimit_) + "</"+name+">");
}

bool FocusConfiguration20::operator==(const FocusConfiguration20& rhs) const
{
  return ((autofocusmode_ == rhs.autofocusmode_) && (defaultspeed_ == rhs.defaultspeed_) && (nearlimit_ == rhs.nearlimit_) && (farlimit_ == rhs.farlimit_));
}

}
