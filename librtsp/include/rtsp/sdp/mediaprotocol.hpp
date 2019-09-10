// mediaprotocol.hpp
//

#ifndef IDQOYE15R2P3LMP7OKISQV40MPQLXHVLWF
#define IDQOYE15R2P3LMP7OKISQV40MPQLXHVLWF

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Enumerations /////

enum MEDIAPROTOCOL
{
  MEDIAPROTOCOL_INVALID = 0,
  MEDIAPROTOCOL_UDP,
  MEDIAPROTOCOL_RTPAVP,
  MEDIAPROTOCOL_RTPSAVP
};

///// Proto /////

std::string ToString(MEDIAPROTOCOL mediaprotocol);
MEDIAPROTOCOL MediaProtocolFromString(const std::string& mediaprotocol);

}

}

#endif
