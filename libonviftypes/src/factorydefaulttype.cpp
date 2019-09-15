// factorydefaulttype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<FACTORYDEFAULTTYPE> GetFactoryDefaultType(const std::string& factorydefaulttype)
{
  if (boost::iequals(factorydefaulttype, "Soft"))
  {

    return FACTORYDEFAULTTYPE_SOFT;
  }
  else if (boost::iequals(factorydefaulttype, "Hard"))
  {

    return FACTORYDEFAULTTYPE_HARD;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(FACTORYDEFAULTTYPE factorydefaulttype)
{
  switch (factorydefaulttype)
  {
    case FACTORYDEFAULTTYPE_SOFT:
    {
    
      return std::string("Soft");
    }
    case FACTORYDEFAULTTYPE_HARD:
    {
    
      return std::string("Hard");
    }
    default:
    {

      throw std::runtime_error("Invalid FACTORYDEFAULTTYPE");
    }
  }
}

}
