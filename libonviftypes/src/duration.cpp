// duration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<Duration> GetDuration(const std::string duration)
{
  static const boost::regex regex(R"regex(^P(?!$)(\d+Y)?(\d+M)?(\d+D)?(T(?=\d+(?:\.\d+)?[HMS])(\d+(?:\.\d+)?H)?(\d+(?:\.\d+)?M)?(\d+(?:\.\d+)?S)?)?$)regex");
  boost::smatch smatch;
  if (!boost::regex_search(duration, smatch, regex))
  {

    return boost::none;
  }

  Duration result;
  try
  {
    bool time = false;
    for (size_t i = 1; i < smatch.size(); ++i)
    {
      const std::string& match = smatch[i].str();
      if (match.empty())
      {

        continue;
      }

      if (match.front() == 'T')
      {
        time = true;
        continue;
      }

      switch (match.back())
      {
        case 'Y':
        {
          result.years_ = std::stoi(match);
          break;
        }
        case 'M':
        {
          if (time)
          {
            const double minutes = std::stod(match);
            result.minutes_ += static_cast<int64_t>(minutes);
            result.seconds_ = static_cast<int64_t>(minutes * 60) % 60;
          }
          else
          {
            result.months_ = std::stoi(match);

          }
          break;
        }
        case 'D':
        {
          result.days_ = std::stoi(match);
          break;
        }
        case 'H':
        {
          const double hours = std::stod(match);
          result.hours_ += static_cast<int64_t>(hours);
          result.minutes_ = static_cast<int64_t>(hours * 60) % 60;
          break;
        }
        case 'S':
        {
          const double seconds = std::stod(match);
          result.seconds_ += static_cast<int64_t>(seconds);
          result.milliseconds_ = static_cast<int64_t>(seconds * 1000.0) % 1000;
          break;
        }
        default:
        {

          break;
        }
      }
    }
  }
  catch (...)
  {

    return boost::none;
  }
  return result;
}

///// Classes /////

Duration::Duration() :
  years_(0),
  months_(0),
  days_(0),
  hours_(0),
  minutes_(0),
  seconds_(0),
  milliseconds_(0)
{
  
}

Duration::Duration(const int64_t seconds) :
  years_(0),
  months_(0),
  days_(0),
  hours_(0),
  minutes_(0),
  seconds_(0),
  milliseconds_(0)
{
  // We don't include months and years because they are unreliable in how long they are
  days_ = seconds / 86400;
  hours_ = (seconds % 86400) / 3600;
  minutes_ = (seconds % 3600) / 60;
  seconds_ = seconds % 60;
}

Duration::Duration(const int64_t years, const int64_t months, const int64_t days, const int64_t hours, const int64_t minutes, const int64_t seconds) :
  years_(years),
  months_(months),
  days_(days),
  hours_(hours),
  minutes_(minutes),
  seconds_(seconds),
  milliseconds_(0)
{

}

Duration::Duration(const int64_t years, const int64_t months, const int64_t days, const int64_t hours, const int64_t minutes, const int64_t seconds, const int64_t milliseconds) :
  years_(years),
  months_(months),
  days_(days),
  hours_(hours),
  minutes_(minutes),
  seconds_(seconds),
  milliseconds_(milliseconds)
{

}

int64_t Duration::ToMilliseconds() const
{
  return (((((((((years_ * 365) + (months_ * 31) + days_) * 24) + hours_) * 60) + minutes_) * 60) + seconds_) * 1000) + milliseconds_; // This is not exact(because years and months differ, but this is the maximum and close)
}

std::string Duration::ToString() const
{
  if ((years_ == 0) && (months_ == 0) && (days_ == 0) && (hours_ == 0) && (minutes_ == 0) && (seconds_ == 0) && (milliseconds_ == 0))
  {

    return std::string("PT0S");
  }

  std::string date;
  if (years_)
  {
    date += std::to_string(years_) + std::string("Y");

  }
  if (months_)
  {
    date += std::to_string(months_) + std::string("M");

  }
  if (days_)
  {
    date += std::to_string(days_) + std::string("D");

  }

  std::string time;
  if (hours_ || minutes_ || seconds_ || milliseconds_)
  {
    time += std::string("T");
    if (hours_)
    {
      time += std::to_string(hours_) + std::string("H");

    }
    if (minutes_)
    {
      time += std::to_string(minutes_) + std::string("M");

    }
    if (seconds_ && milliseconds_)
    {
      time += std::to_string(seconds_ + (milliseconds_ / 1000)) + std::string(".") + std::to_string(milliseconds_ % 1000) + std::string("S");

    }
    else if (seconds_ && !milliseconds_)
    {
      time += std::to_string(seconds_) + std::string("S");

    }
    else if (!seconds_ && milliseconds_)
    {
      time += std::to_string(milliseconds_ / 1000) + std::string(".") + std::to_string(milliseconds_ % 1000) + std::string("S");

    }
  }

  return std::string("P") + date + time;
}

bool Duration::operator==(const Duration& rhs) const
{
  return ((years_ == rhs.years_) && (months_ == rhs.months_) && (days_ == rhs.days_) && (hours_ == rhs.hours_) && (minutes_ == rhs.minutes_) && (seconds_ == rhs.seconds_) && (milliseconds_ == rhs.milliseconds_));
}

}
