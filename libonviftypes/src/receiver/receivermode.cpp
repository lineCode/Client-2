// receivermode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Functions /////

boost::optional<RECEIVERMODE> GetReceiverMode(const std::string& receivermode)
{
  if (boost::iequals(receivermode, "AutoConnect"))
  {

    return RECEIVERMODE_AUTOCONNECT;
  }
  else if (boost::iequals(receivermode, "AlwaysConnect"))
  {

    return RECEIVERMODE_ALWAYSCONNECT;
  }
  else if (boost::iequals(receivermode, "NeverConnect"))
  {

    return RECEIVERMODE_NEVERCONNECT;
  }
  else if (boost::iequals(receivermode, "Unknown"))
  {

    return RECEIVERMODE_UNKNOWN;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(const RECEIVERMODE receivermode)
{
  switch (receivermode)
  {
    case RECEIVERMODE_AUTOCONNECT:
    {
    
      return std::string("AutoConnect");
    }
    case RECEIVERMODE_ALWAYSCONNECT:
    {
    
      return std::string("AlwaysConnect");
    }
    case RECEIVERMODE_NEVERCONNECT:
    {
    
      return std::string("NeverConnect");
    }
    case RECEIVERMODE_UNKNOWN:
    {
    
      return std::string("Unknown");
    }
    default:
    {

      throw std::runtime_error("Invalid RECEIVERMODE");
    }
  }
}

}

}
