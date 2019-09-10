// g726decoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

G726DecOptions::G726DecOptions()
{

}

G726DecOptions::G726DecOptions(const boost::optional<IntList>& bitrate, const boost::optional<IntList>& sampleraterange) :
  bitrate_(bitrate),
  sampleraterange_(sampleraterange)
{

}

G726DecOptions::G726DecOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Bitrate")
    {
      bitrate_ = IntList(element);

    }
    else if (name == "SampleRateRange")
    {
      sampleraterange_ = IntList(element);

    }
  }
}

std::string G726DecOptions::ToXml(const std::string& name) const
{
  return ("<"+name+">" + ToXmlClass("tt:Bitrate", sampleraterange_) + ToXmlClass("tt:SampleRateRange", sampleraterange_) + "</"+name+">");
}

bool G726DecOptions::operator==(const G726DecOptions& rhs) const
{
  return ((sampleraterange_ == rhs.sampleraterange_) && (sampleraterange_ == rhs.sampleraterange_));
}

}
