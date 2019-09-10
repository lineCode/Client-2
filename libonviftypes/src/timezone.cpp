// timezone.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

TimeZone::TimeZone()
{

}

TimeZone::TimeZone(const boost::optional<std::string>& tz) :
  tz_(tz)
{

}

TimeZone::TimeZone(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "TZ")
    {
      tz_ = element.text().get();

    }
  }
}

std::string TimeZone::ToXml(const std::string& name) const
{
 return std::string("<"+name+">" + onvif::ToXml("tt:TZ", tz_) + "</"+name+">");
}

boost::optional<boost::local_time::posix_time_zone> TimeZone::GetTimeZone() const
{
  if (!tz_)
  {

    return boost::none;
  }

  try
  {

    return boost::local_time::posix_time_zone(*tz_);
  }
  catch (...)
  {
    
    return boost::none;
  }
}

bool TimeZone::operator==(const TimeZone& rhs) const
{
  return (tz_ == rhs.tz_);
}

}
