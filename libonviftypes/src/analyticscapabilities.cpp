// analyticscapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AnalyticsCapabilities::AnalyticsCapabilities()
{

}

AnalyticsCapabilities::AnalyticsCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& rulesupport, const boost::optional<bool>& analyticsmodulesupport) :
  xaddr_(xaddr),
  rulesupport_(rulesupport),
  analyticsmodulesupport_(analyticsmodulesupport)
{

}

AnalyticsCapabilities::AnalyticsCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "RuleSupport")
    {
      rulesupport_ = element.text().as_bool();

    }
    else if (name == "AnalyticsModuleSupport")
    {
      analyticsmodulesupport_ = element.text().as_bool();

    }
  }
}

std::string AnalyticsCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:RuleSupport", rulesupport_) + onvif::ToXml("tt:AnalyticsModuleSupport", analyticsmodulesupport_) + "</"+name+">");
}

bool AnalyticsCapabilities::operator==(const AnalyticsCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (rulesupport_ == rhs.rulesupport_) && (analyticsmodulesupport_ == rhs.analyticsmodulesupport_));
}

}
