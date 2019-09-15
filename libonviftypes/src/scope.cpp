// scope.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Scope::Scope()
{

}

Scope::Scope(const boost::optional<SCOPEDEFINITION>& scopedef, const boost::optional<std::string>& scopeitem) :
  scopedef_(scopedef),
  scopeitem_(scopeitem)
{

}

Scope::Scope(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "ScopeDef")
    {
      scopedef_ = GetScopeDefinitionType(element.text().get());

    }
    else if (name == "ScopeItem")
    {
      scopeitem_ = element.text().get();

    }
  }
}

std::string Scope::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:ScopeDef", scopedef_) + (onvif::ToXml("tt:ScopeItem", scopeitem_)) + "</"+name+">");
}

bool Scope::operator==(const Scope& rhs) const
{
  return ((scopedef_ == rhs.scopedef_) && (scopeitem_ == rhs.scopeitem_));
}

}
