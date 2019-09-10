// audiosource.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioSource::AudioSource()
{

}

AudioSource::AudioSource(const boost::optional<std::string>& token, const boost::optional<int>& channels) :
  token_(token),
  channels_(channels)
{

}

AudioSource::AudioSource(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }

  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Channels")
    {
      channels_ = element.text().as_int();

    }
  }
}

std::string AudioSource::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Channels", channels_) + "</"+name+">");
}

bool AudioSource::operator==(const AudioSource& rhs) const
{
  return ((token_ == rhs.token_) && (channels_ == rhs.channels_));
}

}
