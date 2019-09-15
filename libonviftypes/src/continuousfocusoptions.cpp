// continuousfocusoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ContinuousFocusOptions::ContinuousFocusOptions()
{
  
}

ContinuousFocusOptions::ContinuousFocusOptions(const boost::optional<FloatRange>& speed) :
  speed_(speed)
{
  
}

ContinuousFocusOptions::ContinuousFocusOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Speed")
    {
      speed_ = FloatRange(element);

    }
  }
}

std::string ContinuousFocusOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Speed", speed_) + "</"+name+">");
}

bool ContinuousFocusOptions::operator==(const ContinuousFocusOptions& rhs) const
{
  return (speed_ == rhs.speed_);
}

}
