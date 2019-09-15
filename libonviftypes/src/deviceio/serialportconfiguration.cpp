// serialportconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

SerialPortConfiguration::SerialPortConfiguration()
{

}

SerialPortConfiguration::SerialPortConfiguration(const boost::optional<std::string>& token, const boost::optional<SERIALPORTTYPE>& type, const boost::optional<int>& baudrate, const boost::optional<PARITYBIT>& paritybit, const boost::optional<int>& characterlength, const boost::optional<float>& stopbit) :
  token_(token),
  type_(type),
  baudrate_(baudrate),
  paritybit_(paritybit),
  characterlength_(characterlength),
  stopbit_(stopbit)
{

}

SerialPortConfiguration::SerialPortConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BaudRate")
    {
      baudrate_ = element.text().as_int();

    }
    else if (name == "ParityBit")
    {
      paritybit_ = GetParityBit(element.text().get());

    }
    else if (name == "CharacterLength")
    {
      characterlength_ = element.text().as_int();

    }
    else if (name == "StopBit")
    {
      stopbit_ = element.text().as_float();

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "token")
    {
      token_ = attribute.value();

    }
    else if (name == "type")
    {
      type_ = GetSerialPortType(attribute.value());

    }
  }
}

std::string SerialPortConfiguration::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ToXmlAttributeEnum("type", type_) + ">" + onvif::ToXml("tt:BaudRate", baudrate_) + onvif::ToXmlEnum("tt:ParityBit", paritybit_) + onvif::ToXml("tt:CharacterLength", characterlength_) + onvif::ToXml("tt:StopBit", stopbit_) + "</" + name + ">");
}

bool SerialPortConfiguration::operator==(const SerialPortConfiguration& rhs) const
{
  return ((token_ == rhs.token_) && (type_ == rhs.type_) && (baudrate_ == rhs.baudrate_) && (paritybit_ == rhs.paritybit_) && (characterlength_ == rhs.characterlength_) && (stopbit_ == rhs.stopbit_));
}

}

}
