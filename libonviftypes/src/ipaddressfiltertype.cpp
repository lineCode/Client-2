// ipaddressfiltertype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<IPADDRESSFILTERTYPE> GetIPAddressFilterType(const std::string& ipaddressfiltertype)
{
  if (boost::iequals(ipaddressfiltertype, "Allow"))
  {

    return IPADDRESSFILTERTYPE_ALLOW;
  }
  else if (boost::iequals(ipaddressfiltertype, "Deny"))
  {

    return IPADDRESSFILTERTYPE_DENY;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(IPADDRESSFILTERTYPE ipaddressfiltertype)
{
  switch (ipaddressfiltertype)
  {
    case IPADDRESSFILTERTYPE_ALLOW:
    {
    
      return std::string("Allow");
    }
    case IPADDRESSFILTERTYPE_DENY:
    {
    
      return std::string("Deny");
    }
    default:
    {

      throw std::runtime_error("Invalid IPADDRESSFILTERTYPE");
    }
  }
}

}
