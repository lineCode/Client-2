// duplex.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DUPLEX> GetDuplex(const std::string& duplex)
{
  if (boost::iequals(duplex, "Full"))
  {

    return DUPLEX_FULL;
  }
  else if (boost::iequals(duplex, "Half"))
  {

    return DUPLEX_HALF;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(DUPLEX duplex)
{
  switch (duplex)
  {
    case DUPLEX_FULL:
    {
    
      return std::string("Full");
    }
    case DUPLEX_HALF:
    {
    
      return std::string("Half");
    }
    default:
    {

      throw std::runtime_error("Invalid DUPLEX");
    }
  }
}

}
