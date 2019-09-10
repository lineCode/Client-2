// recordingsourceinformation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

RecordingSourceInformation::RecordingSourceInformation()
{

}

RecordingSourceInformation::RecordingSourceInformation(const boost::optional<std::string>& sourceid, const boost::optional<std::string>& name, const boost::optional<std::string>& location, const boost::optional<std::string>& description, const boost::optional<std::string>& address) :
  sourceid_(sourceid),
  name_(name),
  location_(location),
  description_(description),
  address_(address)
{

}

RecordingSourceInformation::RecordingSourceInformation(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "SourceId")
    {
      sourceid_ = element.text().get();

    }
    else if (name == "Name")
    {
      name_ = element.text().get();

    }
    else if (name == "Location")
    {
      location_ = element.text().get();

    }
    else if (name == "Description")
    {
      description_ = element.text().get();

    }
    else if (name == "Address")
    {
      address_ = element.text().get();

    }
  }
}

std::string RecordingSourceInformation::ToXml(const std::string& name) const
{
  return ToXml(name, true);
}

std::string RecordingSourceInformation::ToXml(const std::string& name, const bool namespaces) const
{
  const std::string namespacett = namespaces ? "tt:" : "";
  return ("<"+name+">" + onvif::ToXml(namespacett + "SourceId", sourceid_) + onvif::ToXml(namespacett + "Name", name_) + onvif::ToXml(namespacett + "Location", location_) + onvif::ToXml(namespacett + "Description", description_) + onvif::ToXml(namespacett + "Address", address_) + "</"+name+">");
}

bool RecordingSourceInformation::operator==(const RecordingSourceInformation& rhs) const
{
  return ((sourceid_ == rhs.sourceid_) && (name_ == rhs.name_) && (location_ == rhs.location_) && (description_ == rhs.description_) && (address_ == rhs.address_));
}

}
