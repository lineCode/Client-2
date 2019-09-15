// capabilities.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace event
{

///// Classes /////

Capabilities::Capabilities()
{

}

Capabilities::Capabilities(const boost::optional<bool>& wssubscriptionpolicysupport, const boost::optional<bool>& wspullpointsupport, const boost::optional<bool>& wspausablesubscriptionmanagerinterfacesupport, const boost::optional<int>& maxnotificationproducers, const boost::optional<int>& maxpullpoints, const boost::optional<bool>& persistentnotificationstorage) :
  wssubscriptionpolicysupport_(wssubscriptionpolicysupport),
  wspullpointsupport_(wspullpointsupport),
  wspausablesubscriptionmanagerinterfacesupport_(wspausablesubscriptionmanagerinterfacesupport),
  maxnotificationproducers_(maxnotificationproducers),
  maxpullpoints_(maxpullpoints),
  persistentnotificationstorage_(persistentnotificationstorage)
{

}

Capabilities::Capabilities(const pugi::xml_node& node)
{
  for (const auto& attribute : node.attributes())
  {
    const std::string name = TrimNamespace(attribute.name());
    if (name == "WSSubscriptionPolicySupport")
    {
      wssubscriptionpolicysupport_ = attribute.as_bool();
      
    }
    else if (name == "WSPullPointSupport")
    {
      wspullpointsupport_ = attribute.as_bool();

    }
    else if (name == "WSPausableSubscriptionManagerInterfaceSupport")
    {
      wspausablesubscriptionmanagerinterfacesupport_ = attribute.as_bool();

    }
    else if (name == "MaxNotificationProducers")
    {
      maxnotificationproducers_ = attribute.as_int();

    }
    else if (name == "MaxPullPoints")
    {
      maxpullpoints_ = attribute.as_int();

    }
    else if (name == "PersistentNotificationStorage")
    {
      persistentnotificationstorage_ = attribute.as_bool();

    }
  }
}

std::string Capabilities::ToXml(const std::string& name) const
{
  return ("<"+name + ToXmlAttribute("WSSubscriptionPolicySupport", wssubscriptionpolicysupport_) + ToXmlAttribute("WSPullPointSupport", wspullpointsupport_) + ToXmlAttribute("WSPausableSubscriptionManagerInterfaceSupport", wspausablesubscriptionmanagerinterfacesupport_) + ToXmlAttribute("MaxNotificationProducers", maxnotificationproducers_) + ToXmlAttribute("MaxPullPoints", maxpullpoints_) + ToXmlAttribute("PersistentNotificationStorage", persistentnotificationstorage_) + "></"+name+">");
}

Element Capabilities::ToElement(const std::string& name) const
{
  std::vector< std::pair<std::string, std::string> > attributes;
  if (wssubscriptionpolicysupport_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("WSSubscriptionPolicySupport"), ToString(*wssubscriptionpolicysupport_)));

  }

  if (wspullpointsupport_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("WSPullPointSupport"), ToString(*wspullpointsupport_)));

  }

  if (wspausablesubscriptionmanagerinterfacesupport_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("WSPausableSubscriptionManagerInterfaceSupport"), ToString(*wspausablesubscriptionmanagerinterfacesupport_)));

  }

  if (maxnotificationproducers_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxNotificationProducers"), std::to_string(*maxnotificationproducers_)));

  }

  if (maxpullpoints_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("MaxPullPoints"), std::to_string(*maxpullpoints_)));

  }

  if (persistentnotificationstorage_.is_initialized())
  {
    attributes.push_back(std::make_pair(std::string("PersistentNotificationStorage"), ToString(*persistentnotificationstorage_)));

  }

  return Element(name, attributes, std::string(), {});
}

bool Capabilities::operator==(const Capabilities& rhs) const
{
  return ((wssubscriptionpolicysupport_ == rhs.wssubscriptionpolicysupport_) && (wspullpointsupport_ == rhs.wspullpointsupport_) && (wspausablesubscriptionmanagerinterfacesupport_ == rhs.wspausablesubscriptionmanagerinterfacesupport_) && (maxnotificationproducers_ == rhs.maxnotificationproducers_) && (maxpullpoints_ == rhs.maxpullpoints_) && (persistentnotificationstorage_ == rhs.persistentnotificationstorage_));
}

}

}
