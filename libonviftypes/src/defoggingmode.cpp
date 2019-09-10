// defoggingmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DEFOGGINGMODE> GetDefoggingMode(const std::string& defoggingmode)
{
  if (boost::iequals(defoggingmode, "OFF"))
  {

    return DEFOGGINGMODE_OFF;
  }
  else if (boost::iequals(defoggingmode, "ON"))
  {

    return DEFOGGINGMODE_ON;
  }
  else if (boost::iequals(defoggingmode, "AUTO"))
  {

    return DEFOGGINGMODE_AUTO;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(DEFOGGINGMODE defoggingmode)
{
  switch (defoggingmode)
  {
    case DEFOGGINGMODE_OFF:
    {
    
      return std::string("OFF");
    }
    case DEFOGGINGMODE_ON:
    {
    
      return std::string("ON");
    }
    case DEFOGGINGMODE_AUTO:
    {
    
      return std::string("AUTO");
    }
    default:
    {

      throw std::runtime_error("Invalid TONECOMPENSATIONMODE");
    }
  }
}

}
