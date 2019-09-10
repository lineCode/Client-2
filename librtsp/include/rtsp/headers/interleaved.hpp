// interleaved.hpp
//

#ifndef IDRWDI2SYAB9C96SK1G31MWW4ERFSJLVHJ
#define IDRWDI2SYAB9C96SK1G31MWW4ERFSJLVHJ

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

class Interleaved
{
public:

  Interleaved();
  Interleaved(const boost::optional<uint8_t>& rtp, const boost::optional<uint8_t>& rtcp);
  Interleaved(const std::string& text);
  ~Interleaved();

  std::string ToString() const;

  operator bool() const;
  bool operator==(const Interleaved& rhs) const;

  boost::optional<uint8_t> rtp_;
  boost::optional<uint8_t> rtcp_;

};

}

}

#endif
