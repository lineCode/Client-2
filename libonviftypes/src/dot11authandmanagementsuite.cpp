// dot11authandmanagementsuite.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DOT11AUTHANDMANAGEMENTSUITE> GetDot11AuthAndManagementSuite(const std::string& dot11authandmanagementsuite)
{
  if (boost::iequals(dot11authandmanagementsuite, "None"))
  {

    return DOT11AUTHANDMANAGEMENTSUITE_NONE;
  }
  else if (boost::iequals(dot11authandmanagementsuite, "Dot1X"))
  {

    return DOT11AUTHANDMANAGEMENTSUITE_DOT1X;
  }
  else if (boost::iequals(dot11authandmanagementsuite, "PSK"))
  {

    return DOT11AUTHANDMANAGEMENTSUITE_PSK;
  }
  else if (boost::iequals(dot11authandmanagementsuite, "Extended"))
  {

    return DOT11AUTHANDMANAGEMENTSUITE_EXTENDED;
  }
  else
  {
    
    return boost::none;
  }
}

std::string ToString(DOT11AUTHANDMANAGEMENTSUITE dot11authandmanagementsuite)
{
  switch (dot11authandmanagementsuite)
  {
    case DOT11AUTHANDMANAGEMENTSUITE_NONE:
    {
    
      return std::string("None");
    }
    case DOT11AUTHANDMANAGEMENTSUITE_DOT1X:
    {
    
      return std::string("Dot1X");
    }
    case DOT11AUTHANDMANAGEMENTSUITE_PSK:
    {
    
      return std::string("PSK");
    }
    case DOT11AUTHANDMANAGEMENTSUITE_EXTENDED:
    {
    
      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid DOT11AUTHANDMANAGEMENTSUITE");
    }
  }
}

}
