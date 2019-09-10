// iptype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<IPTYPE> GetIpType(const std::string& iptype)
{
  if (boost::iequals(iptype, "IPv4"))
  {

    return IPTYPE_IPV4;
  }
  else if (boost::iequals(iptype, "IPv6"))
  {

    return IPTYPE_IPV6;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(IPTYPE iptype)
{
  switch (iptype)
  {
    case IPTYPE_IPV4:
    {
    
      return std::string("IPv4");
    }
    case IPTYPE_IPV6:
    {
    
      return std::string("IPv6");
    }
    default:
    {

      throw std::runtime_error("Invalid IPTYPE");
    }
  }
}

}
