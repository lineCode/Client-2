// videoanalyticsconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoAnalyticsConfiguration::VideoAnalyticsConfiguration()
{

}

VideoAnalyticsConfiguration::VideoAnalyticsConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<AnalyticsEngineConfiguration>& analyticsengineconfiguration) :
  name_(name),
  usecount_(usecount),
  token_(token),
  analyticsengineconfiguration_(analyticsengineconfiguration)
{

}

VideoAnalyticsConfiguration::VideoAnalyticsConfiguration(const pugi::xml_node& node)
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
    else if (name == "AnalyticsEngineConfiguration")
    {
      analyticsengineconfiguration_ = AnalyticsEngineConfiguration(element);

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

std::string VideoAnalyticsConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXmlClass("tt:AnalyticsEngineConfiguration", analyticsengineconfiguration_) + "</"+name+">");
}

bool VideoAnalyticsConfiguration::operator==(const VideoAnalyticsConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (analyticsengineconfiguration_ == rhs.analyticsengineconfiguration_) && (token_ == rhs.token_));
}

}
