// proxytype.cpp
//

///// Includes /////

#include "socket/proxytype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace sock
{

///// Functions /////

std::string ToString(PROXYTYPE proxytype)
{
  switch (proxytype)
  {
    case PROXYTYPE_HTTP:
    {

      return std::string("HTTP");
    }
    case PROXYTYPE_SOCKS5:
    {
  
      return std::string("socks5");
    }
    default:
    {
      
      return std::string("none");
    }
  }
}

PROXYTYPE FromString(const std::string& proxytype)
{
  if (boost::iequals(proxytype, "HTTP"))
  {

    return PROXYTYPE_HTTP;
  }
  else if (boost::iequals(proxytype, "socks5"))
  {
  
    return PROXYTYPE_SOCKS5;
  }
  else
  {
 
    return PROXYTYPE_NONE;
  }
}

PROXYTYPE FromInt(int proxytype)
{
  switch (proxytype)
  {
    case PROXYTYPE_HTTP:
    {

      return PROXYTYPE_HTTP;
    }
    case PROXYTYPE_SOCKS5:
    {
    
      return PROXYTYPE_SOCKS5;
    }
    default:
    {
  
      return PROXYTYPE_NONE;
    }
  }
}

}
