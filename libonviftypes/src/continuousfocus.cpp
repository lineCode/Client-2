// continuousfocus.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

ContinuousFocus::ContinuousFocus()
{
  
}

ContinuousFocus::ContinuousFocus(const boost::optional<float>& speed) :
  speed_(speed)
{
  
}

ContinuousFocus::ContinuousFocus(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Speed")
    {
      speed_ = element.text().as_float();

    }
  }
}

std::string ContinuousFocus::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Speed", speed_) + "</"+name+">");
}

bool ContinuousFocus::operator==(const ContinuousFocus& rhs) const
{
  return (speed_ == rhs.speed_);
}

}
