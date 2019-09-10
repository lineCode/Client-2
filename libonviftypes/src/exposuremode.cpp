// exposuremode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<EXPOSUREMODE> GetExposureMode(const std::string& exposuremode)
{
  if (boost::iequals(exposuremode, "AUTO"))
  {

    return EXPOSUREMODE_AUTO;
  }
  else if (boost::iequals(exposuremode, "MANUAL"))
  {

    return EXPOSUREMODE_MANUAL;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(EXPOSUREMODE exposuremode)
{
  switch (exposuremode)
  {
    case EXPOSUREMODE_AUTO:
    {

      return std::string("AUTO");
    }
    case EXPOSUREMODE_MANUAL:
    {

      return std::string("MANUAL");
    }
    default:
    {

      throw std::runtime_error("Invalid EXPOSUREMODE");
    }
  }
}

}
