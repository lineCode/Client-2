// mediaprotocol.cpp
//

///// Includes /////

#include "rtsp/sdp/mediaprotocol.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Functions /////

std::string ToString(MEDIAPROTOCOL mediaprotocol)
{
  switch (mediaprotocol)
  {
  case MEDIAPROTOCOL_UDP:
  {

    return std::string("udp");
  }
  case MEDIAPROTOCOL_RTPAVP:
  {

    return std::string("RTP/AVP");
  }
  case MEDIAPROTOCOL_RTPSAVP:
  {

    return std::string("RTP/SAVP");
  }
  default:
  {

    return std::string();
  }
  }
}

MEDIAPROTOCOL MediaProtocolFromString(const std::string& mediaprotocol)
{
  if (boost::algorithm::iequals(mediaprotocol, "udp"))
  {

    return MEDIAPROTOCOL_UDP;
  }
  else if (boost::algorithm::iequals(mediaprotocol, "RTP/AVP"))
  {

    return MEDIAPROTOCOL_RTPAVP;
  }
  else if (boost::algorithm::iequals(mediaprotocol, "RTP/SAVP"))
  {

    return MEDIAPROTOCOL_RTPSAVP;
  }
  else
  {

    return MEDIAPROTOCOL_INVALID;
  }
}

}

}
