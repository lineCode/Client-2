// dynamicdnsinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DynamicDNSInformation::DynamicDNSInformation()
{

}

DynamicDNSInformation::DynamicDNSInformation(const boost::optional<DYNAMICDNSTYPE>& type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl) :
  type_(type),
  name_(name),
  ttl_(ttl)
{

}

DynamicDNSInformation::DynamicDNSInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Type")
    {
      type_ = GetDynamicDNSType(element.text().get());

    }
    else if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "TTL")
    {
      ttl_ = GetDuration(element.text().get());

    }
  }
}

std::string DynamicDNSInformation::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:Type", type_) + onvif::ToXml("tt:Name", name_) + onvif::ToXml("tt:TTL", ttl_) + "</"+name+">");
}

bool DynamicDNSInformation::operator==(const DynamicDNSInformation& rhs) const
{
  return ((type_ == rhs.type_) && (name_ == rhs.name_) && (ttl_ == rhs.ttl_));
}

}
