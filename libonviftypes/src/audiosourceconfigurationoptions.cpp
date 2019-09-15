// audiosourceconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioSourceConfigurationOptions::AudioSourceConfigurationOptions()
{

}

AudioSourceConfigurationOptions::AudioSourceConfigurationOptions(const std::vector<std::string>& inputtokensavailable) :
  inputtokensavailable_(inputtokensavailable)
{

}

AudioSourceConfigurationOptions::AudioSourceConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "InputTokensAvailable")
    {
      inputtokensavailable_.push_back(element.text().get());

    }
  }
}

std::string AudioSourceConfigurationOptions::ToXml(const std::string& name) const
{

  return ("<"+name+">" + onvif::ToXml("tt:InputTokensAvailable", inputtokensavailable_) + "</"+name+">");
}

bool AudioSourceConfigurationOptions::operator==(const AudioSourceConfigurationOptions& rhs) const
{
  return std::is_permutation(inputtokensavailable_.begin(), inputtokensavailable_.end(), rhs.inputtokensavailable_.begin(), rhs.inputtokensavailable_.end());
}

}
