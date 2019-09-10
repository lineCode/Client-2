// addresstype.hpp
//

#ifndef IDQQJRY22NRTJMSGVSOMK5GVY8RFQTOHHM
#define IDQQJRY22NRTJMSGVSOMK5GVY8RFQTOHHM

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Enumerations /////

enum ADDRESSTYPE
{
  ADDRESSTYPE_INVALID = 0,
  ADDRESSTYPE_IP4,
  ADDRESSTYPE_IP6
};

///// Functions /////

std::string ToString(ADDRESSTYPE addresstype);
ADDRESSTYPE AddressTypeFromString(const std::string& addresstype);

}

}

#endif
