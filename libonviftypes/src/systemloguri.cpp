// systemloguri.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

SystemLogUri::SystemLogUri()
{

}

SystemLogUri::SystemLogUri(const boost::optional<SYSTEMLOGTYPE>& type, const boost::optional<std::string>& uri) :
  type_(type),
  uri_(uri)
{

}

SystemLogUri::SystemLogUri(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Type")
    {
      type_ = GetSystemLogType(element.text().get());

    }
    else if (name == "Uri")
    {
      uri_ = element.text().get();

    }
  }
}

std::string SystemLogUri::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Type", type_) + (onvif::ToXml("tt:Uri", uri_)) + "</"+name+">");
}

bool SystemLogUri::operator==(const SystemLogUri& rhs) const
{
  return ((type_ == rhs.type_) && (uri_ == rhs.uri_));
}

}
