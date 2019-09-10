// networkhosttype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<NETWORKHOSTTYPE> GetNetworkHostType(const std::string& networkhosttype)
{
  if (boost::iequals(networkhosttype, "IPv4"))
  {

    return NETWORKHOSTTYPE_IPV4;
  }
  else if (boost::iequals(networkhosttype, "IPv6"))
  {

    return NETWORKHOSTTYPE_IPV6;
  }
  else if (boost::iequals(networkhosttype, "DNS"))
  {

    return NETWORKHOSTTYPE_DNS;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(NETWORKHOSTTYPE networkhosttype)
{
  switch (networkhosttype)
  {
    case NETWORKHOSTTYPE_IPV4:
    {
    
      return std::string("IPv4");
    }
    case NETWORKHOSTTYPE_IPV6:
    {
    
      return std::string("IPv6");
    }
    case NETWORKHOSTTYPE_DNS:
    {
    
      return std::string("DNS");
    }
    default:
    {

      throw std::runtime_error("Invalid NETWORKHOSTTYPE");
    }
  }
}

}
