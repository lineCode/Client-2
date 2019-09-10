// eflipmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<EFLIPMODE> GetEFlipMode(const std::string& eflipmode)
{
  if (boost::iequals(eflipmode, "OFF"))
  {

    return EFLIPMODE_OFF;
  }
  else if (boost::iequals(eflipmode, "ON"))
  {

    return EFLIPMODE_ON;
  }
  else if (boost::iequals(eflipmode, "Extended"))
  {

    return EFLIPMODE_EXTENDED;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(EFLIPMODE eflipmode)
{
  switch (eflipmode)
  {
    case EFLIPMODE_OFF:
    {

      return std::string("OFF");
    }
    case EFLIPMODE_ON:
    {

      return std::string("ON");
    }
    case EFLIPMODE_EXTENDED:
    {

      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid EFLIPMODE");
    }
  }
}

}
