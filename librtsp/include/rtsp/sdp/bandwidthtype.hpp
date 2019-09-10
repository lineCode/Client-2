// bandwidthtype.hpp
//

#ifndef IDUDI8JTGS981HY6MP6E0U6WR7DRLRKDEU
#define IDUDI8JTGS981HY6MP6E0U6WR7DRLRKDEU

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Enumerations /////

enum BANDWIDTHTYPE
{
  BANDWIDTHTYPE_INVALID = 0,
  BANDWIDTHTYPE_AS, // Application Specific
  BANDWIDTHTYPE_CT // Conference Total
};

///// Functions /////

std::string ToString(BANDWIDTHTYPE bandwidthtype);
BANDWIDTHTYPE BandwidthTypeFromString(const std::string& bandwidthtype);

}

}

#endif
