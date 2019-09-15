// audiosourceconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

AudioSourceConfiguration::AudioSourceConfiguration()
{

}

AudioSourceConfiguration::AudioSourceConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& sourcetoken) :
  name_(name),
  usecount_(usecount),
  token_(token),
  sourcetoken_(sourcetoken)
{

}

AudioSourceConfiguration::AudioSourceConfiguration(const pugi::xml_node& node)
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
    else if (name == "SourceToken")
    {
      sourcetoken_ = element.text().get();

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

std::string AudioSourceConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXml("tt:SourceToken", sourcetoken_) + "</"+name+">");
}

bool AudioSourceConfiguration::operator==(const AudioSourceConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (sourcetoken_ == rhs.sourcetoken_) && (token_ == rhs.token_));
}

}
