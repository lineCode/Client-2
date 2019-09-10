// supportinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

SupportInformation::SupportInformation()
{

}

SupportInformation::SupportInformation(const boost::optional<BinaryData>& binary, const boost::optional<std::string>& string) :
  binary_(binary),
  string_(string)
{

}

SupportInformation::SupportInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Binary")
    {
      binary_ = BinaryData(element);

    }
    else if (name == "String")
    {
      string_ = element.text().get();

    }
  }
}

std::string SupportInformation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:Binary", binary_) + onvif::ToXml("tt:String", string_) + "</"+name+">");
}

bool SupportInformation::operator==(const SupportInformation& rhs) const
{
  return ((binary_ == rhs.binary_) && (string_ == rhs.string_));
}

}
