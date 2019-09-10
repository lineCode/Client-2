// contenttype.hpp
//

#ifndef IDHZ81T1NZWSOS192QBXDP1J4OEQ9H8FIE
#define IDHZ81T1NZWSOS192QBXDP1J4OEQ9H8FIE

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Enumerations /////

enum CONTENTTYPE
{
  CONTENTTYPE_INVALID = 0,
  CONTENTTYPE_APPLICATIONSDP,
  CONTENTTYPE_TEXTPARAMETERS
};

///// Proto /////

std::string ToString(CONTENTTYPE request);
CONTENTTYPE ContentTypeFromString(const std::string& request);

}

}

#endif
