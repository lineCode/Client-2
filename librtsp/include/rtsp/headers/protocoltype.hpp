// protocoltype.hpp
//

#ifndef IDXA1G1YYM9Y79AK3Q51LE3MZWDWYRPILE
#define IDXA1G1YYM9Y79AK3Q51LE3MZWDWYRPILE

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Enumerations /////

enum PROTOCOLTYPE
{
  PROTOCOLTYPE_INVALID = 0,
  PROTOCOLTYPE_TCP,
  PROTOCOLTYPE_UDP
};

///// Proto /////

std::string ToString(PROTOCOLTYPE protocoltype);
PROTOCOLTYPE ProtocolTypeFromString(const std::string& protocoltype);

}

}

#endif
