// backlightcompensationmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<BACKLIGHTCOMPENSATIONMODE> GetBacklightCompensationMode(const std::string& backlightcompensation)
{
  if (boost::iequals(backlightcompensation, "OFF"))
  {

    return BACKLIGHTCOMPENSATIONMODE_OFF;
  }
  else if (boost::iequals(backlightcompensation, "ON"))
  {

    return BACKLIGHTCOMPENSATIONMODE_ON;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(BACKLIGHTCOMPENSATIONMODE backlightcompensation)
{
  switch (backlightcompensation)
  {
    case BACKLIGHTCOMPENSATIONMODE_OFF:
    {

      return std::string("OFF");
    }
    case BACKLIGHTCOMPENSATIONMODE_ON:
    {

      return std::string("ON");
    }
    default:
    {

      throw std::runtime_error("Invalid BACKLIGHTCOMPENSATION");
    }
  }
}

}
