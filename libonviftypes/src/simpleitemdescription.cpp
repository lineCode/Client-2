// simpleitemdescription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SimpleItemDescription::SimpleItemDescription()
{

}

SimpleItemDescription::SimpleItemDescription(const std::string& name, const std::string& type) :
  name_(name),
  type_(type)
{

}

SimpleItemDescription::SimpleItemDescription(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    std::string name = TrimNamespace(attribute.name());
    if (name == "Name")
    {
      name_ = attribute.value();

    }
    else if (name == "Type")
    {
      type_ = attribute.value();

    }
  }
}

std::string SimpleItemDescription::ToXml(const std::string& name) const
{
  return std::string("<" + name + ToXmlAttribute("tt:Name", boost::optional<std::string>(name_)) + ToXmlAttribute("tt:Type", boost::optional<std::string>(type_)) + "/>");
}

bool SimpleItemDescription::operator==(const SimpleItemDescription& rhs) const
{
  return ((name_ == rhs.name_) && (type_ == rhs.type_));
}

}
