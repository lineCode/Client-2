// eventcapabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

EventCapabilities::EventCapabilities()
{
  
}

EventCapabilities::EventCapabilities(const boost::optional<std::string>& xaddr, const boost::optional<bool>& wssubscriptionpolicysupport, const boost::optional<bool>& wspullpointsupprt, const boost::optional<bool>& wspausablesubscriptionmanagerinterfacesupport) :
  xaddr_(xaddr),
  wssubscriptionpolicysupport_(wssubscriptionpolicysupport),
  wspullpointsupprt_(wspullpointsupprt),
  wspausablesubscriptionmanagerinterfacesupport_(wspausablesubscriptionmanagerinterfacesupport)
{
  
}

EventCapabilities::EventCapabilities(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "XAddr")
    {
      xaddr_ = element.text().get();

    }
    else if (name == "WSSubscriptionPolicySupport")
    {
      wssubscriptionpolicysupport_ = element.text().as_bool();

    }
    else if (name == "WSPullPointSupport")
    {
      wspullpointsupprt_ = element.text().as_bool();

    }
    else if (name == "WSPausableSubscriptionManagerInterfaceSupport")
    {
      wspausablesubscriptionmanagerinterfacesupport_ = element.text().as_bool();

    }
  }
}

std::string EventCapabilities::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:XAddr", xaddr_) + onvif::ToXml("tt:WSSubscriptionPolicySupport", wssubscriptionpolicysupport_) + onvif::ToXml("tt:WSPullPointSupport", wspullpointsupprt_) + onvif::ToXml("tt:WSPausableSubscriptionManagerInterfaceSupport", wspausablesubscriptionmanagerinterfacesupport_) + "</"+name+">");
}

bool EventCapabilities::operator==(const EventCapabilities& rhs) const
{
  return ((xaddr_ == rhs.xaddr_) && (wssubscriptionpolicysupport_ == rhs.wssubscriptionpolicysupport_) && (wspullpointsupprt_ == rhs.wspullpointsupprt_) && (wspausablesubscriptionmanagerinterfacesupport_ == rhs.wspausablesubscriptionmanagerinterfacesupport_));
}

}
