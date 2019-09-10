// widedynamicmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<WIDEDYNAMICMODE> GetWideDynamicMode(const std::string& widedynamicrange)
{
  if (boost::iequals(widedynamicrange, "OFF"))
  {

    return WIDEDYNAMICMODE_OFF;
  }
  else if (boost::iequals(widedynamicrange, "ON"))
  {

    return WIDEDYNAMICMODE_ON;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(WIDEDYNAMICMODE widedynamicrange)
{
  switch (widedynamicrange)
  {
    case WIDEDYNAMICMODE_OFF:
    {

      return std::string("OFF");
    }
    case WIDEDYNAMICMODE_ON:
    {

      return std::string("ON");
    }
    default:
    {

      throw std::runtime_error("Invalid WIDEDYNAMICMODE");
    }
  }
}

}
