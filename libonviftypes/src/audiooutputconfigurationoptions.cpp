// audiooutputconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioOutputConfigurationOptions::AudioOutputConfigurationOptions()
{

}

AudioOutputConfigurationOptions::AudioOutputConfigurationOptions(const std::vector<std::string>& outputtokensavailable, const std::vector<std::string>& sendprimacyoptions, const boost::optional<IntRange>& outputlevelrange) :
  outputtokensavailable_(outputtokensavailable),
  sendprimacyoptions_(sendprimacyoptions),
  outputlevelrange_(outputlevelrange)
{

}

AudioOutputConfigurationOptions::AudioOutputConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "OutputTokensAvailable")
    {
      outputtokensavailable_.push_back(element.text().get());

    }
    else if (name == "SendPrimacyOptions")
    {
      sendprimacyoptions_.push_back(element.text().get());

    }
    else if (name == "OutputLevelRange")
    {
      outputlevelrange_ = IntRange(element);

    }
  }
}

std::string AudioOutputConfigurationOptions::ToXml(const std::string& name) const
{

  return ("<"+name+">" + onvif::ToXml("tt:OutputTokensAvailable", outputtokensavailable_) + onvif::ToXml("tt:SendPrimacyOptions", sendprimacyoptions_) + onvif::ToXmlClass("tt:OutputLevelRange", outputlevelrange_) + "</"+name+">");
}

bool AudioOutputConfigurationOptions::operator==(const AudioOutputConfigurationOptions& rhs) const
{
  return (std::is_permutation(outputtokensavailable_.begin(), outputtokensavailable_.end(), rhs.outputtokensavailable_.begin(), rhs.outputtokensavailable_.end()) && std::is_permutation(sendprimacyoptions_.begin(), sendprimacyoptions_.end(), rhs.sendprimacyoptions_.begin(), rhs.sendprimacyoptions_.end()) && (outputlevelrange_ == rhs.outputlevelrange_));
}

}
