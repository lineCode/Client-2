// elementitemdescription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

ElementItemDescription::ElementItemDescription()
{

}

ElementItemDescription::ElementItemDescription(const std::string& name, const std::string& type) :
  name_(name),
  type_(type)
{

}

ElementItemDescription::ElementItemDescription(const pugi::xml_node& node)
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

std::string ElementItemDescription::ToXml(const std::string& name) const
{
  return std::string("<" + name + ToXmlAttribute("tt:Name", boost::optional<std::string>(name_)) + ToXmlAttribute("tt:Type", boost::optional<std::string>(type_)) + "/>");
}

bool ElementItemDescription::operator==(const ElementItemDescription& rhs) const
{
  return ((name_ == rhs.name_) && (type_ == rhs.type_));
}

}
