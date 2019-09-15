// whitebalancemode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<WHITEBALANCEMODE> GetWhiteBalanceMode(const std::string& whitebalancemode)
{
  if (boost::iequals(whitebalancemode, "AUTO"))
  {

    return WHITEBALANCEMODE_AUTO;
  }
  else if (boost::iequals(whitebalancemode, "MANUAL"))
  {

    return WHITEBALANCEMODE_MANUAL;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(WHITEBALANCEMODE whitebalancemode)
{
  switch (whitebalancemode)
  {
    case WHITEBALANCEMODE_AUTO:
    {

      return std::string("AUTO");
    }
    case WHITEBALANCEMODE_MANUAL:
    {

      return std::string("MANUAL");
    }
    default:
    {

      throw std::runtime_error("Invalid WHITEBALANCEMODE");
    }
  }
}

}
