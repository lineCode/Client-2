// analyticsdevicecapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

AnalyticsDeviceCapabilities::AnalyticsDeviceCapabilities()
{

}

AnalyticsDeviceCapabilities::AnalyticsDeviceCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& rulesupport) :
  xaddr_(xaddr),
  rulesupport_(rulesupport)
{

}

AnalyticsDeviceCapabilities::AnalyticsDeviceCapabilities(const pugi::xml_node& node)
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
  }
}

std::string AnalyticsDeviceCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:RuleSupport", rulesupport_) + "</"+name+">");
}

bool AnalyticsDeviceCapabilities::operator==(const AnalyticsDeviceCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (rulesupport_ == rhs.rulesupport_));
}

}
