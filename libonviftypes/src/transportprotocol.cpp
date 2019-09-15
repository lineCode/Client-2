// transportprotocol.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<TRANSPORTPROTOCOL> GetTransportProtocol(const std::string& transportprotocol)
{
  if (boost::iequals(transportprotocol, "UDP"))
  {

    return TRANSPORTPROTOCOL_UDP;
  }
  else if (boost::iequals(transportprotocol, "TCP"))
  {

    return TRANSPORTPROTOCOL_TCP;
  }
  else if (boost::iequals(transportprotocol, "RTSP"))
  {

    return TRANSPORTPROTOCOL_RTSP;
  }
  else if (boost::iequals(transportprotocol, "HTTP"))
  {

    return TRANSPORTPROTOCOL_HTTP;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(TRANSPORTPROTOCOL transportprotocol)
{
  switch (transportprotocol)
  {
    case TRANSPORTPROTOCOL_UDP:
    {
    
      return std::string("UDP");
    }
    case TRANSPORTPROTOCOL_TCP:
    {
    
      return std::string("TCP");
    }
    case TRANSPORTPROTOCOL_RTSP:
    {
    
      return std::string("RTSP");
    }
    case TRANSPORTPROTOCOL_HTTP:
    {
    
      return std::string("HTTP");
    }
    default:
    {

      throw std::runtime_error("Invalid TRANSPORTPROTOCOL");
    }
  }
}

}
