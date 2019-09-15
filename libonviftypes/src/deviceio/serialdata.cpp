// serialdata.cpp
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

SerialData::SerialData()
{

}

SerialData::SerialData(const boost::optional<std::string>& binary, const boost::optional<std::string>& string) :
  binary_(binary),
  string_(string)
{

}

SerialData::SerialData(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Binary")
    {
      binary_ = element.text().get();

    }
    else if (name == "String")
    {
      string_ = element.text().get();

    }
  }
}

std::string SerialData::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:Binary", binary_) + onvif::ToXml("tt:String", string_) + "</" + name + ">");
}

bool SerialData::operator==(const SerialData& rhs) const
{
  return ((binary_ == rhs.binary_) && (string_ == rhs.string_));
}

}

}
