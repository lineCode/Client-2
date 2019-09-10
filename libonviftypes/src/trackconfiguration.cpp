// trackconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

TrackConfiguration::TrackConfiguration()
{

}

TrackConfiguration::TrackConfiguration(const boost::optional<TRACKTYPE>& tracktype, const boost::optional<std::string>& description) :
  tracktype_(tracktype),
  description_(description)
{

}

TrackConfiguration::TrackConfiguration(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TrackType")
    {
      tracktype_ = GetTrackType(element.text().get());

    }
    else if (name == "Description")
    {
      description_ = element.text().get();

    }
  }
}

std::string TrackConfiguration::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:TrackType", tracktype_) + onvif::ToXml("tt:Description", description_) + "</"+name+">");
}

bool TrackConfiguration::operator==(const TrackConfiguration& rhs) const
{
  return ((tracktype_ == rhs.tracktype_) && (description_ == rhs.description_));
}

}
