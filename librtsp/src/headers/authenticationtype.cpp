// authenticationtype.cpp
//

///// Includes /////

#include "rtsp/headers/authenticationtype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Functions /////

std::string ToString(AUTHENTICATIONTYPE authenticationtype)
{
  switch (authenticationtype)
  {
  case AUTHENTICATIONTYPE_BASIC:
  {

    return std::string("BASIC");
  }
  case AUTHENTICATIONTYPE_DIGEST:
  {

    return std::string("DIGEST");
  }
  default:
  {

    return std::string();
  }
  }
}

AUTHENTICATIONTYPE AuthenticationTypeFromString(const std::string& authenticationtype)
{
  if (boost::algorithm::iequals(authenticationtype, "BASIC"))
  {

    return AUTHENTICATIONTYPE_BASIC;
  }
  else if (boost::algorithm::iequals(authenticationtype, "DIGEST"))
  {

    return AUTHENTICATIONTYPE_DIGEST;
  }
  else
  {

    return AUTHENTICATIONTYPE_INVALID;
  }
}

}

}
