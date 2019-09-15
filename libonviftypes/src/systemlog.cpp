// systemlog.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SystemLog::SystemLog()
{

}

SystemLog::SystemLog(const boost::optional<AttachmentData>& binary, const boost::optional<std::string>& string) :
  binary_(binary),
  string_(string)
{

}

SystemLog::SystemLog(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Binary")
    {
      binary_ = AttachmentData(element);

    }
    else if (name == "String")
    {
      string_ = element.text().get();

    }
  }
}

std::string SystemLog::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:Binary", binary_) + onvif::ToXml("tt:String", string_) + "</"+name+">");
}

bool SystemLog::operator==(const SystemLog& rhs) const
{
  return ((binary_ == rhs.binary_) && (string_ == rhs.string_));
}

}
