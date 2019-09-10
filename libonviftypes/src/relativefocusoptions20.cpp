// relativefocusoptions20.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

RelativeFocusOptions20::RelativeFocusOptions20()
{
  
}

RelativeFocusOptions20::RelativeFocusOptions20(const boost::optional<FloatRange>& distance, const boost::optional<FloatRange>& speed) :
  distance_(distance),
  speed_(speed)
{
  
}

RelativeFocusOptions20::RelativeFocusOptions20(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Distance")
    {
      distance_ = FloatRange(element);

    }
    else if (name == "Speed")
    {
      speed_ = FloatRange(element);

    }
  }
}

std::string RelativeFocusOptions20::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Distance", distance_) + ToXmlClass("tt:Speed", speed_) + "</"+name+">");
}

bool RelativeFocusOptions20::operator==(const RelativeFocusOptions20& rhs) const
{
  return ((distance_ == rhs.distance_) && (speed_ == rhs.speed_));
}

}
