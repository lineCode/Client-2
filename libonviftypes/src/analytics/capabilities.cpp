// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace analytics
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& rulesupport, const boost::optional<bool>& analyticsmodulesupport, const boost::optional<bool>& cellbasedscenedescriptionsupported) :
  rulesupport_(rulesupport),
  analyticsmodulesupport_(analyticsmodulesupport),
  cellbasedscenedescriptionsupported_(cellbasedscenedescriptionsupported)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "RuleSupport")
    {
      rulesupport_ = attribute.as_bool();
      
    }
    else if (name == "AnalyticsModuleSupport")
    {
      analyticsmodulesupport_ = attribute.as_bool();

    }
    else if (name == "CellBasedSceneDescriptionSupported")
    {
      cellbasedscenedescriptionsupported_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("RuleSupport", rulesupport_) + ToXmlAttribute("AnalyticsModuleSupport", analyticsmodulesupport_) + ToXmlAttribute("CellBasedSceneDescriptionSupported", cellbasedscenedescriptionsupported_) + "></"+name+">");
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((rulesupport_ == rhs.rulesupport_) && (analyticsmodulesupport_ == rhs.analyticsmodulesupport_) && (cellbasedscenedescriptionsupported_ == rhs.cellbasedscenedescriptionsupported_));
}

}

}
