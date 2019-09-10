// audioencoderconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioEncoderConfigurationOptions::AudioEncoderConfigurationOptions()
{

}

AudioEncoderConfigurationOptions::AudioEncoderConfigurationOptions(const std::vector<AudioEncoderConfigurationOption>& options) :
  options_(options)
{

}

AudioEncoderConfigurationOptions::AudioEncoderConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Options")
    {
      options_.push_back(AudioEncoderConfigurationOption(element));

    }
  }
}

std::string AudioEncoderConfigurationOptions::ToXml(const std::string& name) const
{
  std::string options;
  for (const auto option : options_)
  {
    options += option.ToXml("tt:Options");

  }

  return ("<"+name+">" + options + "</"+name+">");
}

bool AudioEncoderConfigurationOptions::operator==(const AudioEncoderConfigurationOptions& rhs) const
{
  return std::is_permutation(options_.begin(), options_.end(), rhs.options_.begin(), rhs.options_.end());
}

}
