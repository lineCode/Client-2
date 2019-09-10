// routingtype.cpp
//

///// Includes /////

#include "rtsp/headers/routingtype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Functions /////

std::string ToString(ROUTINGTYPE request)
{
  switch (request)
  {
  case ROUTINGTYPE_UNICAST:
  {

    return std::string("unicast");
  }
  case ROUTINGTYPE_MULTICAST:
  {

    return std::string("multicast");
  }
  default:
  {

    return std::string();
  }
  }
}

ROUTINGTYPE RoutingTypeFromString(const std::string& request)
{
  if (boost::algorithm::iequals(request, "unicast"))
  {

    return ROUTINGTYPE_UNICAST;
  }
  else if (boost::algorithm::iequals(request, "multicast"))
  {

    return ROUTINGTYPE_MULTICAST;
  }
  else
  {

    return ROUTINGTYPE_INVALID;
  }
}

}

}
