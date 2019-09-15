// time.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Classes /////

Time::Time()
{

}

Time::Time(const boost::optional<int64_t>& hour, const boost::optional<int64_t>& minute, const boost::optional<int64_t>& second) :
  hour_(hour),
  minute_(minute),
  second_(second)
{

}

Time::Time(const boost::posix_time::time_duration& time) :
  hour_(time.hours()), minute_(time.minutes()), second_(time.seconds())
{
  
}

Time::Time(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Hour")
    {
      hour_ = element.text().as_int();

    }
    else if (name == "Minute")
    {
      minute_ = element.text().as_int();

    }
    else if (name == "Second")
    {
      second_ = element.text().as_int();

    }
  }
}

std::string Time::ToXml(const std::string& name) const
{
  return std::string("<"+name+">" + onvif::ToXml("tt:Hour", hour_) + onvif::ToXml("tt:Minute", minute_) + onvif::ToXml("tt:Second", second_) + "</"+name+">");
}

boost::optional<boost::posix_time::time_duration> Time::GetTime() const
{
  if (!hour_ || !minute_ || !second_)
  {

    return boost::none;
  }
  
  try
  {
  
    return boost::posix_time::time_duration(static_cast<boost::posix_time::time_duration::hour_type>(*hour_), static_cast<boost::posix_time::time_duration::hour_type>(*minute_), static_cast<boost::posix_time::time_duration::hour_type>(*second_));
  }
  catch (...)
  {

    return boost::none;
  }
}

bool Time::operator==(const Time& rhs) const
{
  return ((hour_ == rhs.hour_) && (minute_ == rhs.minute_) && (second_ == rhs.second_));
}

}
