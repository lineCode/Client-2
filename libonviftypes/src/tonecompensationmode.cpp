// tonecompensationmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<TONECOMPENSATIONMODE> GetToneCompensationMode(const std::string& tonecompensationmode)
{
  if (boost::iequals(tonecompensationmode, "OFF"))
  {

    return TONECOMPENSATIONMODE_OFF;
  }
  else if (boost::iequals(tonecompensationmode, "ON"))
  {

    return TONECOMPENSATIONMODE_ON;
  }
  else if (boost::iequals(tonecompensationmode, "AUTO"))
  {

    return TONECOMPENSATIONMODE_AUTO;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(TONECOMPENSATIONMODE tonecompensationmode)
{
  switch (tonecompensationmode)
  {
    case TONECOMPENSATIONMODE_OFF:
    {
    
      return std::string("OFF");
    }
    case TONECOMPENSATIONMODE_ON:
    {
    
      return std::string("ON");
    }
    case TONECOMPENSATIONMODE_AUTO:
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
