// relaylogicalstate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<RELAYLOGICALSTATE> GetRelayLogicalState(const std::string& relaylogicalstate)
{
  if (boost::iequals(relaylogicalstate, "active"))
  {

    return RELAYLOGICALSTATE_ACTIVE;
  }
  else if (boost::iequals(relaylogicalstate, "inactive"))
  {

    return RELAYLOGICALSTATE_INACTIVE;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(RELAYLOGICALSTATE relaylogicalstate)
{
  switch (relaylogicalstate)
  {
    case RELAYLOGICALSTATE_ACTIVE:
    {
    
      return std::string("active");
    }
    case RELAYLOGICALSTATE_INACTIVE:
    {
    
      return std::string("inactive");
    }
    default:
    {

      throw std::runtime_error("Invalid RELAYLOGICALSTATE");
    }
  }
}
}
