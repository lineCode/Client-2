// requesttype.cpp
//

///// Includes /////

#include "rtsp/headers/requesttype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Functions /////

std::string ToString(REQUESTTYPE request)
{
  switch (request)
  {
  case REQUESTTYPE_DESCRIBE:
  {

    return std::string("DESCRIBE");
  }
  case REQUESTTYPE_GETPARAMETER:
  {

    return std::string("GET_PARAMETER");
  }
  case REQUESTTYPE_OPTIONS:
  {

    return std::string("OPTIONS");
  }
  case REQUESTTYPE_PAUSE:
  {

    return std::string("PAUSE");
  }
  case REQUESTTYPE_PLAY:
  {

    return std::string("PLAY");
  }
  case REQUESTTYPE_SETUP:
  {

    return std::string("SETUP");
  }
  case REQUESTTYPE_SETPARAMETER:
  {

    return std::string("SET_PARAMETER");
  }
  case REQUESTTYPE_TEARDOWN:
  {

    return std::string("TEARDOWN");
  }
  default:
  {

    return std::string();
  }
  }
}

REQUESTTYPE RequestTypeFromString(const std::string& request)
{
  if (boost::algorithm::iequals(request, "DESCRIBE"))
  {

    return REQUESTTYPE_DESCRIBE;
  }
  else if (boost::algorithm::iequals(request, "GET_PARAMETER"))
  {

    return REQUESTTYPE_GETPARAMETER;
  }
  else if (boost::algorithm::iequals(request, "OPTIONS"))
  {

    return REQUESTTYPE_OPTIONS;
  }
  else if (boost::algorithm::iequals(request, "PAUSE"))
  {

    return REQUESTTYPE_PAUSE;
  }
  else if (boost::algorithm::iequals(request, "PLAY"))
  {

    return REQUESTTYPE_PLAY;
  }
  else if (boost::algorithm::iequals(request, "SETUP"))
  {

    return REQUESTTYPE_SETUP;
  }
  else if (boost::algorithm::iequals(request, "SET_PARAMETER"))
  {

    return REQUESTTYPE_SETPARAMETER;
  }
  else if (boost::algorithm::iequals(request, "TEARDOWN"))
  {

    return REQUESTTYPE_TEARDOWN;
  }
  else
  {

    return REQUESTTYPE_INVALID;
  }
}

}

}
