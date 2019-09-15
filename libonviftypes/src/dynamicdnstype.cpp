// dynamicdnstype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<DYNAMICDNSTYPE> GetDynamicDNSType(const std::string& dynamicdnstype)
{
  if (boost::iequals(dynamicdnstype, "NoUpdate"))
  {

    return DYNAMICDNSTYPE_NOUPDATE;
  }
  else if (boost::iequals(dynamicdnstype, "ClientUpdates"))
  {

    return DYNAMICDNSTYPE_CLIENTUPDATES;
  }
  else if (boost::iequals(dynamicdnstype, "ServerUpdates"))
  {

    return DYNAMICDNSTYPE_SERVERUPDATES;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(DYNAMICDNSTYPE dynamicdnstype)
{
  switch (dynamicdnstype)
  {
    case DYNAMICDNSTYPE_NOUPDATE:
    {
    
      return std::string("NoUpdate");
    }
    case DYNAMICDNSTYPE_CLIENTUPDATES:
    {
    
      return std::string("ClientUpdates");
    }
    case DYNAMICDNSTYPE_SERVERUPDATES:
    {
    
      return std::string("ServerUpdates");
    }
    default:
    {

      throw std::runtime_error("Invalid DYNAMICDNSTYPE");
    }
  }
}

}
