// ssrc.hpp
//

#ifndef ID5XOAQURD0HDOYN155INVLC26WMLN2SZB
#define ID5XOAQURD0HDOYN155INVLC26WMLN2SZB

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

class SSrc
{
public:

  SSrc();
  SSrc(const std::string& text);
  SSrc(const boost::optional<uint32_t>& ssrc);
  ~SSrc();

  void Clear();
  std::string ToString() const;

  bool operator==(const SSrc& rhs) const;

  boost::optional<uint32_t> ssrc_;

};

}

}

#endif
