// videosourceconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

VideoSourceConfiguration::VideoSourceConfiguration()
{
  
}

VideoSourceConfiguration::VideoSourceConfiguration(const boost::optional<std::string>& name, const boost::optional<int>& usecount, const boost::optional<std::string>& token, const boost::optional<std::string>& sourcetoken, const boost::optional<IntRectangle>& bounds, const boost::optional<Rotate>& rotate) :
  name_(name),
  usecount_(usecount),
  token_(token),
  sourcetoken_(sourcetoken),
  bounds_(bounds),
  rotate_(rotate)
{
  
}

VideoSourceConfiguration::VideoSourceConfiguration(const pugi::xml_node& node)
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
    else if (name == "Bounds")
    {
      bounds_ = IntRectangle(element);

    }
    else if (name == "Extension")
    {
      for (const auto& extension : element)
      {
        const std::string name = TrimNamespace(extension.name());
        if (name == "Rotate")
        {
          rotate_ = Rotate(extension);

        }
      }
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

std::string VideoSourceConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("token", token_) + ">" + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:UseCount", usecount_) + onvif::ToXml("tt:SourceToken", sourcetoken_) + ToXmlClass("tt:Bounds", bounds_) + "<tt:Extension>" + ToXmlClass("tt:Rotate", rotate_) + "</tt:Extension></"+name+">");
}

bool VideoSourceConfiguration::operator==(const VideoSourceConfiguration& rhs) const
{
  return ((name_ == rhs.name_) && (usecount_ == rhs.usecount_) && (token_ == rhs.token_) && (sourcetoken_ == rhs.sourcetoken_) && (bounds_ == rhs.bounds_) && (rotate_ == rhs.rotate_));
}

}
