// nettype.hpp
//

#ifndef ID1RYUAE4F29Q7EKXDD50PU3Z7ATJLQ1BM
#define ID1RYUAE4F29Q7EKXDD50PU3Z7ATJLQ1BM

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Enumerations /////

enum NETTYPE
{
  NETTYPE_INVALID = 0,
  NETTYPE_INTERNET
};

///// Functions /////

std::string ToString(NETTYPE nettype);
NETTYPE NetTypeFromString(const std::string& nettype);

}

}

#endif
