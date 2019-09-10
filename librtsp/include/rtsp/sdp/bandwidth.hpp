// bandwidth.hpp
//

#ifndef ID5XOAQURD0HDOYN155INVLC26WMLN2SZE
#define ID5XOAQURD0HDOYN155INVLC26WMLN2SZE

///// Includes /////

#include <string>

#include "bandwidthtype.hpp"

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class Bandwidth
{
 public:

  Bandwidth();
  Bandwidth(const std::string& text);
  Bandwidth(BANDWIDTHTYPE bandwidthtype, int bandwidth);
  ~Bandwidth();

  void Clear();
  bool IsValid() const;
  std::string ToString() const;

  bool operator==(const Bandwidth& rhs) const;

  BANDWIDTHTYPE bandwidthtype_;
  int bandwidth_; // Kilobits per second by default, but can be set by an attribute

};

}

}

#endif
