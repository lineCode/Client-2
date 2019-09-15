// profile.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Profile::Profile()
{

}

Profile::Profile(const boost::optional<std::string>& name, const boost::optional<VideoSourceConfiguration>& videosourceconfiguration, const boost::optional<AudioSourceConfiguration>& audiosourceconfiguration, const boost::optional<VideoEncoderConfiguration>& videoencoderconfiguration, const boost::optional<AudioEncoderConfiguration>& audioencoderconfiguration, const boost::optional<VideoAnalyticsConfiguration>& videoanalyticsconfiguration, const boost::optional<PTZConfiguration>& ptzconfiguration, const boost::optional<MetadataConfiguration>& metadataconfiguration, const boost::optional<AudioOutputConfiguration>& audiooutputconfiguration, const boost::optional<AudioDecoderConfiguration>& audiodecoderconfiguration, const boost::optional<std::string>& token, const boost::optional<bool>& fixed) :
  name_(name),
  videosourceconfiguration_(videosourceconfiguration),
  audiosourceconfiguration_(audiosourceconfiguration),
  videoencoderconfiguration_(videoencoderconfiguration),
  audioencoderconfiguration_(audioencoderconfiguration),
  videoanalyticsconfiguration_(videoanalyticsconfiguration),
  ptzconfiguration_(ptzconfiguration),
  metadataconfiguration_(metadataconfiguration),
  audiooutputconfiguration_(audiooutputconfiguration),
  audiodecoderconfiguration_(audiodecoderconfiguration),
  token_(token),
  fixed_(fixed)
{
  
}

Profile::Profile(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "VideoSourceConfiguration")
    {
      videosourceconfiguration_ = VideoSourceConfiguration(element);

    }
    else if (name == "AudioSourceConfiguration")
    {
      audiosourceconfiguration_ = AudioSourceConfiguration(element);
      
    }
    else if (name == "VideoEncoderConfiguration")
    {
      videoencoderconfiguration_ = VideoEncoderConfiguration(element);
      
    }
    else if (name == "AudioEncoderConfiguration")
    {
      audioencoderconfiguration_ = AudioEncoderConfiguration(element);
      
    }
    else if (name == "VideoAnalyticsConfiguration")
    {
      videoanalyticsconfiguration_ = VideoAnalyticsConfiguration(element);
      
    }
    else if (name == "PTZConfiguration")
    {
      ptzconfiguration_ = PTZConfiguration(element);

    }
    else if (name == "MetadataConfiguration")
    {
      metadataconfiguration_ = MetadataConfiguration(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "AudioOutputConfiguration")
        {
          audiooutputconfiguration_ = AudioOutputConfiguration(extension);

        }
        else if (name == "AudioDecoderConfiguration")
        {
          audiodecoderconfiguration_ = AudioDecoderConfiguration(extension);

        }
      }
    }
  }

  for (const auto& attribute : node.attributes())
  {
    if (std::string("token") == attribute.name())
    {
      token_ = attribute.value();

    }
    else if (std::string("fixed") == attribute.name())
    {
      fixed_ = attribute.as_bool();

    }
  }
}

std::string Profile::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_) + ToXmlAttribute("fixed", fixed_) + ">" + onvif::ToXml("tt:Name", name_) + ToXmlClass("tt:VideoSourceConfiguration", videosourceconfiguration_) + ToXmlClass("tt:AudioSourceConfiguration", audiosourceconfiguration_) + ToXmlClass("tt:VideoEncoderConfiguration", videoencoderconfiguration_) + ToXmlClass("tt:AudioEncoderConfiguration", audioencoderconfiguration_) + ToXmlClass("tt:VideoAnalyticsConfiguration", videoanalyticsconfiguration_) + ToXmlClass("tt:PTZConfiguration", ptzconfiguration_) + ToXmlClass("tt:MetadataConfiguration", metadataconfiguration_) + "<tt:Extension>" + ToXmlClass("tt:AudioOutputConfiguration", audiooutputconfiguration_) + ToXmlClass("tt:AudioDecoderConfiguration", audiodecoderconfiguration_) + "</tt:Extension></"+name+">");
}

bool Profile::operator==(const Profile& rhs) const
{
  return ((name_ == rhs.name_) && (videosourceconfiguration_ == rhs.videosourceconfiguration_) && (audiosourceconfiguration_ == rhs.audiosourceconfiguration_) && (videoencoderconfiguration_ == rhs.videoencoderconfiguration_) && (audioencoderconfiguration_ == rhs.audioencoderconfiguration_) && (videoanalyticsconfiguration_ == rhs.videoanalyticsconfiguration_) && (ptzconfiguration_ == rhs.ptzconfiguration_) && (metadataconfiguration_ == rhs.metadataconfiguration_) && (audiooutputconfiguration_ == rhs.audiooutputconfiguration_) && (audiodecoderconfiguration_ == rhs.audiodecoderconfiguration_) && (token_ == rhs.token_) && (fixed_ == rhs.fixed_));
}

}
