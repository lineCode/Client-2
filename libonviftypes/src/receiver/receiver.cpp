// receiver.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Classes /////

Receiver::Receiver()
{

}

Receiver::Receiver(const boost::optional<std::string>& token, const boost::optional<ReceiverConfiguration>& configuration) :
  token_(token),
  configuration_(configuration)
{

}

Receiver::Receiver(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Token")
    {
      token_ = element.text().get();

    }
    else if (name == "Configuration")
    {
      configuration_ = ReceiverConfiguration(element);

    }
  }
}

std::string Receiver::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Token", token_) + onvif::ToXmlClass("tt:Configuration", configuration_) + "</"+name+">");
}

bool Receiver::operator==(const Receiver& rhs) const
{
  return ((token_ == rhs.token_) && (configuration_ == rhs.configuration_));
}

}

}
