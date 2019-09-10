// routingtype.hpp
//

#ifndef IDCHHIC5TMBFOU5C90K2NQHVZYA1PM6G1V
#define IDCHHIC5TMBFOU5C90K2NQHVZYA1PM6G1V

///// Includes /////

#include <string>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Enumerations /////

enum ROUTINGTYPE
{
  ROUTINGTYPE_INVALID = 0,
  ROUTINGTYPE_UNICAST,
  ROUTINGTYPE_MULTICAST
};

///// Proto /////

std::string ToString(ROUTINGTYPE request);
ROUTINGTYPE RoutingTypeFromString(const std::string& request);

}

}

#endif
