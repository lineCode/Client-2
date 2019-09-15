// videosource.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoSource::VideoSource()
{

}

VideoSource::VideoSource(const boost::optional<std::string>& token, const boost::optional<float>& framerate, const boost::optional<VideoResolution>& resolution, const boost::optional<ImagingSettings>& imaging, const boost::optional<ImagingSettings20>& extensionimaging) :
  token_(token),
  framerate_(framerate),
  resolution_(resolution),
  imaging_(imaging),
  extensionimaging_(extensionimaging)
{

}

VideoSource::VideoSource(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Framerate")
    {
      framerate_ = element.text().as_float();

    }
    else if (name == "Resolution")
    {
      resolution_ = VideoResolution(element);

    }
    else if (name == "Imaging")
    {
      imaging_ = ImagingSettings(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Imaging")
        {
          extensionimaging_ = ImagingSettings20(extension);

        }
      }
    }

    for (const auto& attribute : node.attributes())
    {
      const std::string name = TrimNamespace(attribute.name());
      if (name == "token")
      {
        token_ = attribute.value();

      }
    }
  }
}

std::string VideoSource::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Framerate", framerate_) + ToXmlClass("tt:Resolution", resolution_) + ToXmlClass("tt:Imaging", imaging_) + "<Extension>" + ToXmlClass("tt:Imaging", extensionimaging_) + "</Extension></"+name+">");
}

bool VideoSource::operator==(const VideoSource& rhs) const
{
  return ((token_ == rhs.token_) && (framerate_ == rhs.framerate_) && (resolution_ == rhs.resolution_) && (imaging_ == rhs.imaging_) && (extensionimaging_ == rhs.extensionimaging_));
}

}
