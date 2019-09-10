// mode.hpp
//

#ifndef IDYI477B1PV3EAE04ANBJPG9TXJPTJYO8C
#define IDYI477B1PV3EAE04ANBJPG9TXJPTJYO8C

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Enumerations /////

enum MODE
{
  MODE_INVALID = 0,
  MODE_PLAY,
  MODE_RECORD
};

///// Proto /////

std::string ToString(MODE request);
MODE ModeTypeFromString(const std::string& request);

}

}

#endif
