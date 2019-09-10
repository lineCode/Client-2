// addresstype.cpp
//

///// Includes /////

#include "rtsp/sdp/addresstype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Functions /////

std::string ToString(ADDRESSTYPE addresstype)
{
  switch (addresstype)
  {
  case ADDRESSTYPE_IP4:
  {

    return std::string("IP4");
  }
  case ADDRESSTYPE_IP6:
  {

    return std::string("IP6");
  }
  default:
  {

    return std::string();
  }
  }
}

ADDRESSTYPE AddressTypeFromString(const std::string& addresstype)
{
  if (boost::algorithm::iequals(addresstype, "IP4"))
  {

    return ADDRESSTYPE_IP4;
  }
  else if (boost::algorithm::iequals(addresstype, "IP6"))
  {

    return ADDRESSTYPE_IP6;
  }
  else
  {

    return ADDRESSTYPE_INVALID;
  }
}

}

}
