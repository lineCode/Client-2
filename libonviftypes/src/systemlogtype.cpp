// systemlogtype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<SYSTEMLOGTYPE> GetSystemLogType(const std::string& systemlogtype)
{
  if (boost::iequals(systemlogtype, "System"))
  {

    return SYSTEMLOGLEVEL_SYSTEM;
  }
  else if (boost::iequals(systemlogtype, "Access"))
  {

    return SYSTEMLOGLEVEL_ACCESS;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(SYSTEMLOGTYPE systemlogtype)
{
  switch (systemlogtype)
  {
    case SYSTEMLOGLEVEL_SYSTEM:
    {
    
      return std::string("System");
    }
    case SYSTEMLOGLEVEL_ACCESS:
    {
    
      return std::string("Access");
    }
    default:
    {

      throw std::runtime_error("Invalid SYSTEMLOGTYPE");
    }
  }
}

}
