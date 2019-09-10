// relaymode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<RELAYMODE> GetRelayMode(const std::string& relaymode)
{
  if (boost::iequals(relaymode, "Bistable"))
  {

    return RELAYMODE_BISTABLE;
  }
  else if (boost::iequals(relaymode, "Monostable"))
  {

    return RELAYMODE_MONOSTABLE;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(RELAYMODE relaymode)
{
  switch (relaymode)
  {
    case RELAYMODE_BISTABLE:
    {
    
      return std::string("Bistable");
    }
    case RELAYMODE_MONOSTABLE:
    {
    
      return std::string("Monostable");
    }
    default:
    {

      throw std::runtime_error("Invalid RELAYMODE");
    }
  }
}
}
