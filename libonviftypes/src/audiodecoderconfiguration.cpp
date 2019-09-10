// audiodecoderconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioDecoderConfiguration::AudioDecoderConfiguration()
{

}

AudioDecoderConfiguration::AudioDecoderConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token) :
  name_(name),
  usecount_(usecount),
  token_(token)
{

}

AudioDecoderConfiguration::AudioDecoderConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "UseCount")
    {
      usecount_ = element.text().as_int();

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

std::string AudioDecoderConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + "</"+name+">");
}

bool AudioDecoderConfiguration::operator==(const AudioDecoderConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (token_ == rhs.token_));
}

}
