// reversemode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<REVERSEMODE> GetReverseMode(const std::string& reversemode)
{
  if (boost::iequals(reversemode, "OFF"))
  {

    return REVERSEMODE_OFF;
  }
  else if (boost::iequals(reversemode, "ON"))
  {

    return REVERSEMODE_ON;
  }
  else if (boost::iequals(reversemode, "AUTO"))
  {

    return REVERSEMODE_AUTO;
  }
  else if (boost::iequals(reversemode, "Extended"))
  {

    return REVERSEMODE_EXTENDED;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(REVERSEMODE reversemode)
{
  switch (reversemode)
  {
    case REVERSEMODE_OFF:
    {
    
      return std::string("OFF");
    }
    case REVERSEMODE_ON:
    {
    
      return std::string("ON");
    }
    case REVERSEMODE_AUTO:
    {

      return std::string("AUTO");
    }
    case REVERSEMODE_EXTENDED:
    {

      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid REVERSEMODE");
    }
  }
}

}
