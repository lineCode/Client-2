// authenticationtype.hpp
//

#ifndef IDRTRIF9JI7SZ5X8P26NP99A3SHA4LZ7TS
#define IDRTRIF9JI7SZ5X8P26NP99A3SHA4LZ7TS

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Enumerations /////

enum AUTHENTICATIONTYPE
{
  AUTHENTICATIONTYPE_INVALID = 0,
  AUTHENTICATIONTYPE_BASIC,
  AUTHENTICATIONTYPE_DIGEST
};

///// Proto /////

std::string ToString(AUTHENTICATIONTYPE authenticationtype);
AUTHENTICATIONTYPE AuthenticationTypeFromString(const std::string& authenticationtype);

}

}

#endif
