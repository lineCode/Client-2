// datetimetype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DATETIMETYPE> GetDateTimeType(const std::string& datetimetype)
{
  if (boost::iequals(datetimetype, "Manual"))
  {

    return DATETIME_MANUAL;
  }
  else if (boost::iequals(datetimetype, "NTP"))
  {

    return DATETIME_NTP;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(DATETIMETYPE datetimetype)
{
  switch (datetimetype)
  {
    case DATETIME_MANUAL:
    {
    
      return std::string("Manual");
    }
    case DATETIME_NTP:
    {
    
      return std::string("NTP");
    }
    default:
    {

      throw std::runtime_error("Invalid DATETIMETYPE");
    }
  }
}

}
