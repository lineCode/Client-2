// range.cpp
//

///// Includes /////

#include "rtsp/sdp/range.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Range::Range() :
  starttime_(0)
{

}

Range::Range(double starttime, const boost::optional<double>& endtime) :
  starttime_(starttime),
  endtime_(endtime)
{

}

Range::Range(const std::string& text) :
  starttime_(0)
{
  static const boost::regex regex("[\\s]*npt[\\s]*=[\\s]*(\\d+\\.?\\d*)[\\s]*-?[\\s]*(\\d+\\.?\\d*)?[\\s]*");
  boost::smatch match;
  if (!boost::regex_search(text, match, regex))
  {

    return;
  }

  starttime_ = std::stod(match[1]);

  if (match[2].matched)
  {
    endtime_ = std::stod(match[2]);

  }
}

Range::~Range()
{

}

std::string Range::ToString() const
{
  return (std::string("npt=") + std::to_string(starttime_) + std::string("-") + (endtime_.is_initialized() ? std::to_string(*endtime_) : std::string()));
}

bool Range::operator==(const Range& rhs) const
{
  return ((starttime_ == rhs.starttime_) && (endtime_ == rhs.endtime_));
}

}

}
