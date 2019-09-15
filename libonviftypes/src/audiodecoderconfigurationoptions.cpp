// audiodecoderconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioDecoderConfigurationOptions::AudioDecoderConfigurationOptions()
{

}

AudioDecoderConfigurationOptions::AudioDecoderConfigurationOptions(const boost::optional<AacDecOptions>& aacdecoptions, const boost::optional<G711DecOptions>& g711decoptions, const boost::optional<G726DecOptions>& g726decoptions) :
  aacdecoptions_(aacdecoptions),
  g711decoptions_(g711decoptions),
  g726decoptions_(g726decoptions)
{

}

AudioDecoderConfigurationOptions::AudioDecoderConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "AACDecOptions")
    {
      aacdecoptions_ = AacDecOptions(element);

    }
    else if (name == "G711DecOptions")
    {
      g711decoptions_ = G711DecOptions(element);

    }
    else if (name == "G726DecOptions")
    {
      g726decoptions_ = G726DecOptions(element);

    }
  }
}

std::string AudioDecoderConfigurationOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:AACDecOptions", aacdecoptions_) + ToXmlClass("tt:G711DecOptions", g711decoptions_) + ToXmlClass("tt:G726DecOptions", g726decoptions_) + "</"+name+">");
}

bool AudioDecoderConfigurationOptions::operator==(const AudioDecoderConfigurationOptions& rhs) const
{
  return ((aacdecoptions_ == rhs.aacdecoptions_) && (g711decoptions_ == rhs.g711decoptions_) && (g726decoptions_ == rhs.g726decoptions_));
}

}
