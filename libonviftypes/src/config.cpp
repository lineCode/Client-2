// config.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Config::Config()
{

}

Config::Config(const boost::optional<ItemList>& parameters, const boost::optional<std::string>& name, const boost::optional<std::string>& type) :
  parameters_(parameters),
  name_(name),
  type_(type)
{

}

Config::Config(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Parameters")
    {
      parameters_ = ItemList(element);

    }
  }

  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
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

std::string Config::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("tt:Name", name_) + ToXmlAttribute("tt:Type", type_) + ">" + ToXmlClass("tt:Parameters", parameters_) + "</"+name+">");
}

bool Config::operator==(const Config& rhs) const
{
  return ((parameters_ == rhs.parameters_) && (name_ == rhs.name_) && (type_ == rhs.type_));
}

}
