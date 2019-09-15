// relativefocus.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

RelativeFocus::RelativeFocus()
{
  
}

RelativeFocus::RelativeFocus(const boost::optional<float>& distance, const boost::optional<float>& speed) :
  distance_(distance),
  speed_(speed)
{
  
}

RelativeFocus::RelativeFocus(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Distance")
    {
      distance_ = element.text().as_float();

    }
    else if (name == "Speed")
    {
      speed_ = element.text().as_float();

    }
  }
}

std::string RelativeFocus::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Distance", distance_) + onvif::ToXml("tt:Speed", speed_) + "</"+name+">");
}

bool RelativeFocus::operator==(const RelativeFocus& rhs) const
{
  return ((distance_ == rhs.distance_) && (speed_ == rhs.speed_));
}

}
