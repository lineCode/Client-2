// dot11securitymode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DOT11SECURITYMODE> GetDot11SecurityMode(const std::string& dot11securitymode)
{
  if (boost::iequals(dot11securitymode, "None"))
  {

    return DOT11SECURITYMODE_NONE;
  }
  else if (boost::iequals(dot11securitymode, "WEP"))
  {

    return DOT11SECURITYMODE_WEP;
  }
  else if (boost::iequals(dot11securitymode, "PSK"))
  {

    return DOT11SECURITYMODE_PSK;
  }
  else if (boost::iequals(dot11securitymode, "DOT1X"))
  {

    return DOT11SECURITYMODE_DOT1X;
  }
  else if (boost::iequals(dot11securitymode, "Extended"))
  {

    return DOT11SECURITYMODE_EXTENDED;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(DOT11SECURITYMODE dot11securitymode)
{
  switch (dot11securitymode)
  {
    case DOT11SECURITYMODE_NONE:
    {

      return std::string("None");
    }
    case DOT11SECURITYMODE_WEP:
    {

      return std::string("WEP");
    }
    case DOT11SECURITYMODE_PSK:
    {

      return std::string("PSK");
    }
    case DOT11SECURITYMODE_DOT1X:
    {

      return std::string("DOT1X");
    }
    case DOT11SECURITYMODE_EXTENDED:
    {

      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid DOT11SECURITYMODE");
    }
  }
}

}
