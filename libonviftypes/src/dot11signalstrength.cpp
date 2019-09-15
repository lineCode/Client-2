// dot11signalstrength.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DOT11SIGNALSTRENGTH> GetDot11SignalStrength(const std::string& dot11signalstrength)
{
  if (boost::iequals(dot11signalstrength, "None"))
  {

    return DOT11SIGNALSTRENGTH_NONE;
  }
  else if (boost::iequals(dot11signalstrength, "Very Bad"))
  {

    return DOT11SIGNALSTRENGTH_VERYBAD;
  }
  else if (boost::iequals(dot11signalstrength, "Bad"))
  {

    return DOT11SIGNALSTRENGTH_BAD;
  }
  else if (boost::iequals(dot11signalstrength, "Good"))
  {

    return DOT11SIGNALSTRENGTH_GOOD;
  }
  else if (boost::iequals(dot11signalstrength, "Very Good"))
  {

    return DOT11SIGNALSTRENGTH_VERYGOOD;
  }
  else if (boost::iequals(dot11signalstrength, "Extended"))
  {

    return DOT11SIGNALSTRENGTH_EXTENDED;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(DOT11SIGNALSTRENGTH dot11signalstrength)
{
  switch (dot11signalstrength)
  {
    case DOT11SIGNALSTRENGTH_NONE:
    {

      return std::string("None");
    }
    case DOT11SIGNALSTRENGTH_VERYBAD:
    {

      return std::string("Very Bad");
    }
    case DOT11SIGNALSTRENGTH_BAD:
    {

      return std::string("Bad");
    }
    case DOT11SIGNALSTRENGTH_GOOD:
    {

      return std::string("Good");
    }
    case DOT11SIGNALSTRENGTH_VERYGOOD:
    {

      return std::string("Very Good");
    }
    case DOT11SIGNALSTRENGTH_EXTENDED:
    {

      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid DOT11SIGNALSTRENGTH");
    }
  }
}

}
