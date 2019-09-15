// videooutputconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoOutputConfigurationOptions::VideoOutputConfigurationOptions()
{
  
}

VideoOutputConfigurationOptions::VideoOutputConfigurationOptions(const pugi::xml_node& node)
{

}

std::string VideoOutputConfigurationOptions::ToXml(const std::string& name) const
{
  return ("<"+name+"/>");
}

bool VideoOutputConfigurationOptions::operator==(const VideoOutputConfigurationOptions& rhs) const
{
  return true;
}

}
