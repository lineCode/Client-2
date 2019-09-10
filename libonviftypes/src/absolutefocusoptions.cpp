// absolutefocusoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AbsoluteFocusOptions::AbsoluteFocusOptions()
{
  
}

AbsoluteFocusOptions::AbsoluteFocusOptions(const boost::optional<FloatRange>& position, const boost::optional<FloatRange>& speed) :
  position_(position),
  speed_(speed)
{
  
}

AbsoluteFocusOptions::AbsoluteFocusOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Position")
    {
      position_ = FloatRange(element);

    }
    else if (name == "Speed")
    {
      speed_ = FloatRange(element);

    }
  }
}

std::string AbsoluteFocusOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Position", position_) + ToXmlClass("tt:Speed", speed_) + "</"+name+">");
}

bool AbsoluteFocusOptions::operator==(const AbsoluteFocusOptions& rhs) const
{
  return ((position_ == rhs.position_) && (speed_ == rhs.speed_));
}

}
