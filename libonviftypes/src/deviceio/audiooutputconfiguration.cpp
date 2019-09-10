// audiooutputconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

AudioOutputConfiguration::AudioOutputConfiguration()
{

}

AudioOutputConfiguration::AudioOutputConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& outputtoken, const boost::optional<std::string>& sendprimacy, const boost::optional<int>& outputlevel) :
  name_(name),
  usecount_(usecount),
  token_(token),
  outputtoken_(outputtoken),
  sendprimacy_(sendprimacy),
  outputlevel_(outputlevel)
{

}

AudioOutputConfiguration::AudioOutputConfiguration(const pugi::xml_node& node)
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
    else if (name == "OutputToken")
    {
      outputtoken_ = element.text().get();

    }
    else if (name == "SendPrimacy")
    {
      sendprimacy_ = element.text().get();

    }
    else if (name == "OutputLevel")
    {
      outputlevel_ = element.text().as_int();

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

std::string AudioOutputConfiguration::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXml("tt:OutputToken", outputtoken_) + onvif::ToXml("tt:SendPrimacy", sendprimacy_) + onvif::ToXml("tt:OutputLevel", outputlevel_) + "</" + name + ">");
}

bool AudioOutputConfiguration::operator==(const AudioOutputConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (outputtoken_ == rhs.outputtoken_) && (sendprimacy_ == rhs.sendprimacy_) && (outputlevel_ == rhs.outputlevel_) && (token_ == rhs.token_));
}

}

}
