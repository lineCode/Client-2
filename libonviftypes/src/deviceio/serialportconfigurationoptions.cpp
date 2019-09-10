// serialportconfigurationoptions.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Classes /////

SerialPortConfigurationOptions::SerialPortConfigurationOptions()
{

}

SerialPortConfigurationOptions::SerialPortConfigurationOptions(const boost::optional<IntList>& baudratelist, const boost::optional<ParityBitList>& paritybitlist, const boost::optional<IntList>& characterlengthlist, const boost::optional<FloatList>& stopbitlist, const boost::optional<std::string>& token) :
  baudratelist_(baudratelist),
  paritybitlist_(paritybitlist),
  characterlengthlist_(characterlengthlist),
  stopbitlist_(stopbitlist),
  token_(token)
{

}

SerialPortConfigurationOptions::SerialPortConfigurationOptions(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "BaudRateList")
    {
      baudratelist_ = IntList(element);

    }
    else if (name == "ParityBitList")
    {
      paritybitlist_ = ParityBitList(element);

    }
    else if (name == "CharacterLengthList")
    {
      characterlengthlist_ = IntList(element);

    }
    else if (name == "StopBitList")
    {
      stopbitlist_ = FloatList(element);

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

std::string SerialPortConfigurationOptions::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("token", token_) + ">" + onvif::ToXmlClass("tt:BaudRateList", baudratelist_) + onvif::ToXmlClass("tt:ParityBitList", paritybitlist_) + onvif::ToXmlClass("tt:CharacterLengthList", characterlengthlist_) + onvif::ToXmlClass("tt:StopBitList", stopbitlist_) + "</" + name + ">");
}

bool SerialPortConfigurationOptions::operator==(const SerialPortConfigurationOptions& rhs) const
{
  return ((baudratelist_ == rhs.baudratelist_) && (paritybitlist_ == rhs.paritybitlist_) && (characterlengthlist_ == rhs.characterlengthlist_) && (stopbitlist_ == rhs.stopbitlist_) && (token_ == rhs.token_));
}

}

}
