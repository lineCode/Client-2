// absolutefocus.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AbsoluteFocus::AbsoluteFocus()
{
  
}

AbsoluteFocus::AbsoluteFocus(const boost::optional<float>& position, const boost::optional<float>& speed) :
  position_(position),
  speed_(speed)
{
  
}

AbsoluteFocus::AbsoluteFocus(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Position")
    {
      position_ = element.text().as_float();

    }
    else if (name == "Speed")
    {
      speed_ = element.text().as_float();

    }
  }
}

std::string AbsoluteFocus::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Position", position_) + onvif::ToXml("tt:Speed", speed_) + "</"+name+">");
}

bool AbsoluteFocus::operator==(const AbsoluteFocus& rhs) const
{
  return ((position_ == rhs.position_) && (speed_ == rhs.speed_));
}

}
