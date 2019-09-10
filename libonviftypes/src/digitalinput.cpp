// digitalinput.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

DigitalInput::DigitalInput()
{

}

DigitalInput::DigitalInput(const boost::optional<std::string>& token) :
  token_(token)
{

}

DigitalInput::DigitalInput(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
  }
}

std::string DigitalInput::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + "/>");
}

bool DigitalInput::operator==(const DigitalInput& rhs) const
{
  return (token_ == rhs.token_);
}

}
