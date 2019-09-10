// videosourcemode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Classes /////

VideoSourceMode::VideoSourceMode()
{

}

VideoSourceMode::VideoSourceMode(const boost::optional<float>& maxframerate, const boost::optional<VideoResolution>& maxresolution, const std::vector<VIDEOENCODING>& encodings, const boost::optional<bool>& reboot, const boost::optional<std::string>& description, const boost::optional<std::string>& token, const boost::optional<bool>& enabled) :
  maxframerate_(maxframerate),
  maxresolution_(maxresolution),
  encodings_(encodings),
  reboot_(reboot),
  description_(description),
  token_(token),
  enabled_(enabled)
{

}

VideoSourceMode::VideoSourceMode(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "MaxFramerate")
    {
      maxframerate_ = element.text().as_float();

    }
    else if (name == "MaxResolution")
    {
      maxresolution_ = VideoResolution(element);

    }
    else if (name == "Encodings")
    {
      // Split the string by all imaginable delimeters, then check if it is something we recognise, if it is we add it to the list
      const std::vector<std::string> encodings = utility::SplitString(element.text().get(), { ' ', ',', ':', ';', '\r', '\n' });
      for (const auto& text : encodings)
      {
        boost::optional<VIDEOENCODING> encoding = GetVideoEncoding(text);
        if (encoding.is_initialized())
        {
          encodings_.push_back(*encoding);
        
        }
      }
    }
    else if (name == "Reboot")
    {
      reboot_ = element.text().as_bool();

    }
    else if (name == "Description")
    {
      description_ = element.text().get();

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
    else if (name == "Enabled")
    {
      enabled_ = attribute.as_bool();

    }
  }
}

std::string VideoSourceMode::ToXml(const std::string& name) const
{
  std::vector<std::string> encodings;
  for (const auto encoding : encodings_)
  {
    encodings.push_back(onvif::ToString(encoding));
  
  }
  
  return ("<"+name+ToXmlAttribute("token", token_)+ToXmlAttribute("Enabled", enabled_)+">" + onvif::ToXml("tt:MaxFramerate", maxframerate_) + ToXmlClass("tt:MaxResolution", maxresolution_) + "<tt:Encodings>" + boost::join(encodings, " ") + "</tt:Encodings>" + onvif::ToXml("tt:Reboot", reboot_) + onvif::ToXml("tt:Description", description_) + "</"+name+">");
}

bool VideoSourceMode::operator==(const VideoSourceMode& rhs) const
{
  return ((maxframerate_ == rhs.maxframerate_) && (maxresolution_ == rhs.maxresolution_) && std::is_permutation(encodings_.begin(), encodings_.end(), rhs.encodings_.begin(), rhs.encodings_.end()) && (reboot_ == rhs.reboot_) && (description_ == rhs.description_) && (enabled_ == rhs.enabled_));
}

}

}
