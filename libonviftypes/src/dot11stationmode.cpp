// dot11stationmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DOT11STATIONMODE> GetDot11StationMode(const std::string& dot11stationmode)
{
  if (boost::iequals(dot11stationmode, "Ad-hoc"))
  {

    return DOT11STATIONMODE_ADHOC;
  }
  else if (boost::iequals(dot11stationmode, "Infrastructure"))
  {

    return DOT11STATIONMODE_INFRASTRUCTURE;
  }
  else if (boost::iequals(dot11stationmode, "Extended"))
  {

    return DOT11STATIONMODE_EXTENDED;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(DOT11STATIONMODE dot11stationmode)
{
  switch (dot11stationmode)
  {
    case DOT11STATIONMODE_ADHOC:
    {

      return std::string("Ad-hoc");
    }
    case DOT11STATIONMODE_INFRASTRUCTURE:
    {

      return std::string("Infrastructure");
    }
    case DOT11STATIONMODE_EXTENDED:
    {

      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid DOT11STATIONMODE");
    }
  }
}

}
