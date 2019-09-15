// sourcereference.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

SourceReference::SourceReference()
{

}

SourceReference::SourceReference(const boost::optional<std::string>& type, const boost::optional<std::string>& token) :
  type_(type),
  token_(token)
{

}

SourceReference::SourceReference(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "Type")
    {
      type_ = attribute.value();

    }
  }

  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Token")
    {
      token_ = element.text().get();

    }
  }
}

std::string SourceReference::ToXml(const std::string& name) const
{
  return ("<" + name + ToXmlAttribute("Type", type_) + ">" + onvif::ToXml("tt:Token", token_) + "</"+name+">");
}

bool SourceReference::operator==(const SourceReference& rhs) const
{
  return ((type_ == rhs.type_) && (token_ == rhs.token_));
}

}
