// defaultptzspeed.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DefaultPTZSpeed::DefaultPTZSpeed()
{

}

DefaultPTZSpeed::DefaultPTZSpeed(const boost::optional<Vector2d>& pantilt, const boost::optional<int>& zoom) :
  pantilt_(pantilt),
  zoom_(zoom)
{

}

DefaultPTZSpeed::DefaultPTZSpeed(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "PanTilt")
    {
      pantilt_ = Vector2d(element);

    }
    else if (name == "Zoom")
    {
      auto attribute = element.attribute("x");
      if (attribute)
      {
        zoom_ = element.attribute("x").as_int();

      }
    }
  }
}

std::string DefaultPTZSpeed::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:PanTilt", pantilt_) + "<tt:Zoom" + ToXmlAttribute("x", zoom_) + "></tt:Zoom></"+name+">");
}

bool DefaultPTZSpeed::operator==(const DefaultPTZSpeed& rhs) const
{
  return ((pantilt_ == rhs.pantilt_) && (zoom_ == rhs.zoom_));
}

}
