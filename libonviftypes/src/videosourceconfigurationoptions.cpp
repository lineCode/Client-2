// videosourceconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoSourceConfigurationOptions::VideoSourceConfigurationOptions()
{
  
}

VideoSourceConfigurationOptions::VideoSourceConfigurationOptions(const boost::optional<IntRectangleRange>& boundsrange, const std::vector<std::string>& videosourcetokensavailable, const boost::optional<RotateOptions>& rotate) :
  boundsrange_(boundsrange),
  videosourcetokensavailable_(videosourcetokensavailable),
  rotate_(rotate)
{
  
}

VideoSourceConfigurationOptions::VideoSourceConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BoundsRange")
    {
      boundsrange_ = IntRectangleRange(element);

    }
    else if (name == "VideoSourceTokensAvailable")
    {
      videosourcetokensavailable_.push_back(element.text().get());

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Rotate")
        {
          rotate_ = RotateOptions(extension);

        }
      }
    }
  }
}

std::string VideoSourceConfigurationOptions::ToXml(const std::string& name) const
{
  std::string videosourcetokensavailable;
  for (const auto& videosourcetokenavailable : videosourcetokensavailable_)
  {
    videosourcetokensavailable += std::string("<tt:VideoSourceTokensAvailable>") + videosourcetokenavailable + std::string("</tt:VideoSourceTokensAvailable>");

  }
  
  return ("<"+name+">" + ToXmlClass("tt:BoundsRange", boundsrange_) + videosourcetokensavailable + "<Extension>" + ToXmlClass("tt:Rotate", rotate_) + "</Extension></"+name+">");
}

bool VideoSourceConfigurationOptions::operator==(const VideoSourceConfigurationOptions& rhs) const
{
  return ((boundsrange_ == rhs.boundsrange_) && std::is_permutation(videosourcetokensavailable_.begin(), videosourcetokensavailable_.end(), rhs.videosourcetokensavailable_.begin(), rhs.videosourcetokensavailable_.end()) && (rotate_ == rhs.rotate_));
}

}
