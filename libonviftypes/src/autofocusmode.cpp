// autofocusmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<AUTOFOCUSMODE> GetAutoFocusMode(const std::string& autofocusmode)
{
  if (boost::iequals(autofocusmode, "AUTO"))
  {

    return AUTOFOCUSMODE_AUTO;
  }
  else if (boost::iequals(autofocusmode, "MANUAL"))
  {

    return AUTOFOCUSMODE_MANUAL;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(AUTOFOCUSMODE autofocusmode)
{
  switch (autofocusmode)
  {
    case AUTOFOCUSMODE_AUTO:
    {

      return std::string("AUTO");
    }
    case AUTOFOCUSMODE_MANUAL:
    {

      return std::string("MANUAL");
    }
    default:
    {

      throw std::runtime_error("Invalid AUTOFOCUSMODE");
    }
  }
}

}
