// audioencoderconfigurationoption.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioEncoderConfigurationOption::AudioEncoderConfigurationOption()
{

}

AudioEncoderConfigurationOption::AudioEncoderConfigurationOption(const boost::optional<AUDIOENCODING>& encoding, const boost::optional<IntList>& bitratelist, const boost::optional<IntList>& sampleratelist) :
  encoding_(encoding),
  bitratelist_(bitratelist),
  sampleratelist_(sampleratelist)
{

}

AudioEncoderConfigurationOption::AudioEncoderConfigurationOption(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Encoding")
    {
      encoding_ = GetAudioEncoding(element.text().get());

    }
    else if (name == "BitrateList")
    {
      bitratelist_ = IntList(element);

    }
    else if (name == "SampleRateList")
    {
      sampleratelist_ = IntList(element);

    }
  }
}

std::string AudioEncoderConfigurationOption::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlEnum("tt:Encoding", encoding_) + ToXmlClass("tt:BitrateList", bitratelist_) + ToXmlClass("tt:SampleRateList", sampleratelist_) + "</"+name+">");
}

bool AudioEncoderConfigurationOption::operator==(const AudioEncoderConfigurationOption& rhs) const
{
  return ((encoding_ == rhs.encoding_) && (bitratelist_ == rhs.bitratelist_) && (sampleratelist_ == rhs.sampleratelist_));
}

}
