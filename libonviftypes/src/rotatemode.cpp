// rotatemode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<ROTATEMODE> GetRotateMode(const std::string& rotatemode)
{
  if (boost::iequals(rotatemode, "OFF"))
  {

    return ROTATE_OFF;
  }
  else if (boost::iequals(rotatemode, "ON"))
  {

    return ROTATE_ON;
  }
  else if (boost::iequals(rotatemode, "AUTO"))
  {

    return ROTATE_AUTO;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(ROTATEMODE rotatemode)
{
  switch (rotatemode)
  {
    case ROTATE_OFF:
    {
    
      return std::string("OFF");
    }
    case ROTATE_ON:
    {
    
      return std::string("ON");
    }
    case ROTATE_AUTO:
    {
    
      return std::string("AUTO");
    }
    default:
    {

      throw std::runtime_error("Invalid ROTATEMODE");
    }
  }
}

}
