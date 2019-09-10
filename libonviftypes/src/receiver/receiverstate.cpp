// receiverstate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Functions /////

boost::optional<RECEIVERSTATE> GetReceiverState(const std::string& receiverstate)
{
  if (boost::iequals(receiverstate, "NotConnected"))
  {

    return RECEIVERSTATE_NOTCONNECTED;
  }
  else if (boost::iequals(receiverstate, "Connecting"))
  {

    return RECEIVERSTATE_CONNECTING;
  }
  else if (boost::iequals(receiverstate, "Connected"))
  {

    return RECEIVERSTATE_CONNECTED;
  }
  else if (boost::iequals(receiverstate, "Unknown"))
  {

    return RECEIVERSTATE_UNKNOWN;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(const RECEIVERSTATE receiverstate)
{
  switch (receiverstate)
  {
    case RECEIVERSTATE_NOTCONNECTED:
    {
    
      return std::string("NotConnected");
    }
    case RECEIVERSTATE_CONNECTING:
    {
    
      return std::string("Connecting");
    }
    case RECEIVERSTATE_CONNECTED:
    {
    
      return std::string("Connected");
    }
    case RECEIVERSTATE_UNKNOWN:
    {
    
      return std::string("Unknown");
    }
    default:
    {

      throw std::runtime_error("Invalid RECEIVESTATE");
    }
  }
}

}

}
