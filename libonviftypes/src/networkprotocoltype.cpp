// networkprotocoltype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<NETWORKPROTOCOLTYPE> GetNetworkProtocolType(const std::string& networkprotocoltype)
{
  if (boost::iequals(networkprotocoltype, "HTTP"))
  {

    return NETWORKPROTOCOLTYPE_HTTP;
  }
  else if (boost::iequals(networkprotocoltype, "HTTPS"))
  {

    return NETWORKPROTOCOLTYPE_HTTPS;
  }
  else if (boost::iequals(networkprotocoltype, "RTSP"))
  {

    return NETWORKPROTOCOLTYPE_RTSP;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(NETWORKPROTOCOLTYPE networkprotocoltype)
{
  switch (networkprotocoltype)
  {
    case NETWORKPROTOCOLTYPE_HTTP:
    {
    
      return std::string("HTTP");
    }
    case NETWORKPROTOCOLTYPE_HTTPS:
    {
    
      return std::string("HTTPS");
    }
    case NETWORKPROTOCOLTYPE_RTSP:
    {
    
      return std::string("RTSP");
    }
    default:
    {

      throw std::runtime_error("Invalid NETWORKPROTOCOLTYPE");
    }
  }
}

}
