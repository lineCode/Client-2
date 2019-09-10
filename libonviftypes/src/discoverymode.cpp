// discoverymode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DISCOVERYMODE> GetDiscoveryMode(const std::string& discoverymode)
{
  if (boost::iequals(discoverymode, "Discoverable"))
  {

    return DISCOVERYMODE_DISCOVERABLE;
  }
  else if (boost::iequals(discoverymode, "NonDiscoverable"))
  {

    return DISCOVERYMODE_NONDISCOVERABLE;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(DISCOVERYMODE discoverymode)
{
  switch (discoverymode)
  {
    case DISCOVERYMODE_DISCOVERABLE:
    {

      return std::string("Discoverable");
    }
    case DISCOVERYMODE_NONDISCOVERABLE:
    {

      return std::string("NonDiscoverable");
    }
    default:
    {

      throw std::runtime_error("Invalid DISCOVERYMODE");
    }
  }
}

}
