// range.cpp
//

///// Includes /////

#include "rtsp/headers/range.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Methods /////

Range::Range() :
  npt_(true),
  starttime_(0)
{

}

Range::Range(const bool npt, const uint64_t starttime, const boost::optional<uint64_t>& endtime) :
  npt_(npt),
  starttime_(starttime),
  endtime_(endtime)
{

}

Range::Range(const std::string& text) :
  starttime_(0)
{
  static const boost::regex nptregex("Range:[\\s]*npt[\\s]*=[\\s]*(\\d+\\.?\\d*)[\\s]*-?[\\s]*(\\d+\\.?\\d*)?[\\s]*");
  static const boost::regex clockregex("Range:[\\s]*clock[\\s]*=[\\s]*(\\d\\d\\d\\d)(\\d\\d)(\\d\\d)T(\\d\\d)(\\d\\d)(\\d\\d)(?:\\.?(\\d\\d\\d))Z[\\s]*(?:-[\\s]*(\\d\\d\\d\\d)(\\d\\d)(\\d\\d)T(\\d\\d)(\\d\\d)(\\d\\d)(?:\\.?(\\d\\d\\d))Z)?[\\s]*");
  boost::smatch match;
  if (boost::regex_search(text, match, nptregex))
  {
    npt_ = true;
    starttime_ = static_cast<uint64_t>(std::stod(match[1])) * 1000;
    if (match[2].matched)
    {
      endtime_ = static_cast<uint64_t>(std::stod(match[2])) * 1000;

    }
  }
  else if (boost::regex_search(text, match, clockregex))
  {
    npt_ = false;
    try
    {
      struct tm t;
      t.tm_isdst = 1;
      t.tm_year = boost::lexical_cast<int>(match[1]) - 1900;
      t.tm_mon = boost::lexical_cast<int>(match[2]) - 1;
      t.tm_mday = boost::lexical_cast<int>(match[3]);
      t.tm_hour = boost::lexical_cast<int>(match[4]);
      t.tm_min = boost::lexical_cast<int>(match[5]);
      t.tm_sec = boost::lexical_cast<int>(match[6]);
#ifdef _WIN32
      starttime_ = _mkgmtime(&t);
#else
      starttime_ = timegm(&t);
#endif
      if (starttime_ == std::numeric_limits<decltype(starttime_)>::max())
      {
        starttime_ = 0;

      }
      else
      {
        starttime_ = starttime_ * 1000;

      }

      if (match[7].matched)
      {
        starttime_ += boost::lexical_cast<uint64_t>(match[7]);//TODO do we need to multiply by 1000?

      }

      if (match[8].matched && match[9].matched && match[10].matched && match[11].matched && match[12].matched && match[13].matched)
      {
        try
        {
          struct tm t;
          t.tm_isdst = false;
          t.tm_year = boost::lexical_cast<int>(match[8]) - 1900;
          t.tm_mon = boost::lexical_cast<int>(match[9]) - 1;
          t.tm_mday = boost::lexical_cast<int>(match[10]);
          t.tm_hour = boost::lexical_cast<int>(match[11]);
          t.tm_min = boost::lexical_cast<int>(match[12]);
          t.tm_sec = boost::lexical_cast<int>(match[13]);
#ifdef _WIN32
          endtime_ = _mkgmtime(&t);
#else
          endtime_ = timegm(&t);
#endif
          if (endtime_ == std::numeric_limits<decltype(starttime_)>::max())
          {
            endtime_ = 0;

          }
          else
          {
            *endtime_ = *endtime_ * 1000;

          }

          if (match[14].matched)
          {
            *endtime_ += boost::lexical_cast<uint64_t>(match[14]);

          }
        }
        catch (...)
        {

        }
      }
    }
    catch (...)
    {

    }
  }
}

Range::~Range()
{

}

std::string Range::ToString() const
{
  if (npt_)
  {
    const std::string starttime = std::to_string(starttime_ / 1000) + "." + (boost::format("%03d") % std::to_string(starttime_ % 1000)).str();
    std::string endtime;
    if (endtime_.is_initialized())
    {
      endtime = std::to_string(*endtime_ / 1000) + "." + (boost::format("%03d") % std::to_string(*endtime_ % 1000)).str();

    }
    return (std::string("Range: npt=") + starttime + std::string("-") + endtime);
  }
  else
  {
    const std::string starttime = boost::posix_time::to_iso_string(boost::posix_time::from_time_t(starttime_ / 1000)) + "." + (boost::format("%03d") % std::to_string(starttime_ % 1000)).str() + "Z";
    std::string endtime;
    if (endtime_.is_initialized())
    {
      endtime = boost::posix_time::to_iso_string(boost::posix_time::from_time_t(*endtime_ / 1000)) + "." + (boost::format("%03d") % std::to_string(*endtime_ % 1000)).str() + "Z";

    }
    return (std::string("Range: clock=") + starttime + std::string("-") + endtime);
  }
}

bool Range::operator==(const Range& rhs) const
{
  return ((starttime_ == rhs.starttime_) && (endtime_ == rhs.endtime_));
}

}

}
