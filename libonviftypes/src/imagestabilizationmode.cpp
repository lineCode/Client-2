// imagestabilizationmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<IMAGESTABILIZATIONMODE> GetImageStabilizationMode(const std::string& imagestabilizationmode)
{
  if (boost::iequals(imagestabilizationmode, "OFF"))
  {

    return IMAGESTABILIZATIONMODE_OFF;
  }
  else if (boost::iequals(imagestabilizationmode, "ON"))
  {

    return IMAGESTABILIZATIONMODE_ON;
  }
  else if (boost::iequals(imagestabilizationmode, "Auto"))
  {

    return IMAGESTABILIZATIONMODE_AUTO;
  }
  else if (boost::iequals(imagestabilizationmode, "Extended"))
  {

    return IMAGESTABILIZATIONMODE_EXTENDED;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(IMAGESTABILIZATIONMODE imagestabilizationmode)
{
  switch (imagestabilizationmode)
  {
    case IMAGESTABILIZATIONMODE_OFF:
    {
    
      return std::string("OFF");
    }
    case IMAGESTABILIZATIONMODE_ON:
    {
    
      return std::string("ON");
    }
    case IMAGESTABILIZATIONMODE_AUTO:
    {
    
      return std::string("Auto");
    }
    case IMAGESTABILIZATIONMODE_EXTENDED:
    {
    
      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid IMAGESTABILIZATIONMODE");
    }
  }
}

}
