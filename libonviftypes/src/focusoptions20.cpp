// focusoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

FocusOptions20::FocusOptions20()
{

}

FocusOptions20::FocusOptions20(const std::vector<AUTOFOCUSMODE>& autofocusmodes, const boost::optional<FloatRange>& defaultspeed, const boost::optional<FloatRange>& nearlimit, const boost::optional<FloatRange>& farlimit) :
  autofocusmodes_(autofocusmodes),
  defaultspeed_(defaultspeed),
  nearlimit_(nearlimit),
  farlimit_(farlimit)
{

}

FocusOptions20::FocusOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AutoFocusModes")
    {
      boost::optional<AUTOFOCUSMODE> autofocusmode = GetAutoFocusMode(element.text().get());
      if (autofocusmode.is_initialized())
      {
        autofocusmodes_.push_back(*autofocusmode);

      }
    }
    else if (name == "DefaultSpeed")
    {
      defaultspeed_ = FloatRange(element);

    }
    else if (name == "NearLimit")
    {
      nearlimit_ = FloatRange(element);

    }
    else if (name == "FarLimit")
    {
      farlimit_ = FloatRange(element);

    }
  }
}

std::string FocusOptions20::ToXml(const std::string& name) const
{
  std::string autofocusmodes;
  for (const auto& autofocusmode : autofocusmodes_)
  {
    autofocusmodes += onvif::ToXmlEnum("tt:AutoFocusModes", autofocusmode);

  }
  
  return ("<"+name+">" + autofocusmodes + onvif::ToXmlClass("tt:DefaultSpeed", defaultspeed_) + onvif::ToXmlClass("tt:NearLimit", nearlimit_) + onvif::ToXmlClass("tt:FarLimit", farlimit_) + "</"+name+">");
}

bool FocusOptions20::operator==(const FocusOptions20& rhs) const
{
  return (std::is_permutation(autofocusmodes_.begin(), autofocusmodes_.end(), rhs.autofocusmodes_.begin(), rhs.autofocusmodes_.end()) && (defaultspeed_ == rhs.defaultspeed_) && (nearlimit_ == rhs.nearlimit_) && (farlimit_ == rhs.farlimit_));
}

}
