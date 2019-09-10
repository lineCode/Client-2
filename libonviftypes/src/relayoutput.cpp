// relayoutput.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

RelayOutput::RelayOutput()
{

}

RelayOutput::RelayOutput(const boost::optional<std::string>& token, const boost::optional<RelayOutputSettings>& properties) :
  token_(token),
  properties_(properties)
{

}

RelayOutput::RelayOutput(const pugi::xml_node& node)
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
    if (name == "Properties")
    {
      properties_ = RelayOutputSettings(element);

    }
  }
}

std::string RelayOutput::ToXml(const std::string& name) const
{
  return ("<"+name+ onvif::ToXmlAttribute("token", token_) + ">" + onvif::ToXmlClass("Properties", properties_) + "</"+name+">");
}

bool RelayOutput::operator==(const RelayOutput& rhs) const
{
  return ((token_ == rhs.token_) && (properties_ == rhs.properties_));
}

}
