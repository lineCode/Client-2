// timing.cpp
//

///// Includes /////

#include "rtsp/sdp/timing.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Timing::Timing()
{

}

Timing::Timing(const std::vector< std::pair<uint64_t, uint64_t> >& timings) :
  timings_(timings)
{

}

Timing::Timing(const std::vector<std::string>& timings)
{
  for (const auto& timing : timings)
  {
    AddTiming(timing);

  }
}

Timing::~Timing()
{

}

void Timing::AddTiming(const std::string& timing)
{
  static const boost::regex regex("[\\s]*t[\\s]*=[\\s]*([\\d])+[\\s]*([\\d]+)");
  boost::smatch match;
  if (!boost::regex_search(timing, match, regex))
  {

    return;
  }

  timings_.push_back(std::make_pair(std::stoull(match[1].str()), std::stoull(match[2].str())));
}

bool Timing::IsPermanent() const
{
  for (const auto& timing : timings_)
  {
    if (timing.first == 0)
    {

      return true;
    }
  }

  return false;
}

bool Timing::IsUnbounded() const
{
  for (const auto& timing : timings_)
  {
    if (timing.second == 0)
    {

      return true;
    }
  }

  return false;
}

void Timing::Clear()
{
  timings_.clear();

}

std::vector<std::string> Timing::ToString() const
{
  std::vector<std::string> result;
  for (const auto& timing : timings_)
  {
    result.push_back(std::string("t=") + std::to_string(timing.first) + std::string(" ") + std::to_string(timing.second));

  }
  return result;
}

bool Timing::operator==(const Timing& rhs) const
{
  return (timings_ == rhs.timings_);
}

}

}
