// contenttype.cpp
//

///// Includes /////

#include "rtsp/headers/contenttype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Functions /////

std::string ToString(CONTENTTYPE request)
{
  switch (request)
  {
  case CONTENTTYPE_APPLICATIONSDP:
  {

    return std::string("application/sdp");
  }
  case CONTENTTYPE_TEXTPARAMETERS:
  {

    return std::string("text/parameters");
  }
  default:
  {

    return std::string();
  }
  }
}

CONTENTTYPE ContentTypeFromString(const std::string& request)
{
  if (boost::algorithm::iequals(request, "application/sdp"))
  {

    return CONTENTTYPE_APPLICATIONSDP;
  }
  else if (boost::algorithm::iequals(request, "text/parameters"))
  {

    return CONTENTTYPE_TEXTPARAMETERS;
  }
  else
  {

    return CONTENTTYPE_INVALID;
  }
}

}

}
