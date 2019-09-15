// gettracksresponseitem.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

GetTracksResponseItem::GetTracksResponseItem()
{

}

GetTracksResponseItem::GetTracksResponseItem(const boost::optional<std::string>& tracktoken, const boost::optional<TrackConfiguration>& configuration) :
  tracktoken_(tracktoken),
  configuration_(configuration)
{

}

GetTracksResponseItem::GetTracksResponseItem(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TrackToken")
    {
      tracktoken_ = element.text().get();

    }
    else if (name == "Configuration")
    {
      configuration_ = TrackConfiguration(element);

    }
  }
}

std::string GetTracksResponseItem::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXml("tt:TrackToken", tracktoken_) + onvif::ToXmlClass("tt:Configuration", configuration_) + "</"+name+">");
}

bool GetTracksResponseItem::operator==(const GetTracksResponseItem& rhs) const
{
  return ((tracktoken_ == rhs.tracktoken_) && (configuration_ == rhs.configuration_));
}

}

}
