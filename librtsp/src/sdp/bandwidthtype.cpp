// bandwidthtype.cpp
//

///// Includes /////

#include "rtsp/sdp/bandwidthtype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Functions /////

std::string ToString(BANDWIDTHTYPE bandwidthtype)
{
  switch (bandwidthtype)
  {
  case BANDWIDTHTYPE_AS:
  {

    return std::string("AS");
  }
  case BANDWIDTHTYPE_CT:
  {

    return std::string("CT");
  }
  default:
  {

    return std::string();
  }
  }
}

BANDWIDTHTYPE BandwidthTypeFromString(const std::string& bandwidthtype)
{
  if (boost::algorithm::iequals(bandwidthtype, "AS"))
  {

    return BANDWIDTHTYPE_AS;
  }
  else if (boost::algorithm::iequals(bandwidthtype, "CT"))
  {

    return BANDWIDTHTYPE_CT;
  }
  else
  {

    return BANDWIDTHTYPE_INVALID;
  }
}

}

}
