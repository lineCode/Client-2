// ircutfiltermode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>
#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<IRCUTFILTERMODE> GetIrCutFilterMode(const std::string& ircutfiltermode)
{
  if (boost::iequals(ircutfiltermode, "ON"))
  {

    return IRCUTFILTERMODE_ON;
  }
  else if (boost::iequals(ircutfiltermode, "OFF"))
  {

    return IRCUTFILTERMODE_OFF;
  }
  else if (boost::iequals(ircutfiltermode, "AUTO"))
  {

    return IRCUTFILTERMODE_AUTO;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(IRCUTFILTERMODE ircutfiltermode)
{
  switch (ircutfiltermode)
  {
    case IRCUTFILTERMODE_ON:
    {

      return std::string("ON");
    }
    case IRCUTFILTERMODE_OFF:
    {

      return std::string("OFF");
    }
    case IRCUTFILTERMODE_AUTO:
    {

      return std::string("AUTO");
    }
    default:
    {

      throw std::runtime_error("Invalid IRCUTFILTERMODE");
    }
  }
}

}
