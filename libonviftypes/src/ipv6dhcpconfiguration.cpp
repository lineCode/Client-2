// ipv6dhcpconfiguration.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<IPV6DHCPCONFIGURATION> GetIPv6DHCPConfiguration(const std::string& ipv6dhcpconfiguration)
{
  if (boost::iequals(ipv6dhcpconfiguration, "Auto"))
  {

    return IPV6DHCPCONFIGURATION_AUTO;
  }
  else if (boost::iequals(ipv6dhcpconfiguration, "Stateful"))
  {

    return IPV6DHCPCONFIGURATION_STATEFUL;
  }
  else if (boost::iequals(ipv6dhcpconfiguration, "Stateless"))
  {

    return IPV6DHCPCONFIGURATION_STATELESS;
  }
  else if (boost::iequals(ipv6dhcpconfiguration, "Off"))
  {

    return IPV6DHCPCONFIGURATION_OFF;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(IPV6DHCPCONFIGURATION ipv6dhcpconfiguration)
{
  switch (ipv6dhcpconfiguration)
  {
    case IPV6DHCPCONFIGURATION_AUTO:
    {
    
      return std::string("Auto");
    }
    case IPV6DHCPCONFIGURATION_STATEFUL:
    {
    
      return std::string("Stateful");
    }
    case IPV6DHCPCONFIGURATION_STATELESS:
    {
    
      return std::string("Stateless");
    }
    case IPV6DHCPCONFIGURATION_OFF:
    {
    
      return std::string("Off");
    }
    default:
    {

      throw std::runtime_error("Invalid IPV6DHCPCONFIGURATION");
    }
  }
}

}
