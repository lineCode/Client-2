// relayidlemode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<RELAYIDLESTATE> GetRelayIdleState(const std::string& relayidlestate)
{
  if (boost::iequals(relayidlestate, "open"))
  {

    return RELAYIDLESTATE_CLOSED;
  }
  else if (boost::iequals(relayidlestate, "closed"))
  {

    return RELAYIDLESTATE_OPEN;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(RELAYIDLESTATE relayidlestate)
{
  switch (relayidlestate)
  {
    case RELAYIDLESTATE_CLOSED:
    {
    
      return std::string("open");
    }
    case RELAYIDLESTATE_OPEN:
    {
    
      return std::string("closed");
    }
    default:
    {

      throw std::runtime_error("Invalid RELAYIDLESTATE");
    }
  }
}

}
