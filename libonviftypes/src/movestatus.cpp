// movestatus.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<MOVESTATUS> GetMoveStatus(const std::string& movestatus)
{
  if (boost::iequals(movestatus, "IDLE"))
  {

    return MOVESTATUS_IDLE;
  }
  else if (boost::iequals(movestatus, "MOVING"))
  {

    return MOVESTATUS_MOVING;
  }
  else if (boost::iequals(movestatus, "UNKNOWN"))
  {

    return MOVESTATUS_UNKNOWN;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(MOVESTATUS movestatus)
{
  switch (movestatus)
  {
    case MOVESTATUS_IDLE:
    {
    
      return std::string("IDLE");
    }
    case MOVESTATUS_MOVING:
    {
    
      return std::string("MOVING");
    }
    case MOVESTATUS_UNKNOWN:
    {
    
      return std::string("UNKNOWN");
    }
    default:
    {

      throw std::runtime_error("Invalid MOVESTATUS");
    }
  }
}

}
