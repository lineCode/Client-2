// videooutput.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoOutput::VideoOutput()
{
  
}

VideoOutput::VideoOutput(const boost::optional<std::string>& token, const boost::optional<Layout>& layout, const boost::optional<VideoResolution>& resolution, const boost::optional<float>& refreshrate, const boost::optional<float>& aspectratio) :
  token_(token),
  layout_(layout),
  resolution_(resolution),
  refreshrate_(refreshrate),
  aspectratio_(aspectratio)
{
  
}

VideoOutput::VideoOutput(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Layout")
    {
      layout_ = Layout(element);

    }
    else if (name == "Resolution")
    {
      resolution_ = VideoResolution(element);

    }
    else if (name == "RefreshRate")
    {
      refreshrate_ = element.text().as_float();

    }
    else if (name == "AspectRatio")
    {
      aspectratio_ = element.text().as_float();

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

std::string VideoOutput::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ">" + onvif::ToXmlClass("tt:Layout", layout_) + onvif::ToXmlClass("tt:Resolution", resolution_) + onvif::ToXml("tt:RefreshRate", refreshrate_) + onvif::ToXml("tt:AspectRatio", aspectratio_) + "</"+name+">");
}

bool VideoOutput::operator==(const VideoOutput& rhs) const
{
  return ((token_ == rhs.token_) && (layout_ == rhs.layout_) && (resolution_ == rhs.resolution_) && (refreshrate_ == rhs.refreshrate_) && (aspectratio_ == rhs.aspectratio_));
}

}
