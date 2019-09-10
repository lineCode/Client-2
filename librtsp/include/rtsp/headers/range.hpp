// range.hpp
//

#ifndef IDASSO7AGB0PDTFK76HNCEHWP6WQ63F3P1
#define IDASSO7AGB0PDTFK76HNCEHWP6WQ63F3P1

///// Includes /////

#include <boost/optional.hpp>
#include <stdint.h>
#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Classes /////

class Range
{
public:

  Range();
  Range(const bool npt, const uint64_t starttime, const boost::optional<uint64_t>& endtime);
  Range(const std::string& text);
  ~Range();

  std::string ToString() const;

  bool operator==(const Range& rhs) const;

  bool npt_; // npt= or clock=
  uint64_t starttime_;
  boost::optional<uint64_t> endtime_;

};

}

}

#endif
