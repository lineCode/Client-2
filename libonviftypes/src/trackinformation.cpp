// trackinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

TrackInformation::TrackInformation()
{

}

TrackInformation::TrackInformation(const boost::optional<std::string>& tracktoken, const boost::optional<TRACKTYPE>& tracktype, const boost::optional<std::string>& description, const boost::optional<ws::DateTime>& datafrom, const boost::optional<ws::DateTime>& datato) :
  tracktoken_(tracktoken),
  tracktype_(tracktype),
  description_(description),
  datafrom_(datafrom),
  datato_(datato)
{

}

TrackInformation::TrackInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TrackToken")
    {
      tracktoken_ = element.text().get();

    }
    else if (name == "TrackType")
    {
      tracktype_ = GetTrackType(element.text().get());

    }
    else if (name == "Description")
    {
      description_ = element.text().get();

    }
    else if (name == "DataFrom")
    {
      datafrom_ = ws::DateTime(element);

    }
    else if (name == "DataTo")
    {
      datato_ = ws::DateTime(element);

    }
  }
}

std::string TrackInformation::ToXml(const std::string& name) const
{
  return ToXml(name, true);
}

std::string TrackInformation::ToXml(const std::string& name, const bool namespaces) const
{
  const std::string namespacett = namespaces ? "tt:" : "";
  return ("<"+name+">" + onvif::ToXml(namespacett + "TrackToken", tracktoken_) + onvif::ToXmlEnum(namespacett + "TrackType", tracktype_) + onvif::ToXml(namespacett + "Description", description_) + onvif::ToXmlClass(namespacett + "DataFrom", datafrom_) + onvif::ToXmlClass(namespacett + "DataTo", datato_) + "</"+name+">");
}

bool TrackInformation::operator==(const TrackInformation& rhs) const
{
  return ((tracktoken_ == rhs.tracktoken_) && (tracktype_ == rhs.tracktype_) && (description_ == rhs.description_) && (datafrom_ == rhs.datafrom_) && (datato_ == rhs.datato_));
}

}
