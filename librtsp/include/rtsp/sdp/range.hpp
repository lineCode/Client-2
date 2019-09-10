// range.hpp
//

#ifndef IDKGZFZLJEVFHJD61IHZO51HPMJ0LNT1VK
#define IDKGZFZLJEVFHJD61IHZO51HPMJ0LNT1VK

///// Includes /////

#include <boost/optional.hpp>
#include <string>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class Range
{
public:

  Range();
  Range(double starttime, const boost::optional<double>& endtime);
  Range(const std::string& text);
  ~Range();

  std::string ToString() const;

  bool operator==(const Range& rhs) const;

  double starttime_;
  boost::optional<double> endtime_;

};

}

}

#endif
