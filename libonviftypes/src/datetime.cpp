// datetime.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

#include <chrono>

///// Namespaces /////

namespace onvif
{

///// Classes /////

DateTime::DateTime()
{
  
}

DateTime::DateTime(const boost::optional<Time>& time, const boost::optional<Date>& date) :
  time_(time),
  date_(date)
{
  
}

DateTime::DateTime(const boost::posix_time::ptime& time) :
  time_(time.time_of_day()),
  date_(time.date())
{

}

DateTime::DateTime(const pugi::xml_node& node)
{
  for (const auto& element : node)
  {
    const std::string name = TrimNamespace(element.name());
    if (name == "Time")
    {
      time_ = Time(element);

    }
    else if (name == "Date")
    {
      date_ = Date(element);

    }
  }
}

std::string DateTime::ToXml(const std::string& name) const
{
  return std::string("<"+name+">" + ToXmlClass("tt:Time", time_) + ToXmlClass("tt:Date", date_) + "</"+name+">");
}

boost::optional<boost::posix_time::ptime> DateTime::GetDateTime() const
{
  if (!date_ || !time_)
  {

    return boost::none;
  }
  
  auto time = time_->GetTime();
  auto date = date_->GetDate();
  if (!time || !date)
  {

    return boost::none;
  }

  return boost::posix_time::ptime(*date, *time);
}

int64_t DateTime::GetOffset() const
{
  if (!date_.is_initialized() || !date_->year_.is_initialized() || !date_->month_.is_initialized() || !date_->day_.is_initialized() || !time_.is_initialized() || !time_->hour_.is_initialized() || !time_->minute_.is_initialized() || !time_->second_.is_initialized())
  {
    
    return 0;
  }

  tm time =
  {
    static_cast<int>(*time_->second_),
    static_cast<int>(*time_->minute_),
    static_cast<int>(*time_->hour_),
    *date_->day_,
    (*date_->month_) - 1,
    (*date_->year_) - 1900,
    0,
    0,
    0
  };
#ifdef _WIN32
  const time_t unixtime = _mkgmtime(&time);
#else
  const time_t unixtime = timegm(&time);
#endif
  if (unixtime == -1)
  {

    return 0;
  }

  return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - (unixtime * 1000));
}

bool DateTime::operator==(const DateTime& rhs) const
{
  return ((time_ == rhs.time_) && (date_ == rhs.date_));
}

}
