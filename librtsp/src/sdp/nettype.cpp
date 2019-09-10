// nettype.cpp
//

///// Includes /////

#include "rtsp/sdp/nettype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Functions /////

std::string ToString(NETTYPE nettype)
{
  switch (nettype)
  {
  case NETTYPE_INTERNET:
  {

    return std::string("IN");
  }
  default:
  {

    return std::string();
  }
  }
}

NETTYPE NetTypeFromString(const std::string& nettype)
{
  if (boost::algorithm::iequals(nettype, "IN"))
  {

    return NETTYPE_INTERNET;
  }
  else
  {

    return NETTYPE_INVALID;
  }
}

}

}
