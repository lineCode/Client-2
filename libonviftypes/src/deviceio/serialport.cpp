// serialport.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

SerialPort::SerialPort()
{

}

SerialPort::SerialPort(const boost::optional<std::string>& token) :
  token_(token)
{

}

SerialPort::SerialPort(const pugi::xml_node& node)
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

std::string SerialPort::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + "/>");
}

bool SerialPort::operator==(const SerialPort& rhs) const
{
  return (token_ == rhs.token_);
}

}

}
