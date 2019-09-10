// ptzstatusfilteroptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

PTZStatusFilterOptions::PTZStatusFilterOptions()
{
  
}

PTZStatusFilterOptions::PTZStatusFilterOptions(const boost::optional<bool>& pantiltstatussupported, const boost::optional<bool>& zoomstatussupported, const boost::optional<bool>& pantiltpositionsupported, const boost::optional<bool>& zoompositionsupported) :
  pantiltstatussupported_(pantiltstatussupported),
  zoomstatussupported_(zoomstatussupported),
  pantiltpositionsupported_(pantiltpositionsupported),
  zoompositionsupported_(zoompositionsupported)
{
  
}

PTZStatusFilterOptions::PTZStatusFilterOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "PanTiltStatusSupported")
    {
      pantiltstatussupported_ = element.text().as_bool();

    }
    else if (name == "ZoomStatusSupported")
    {
      zoomstatussupported_ = element.text().as_bool();

    }
    else if (name == "PanTiltPositionSupported")
    {
      pantiltpositionsupported_ = element.text().as_bool();

    }
    else if (name == "ZoomPositionSupported")
    {
      zoompositionsupported_ = element.text().as_bool();

    }
  }
}

std::string PTZStatusFilterOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:PanTiltStatusSupported", pantiltstatussupported_) + onvif::ToXml("tt:ZoomStatusSupported", zoomstatussupported_) + onvif::ToXml("tt:PanTiltPositionSupported", pantiltpositionsupported_) + onvif::ToXml("tt:ZoomPositionSupported", zoompositionsupported_) + "</"+name+">");
}

bool PTZStatusFilterOptions::operator==(const PTZStatusFilterOptions& rhs) const
{
  return ((pantiltstatussupported_ == rhs.pantiltstatussupported_) && (zoomstatussupported_ == rhs.zoomstatussupported_) && (pantiltpositionsupported_ == rhs.pantiltpositionsupported_) && (zoompositionsupported_ == rhs.zoompositionsupported_));
}

}
