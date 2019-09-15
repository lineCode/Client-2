// imagingstatus20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ImagingStatus20::ImagingStatus20()
{

}

ImagingStatus20::ImagingStatus20(const boost::optional<FocusStatus20>& focusstatus20) :
  focusstatus20_(focusstatus20)
{

}

ImagingStatus20::ImagingStatus20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "FocusStatus20")
    {
      focusstatus20_ = FocusStatus20(element);

    }
  }
}

std::string ImagingStatus20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:FocusStatus20", focusstatus20_) + "</"+name+">");
}

bool ImagingStatus20::operator==(const ImagingStatus20& rhs) const
{
  return (focusstatus20_ == rhs.focusstatus20_);
}

}
