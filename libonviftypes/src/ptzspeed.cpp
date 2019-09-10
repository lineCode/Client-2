// ptzspeed.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZSpeed::PTZSpeed()
{

}

PTZSpeed::PTZSpeed(const boost::optional<Vector2d>& pantilt, const boost::optional<Vector1d>& zoom) :
  pantilt_(pantilt),
  zoom_(zoom)
{

}

PTZSpeed::PTZSpeed(const pugi::xml_node& node)
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
      zoom_ = Vector1d(element);

    }
  }
}

std::string PTZSpeed::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:PanTilt", pantilt_) + ToXmlClass("tt:Zoom", zoom_) + "</"+name+">");
}

bool PTZSpeed::operator==(const PTZSpeed& rhs) const
{
  return ((pantilt_ == rhs.pantilt_) && (zoom_ == rhs.zoom_));
}

}
