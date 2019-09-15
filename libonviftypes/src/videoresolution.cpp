// videoresolution.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoResolution::VideoResolution()
{
  
}

VideoResolution::VideoResolution(const boost::optional<int>& width, const boost::optional<int>& height) :
  width_(width),
  height_(height)
{
  
}

VideoResolution::VideoResolution(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Width")
    {
      width_ = element.text().as_int();

    }
    else if (name == "Height")
    {
      height_ = element.text().as_int();

    }
  }
}

std::string VideoResolution::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Width", width_) + onvif::ToXml("tt:Height", height_) + "</"+name+">");
}

bool VideoResolution::operator==(const VideoResolution& rhs) const
{
  return ((width_ == rhs.width_) && (height_ == rhs.height_));
}

}
