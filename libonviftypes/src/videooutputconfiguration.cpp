// videooutputconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoOutputConfiguration::VideoOutputConfiguration()
{
  
}

VideoOutputConfiguration::VideoOutputConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& outputtoken) :
  name_(name),
  usecount_(usecount),
  token_(token),
  outputtoken_(outputtoken)
{
  
}

VideoOutputConfiguration::VideoOutputConfiguration(const pugi::xml_node& node)
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

std::string VideoOutputConfiguration::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXml("tt:OutputToken", outputtoken_) + "</"+name+">");
}

bool VideoOutputConfiguration::operator==(const VideoOutputConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (outputtoken_ == rhs.outputtoken_) && (token_ == rhs.token_));
}

}
