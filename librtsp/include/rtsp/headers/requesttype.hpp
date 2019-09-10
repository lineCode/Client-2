// requesttype.hpp
//

#ifndef IDG0ZY448MKE3VU72JDZAN9KA8VENUP10L
#define IDG0ZY448MKE3VU72JDZAN9KA8VENUP10L

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Enumerations /////

enum REQUESTTYPE
{
  REQUESTTYPE_INVALID = 0,
  REQUESTTYPE_DESCRIBE,
  REQUESTTYPE_GETPARAMETER,
  REQUESTTYPE_OPTIONS,
  REQUESTTYPE_PAUSE,
  REQUESTTYPE_PLAY,
  REQUESTTYPE_SETUP,
  REQUESTTYPE_SETPARAMETER,
  REQUESTTYPE_TEARDOWN

};

///// Proto /////

std::string ToString(REQUESTTYPE request);
REQUESTTYPE RequestTypeFromString(const std::string& request);

}

}

#endif
