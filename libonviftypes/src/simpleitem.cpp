// simpleitem.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

SimpleItem::SimpleItem()
{
	
}

SimpleItem::SimpleItem(const boost::optional<std::string>& name, const boost::optional<std::string>& value) :
  name_(name),
  value_(value)
{
	
}

SimpleItem::SimpleItem(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    if (std::string("Name") == attribute.name())
    {
      name_ = attribute.value();

    }
    else if (std::string("Value") == attribute.name())
    {
      value_ = attribute.value();

    }
  }
}

std::string SimpleItem::ToXml(const std::string& name) const
{
  return ("<"+name+ ToXmlAttribute("Name", name_) + ToXmlAttribute("Value", value_) + "/>");
}

bool SimpleItem::operator==(const SimpleItem& rhs) const
{
  return ((name_ == rhs.name_) && (value_ == rhs.value_));
}

}
