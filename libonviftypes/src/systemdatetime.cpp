// systemdatetime.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Classes /////

SystemDateTime::SystemDateTime()
{
  
}

SystemDateTime::SystemDateTime(const boost::optional<DATETIMETYPE>& datetimetype, const boost::optional<bool>& daylightsavings, const boost::optional<TimeZone>& timezone, const boost::optional<DateTime>& utctime, const boost::optional<DateTime>& localtime) :
  datetimetype_(datetimetype),
  daylightsavings_(daylightsavings),
  timezone_(timezone),
  utctime_(utctime),
  localtime_(localtime)
{
  
}

SystemDateTime::SystemDateTime(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "DateTimeType")
    {
      datetimetype_ = GetDateTimeType(element.text().get());

    }
    else if (name == "DaylightSavings")
    {
      daylightsavings_ = element.text().as_bool();

    }
    else if (name == "TimeZone")
    {
      timezone_ = TimeZone(element);

    }
    else if (name == "UTCDateTime")
    {
      utctime_ = DateTime(element);

    }
    else if (name == "LocalDateTime")
    {
      localtime_ = DateTime(element);

    }
  }
}

std::string SystemDateTime::ToXml(const std::string& name) const
{
  return ("<"+name+">" + onvif::ToXmlEnum("tt:DateTimeType", datetimetype_) + onvif::ToXml("tt:DaylightSavings", daylightsavings_) + ToXmlClass("tt:TimeZone", timezone_) + ToXmlClass("tt:UTCDateTime", utctime_) + ToXmlClass("tt:LocalDateTime", localtime_) + "</"+name+">");
}

bool SystemDateTime::operator==(const SystemDateTime& rhs) const
{
  return ((datetimetype_ == rhs.datetimetype_) && (daylightsavings_ == rhs.daylightsavings_) && (timezone_ == rhs.timezone_) && (utctime_ == rhs.utctime_) && (localtime_ == rhs.localtime_));
}

}
