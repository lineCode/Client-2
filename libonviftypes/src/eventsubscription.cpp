// eventsubscription.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

EventSubscription::EventSubscription()
{

}

EventSubscription::EventSubscription(const boost::optional<ws::FilterType>& filter) :
  filter_(filter)
{

}

EventSubscription::EventSubscription(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Filter")
    {
      filter_ = ws::FilterType(element);

    }
  }
}

std::string EventSubscription::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlClass("tt:Filter", filter_) + "</"+name+">");
}

bool EventSubscription::operator==(const EventSubscription& rhs) const
{
  return (filter_ == rhs.filter_);
}

}
