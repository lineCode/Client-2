// protocoltype.cpp
//

///// Includes /////

#include "rtsp/headers/protocoltype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Functions /////

std::string ToString(PROTOCOLTYPE protocoltype)
{
  switch (protocoltype)
  {
  case PROTOCOLTYPE_TCP:
  {

    return std::string("TCP");
  }
  case PROTOCOLTYPE_UDP:
  {

    return std::string("UDP");
  }
  default:
  {

    return std::string();
  }
  }
}

PROTOCOLTYPE ProtocolTypeFromString(const std::string& protocoltype)
{
  if (boost::algorithm::iequals(protocoltype, "TCP"))
  {

    return PROTOCOLTYPE_TCP;
  }
  else if (boost::algorithm::iequals(protocoltype, "UDP"))
  {

    return PROTOCOLTYPE_UDP;
  }
  else
  {

    return PROTOCOLTYPE_INVALID;
  }
}

}

}
