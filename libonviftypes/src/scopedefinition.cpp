// scopedefinition.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<SCOPEDEFINITION> GetScopeDefinitionType(const std::string& scopedefinition)
{
  if (boost::iequals(scopedefinition, "Fixed"))
  {

    return SCOPEDEFINITION_FIXED;
  }
  else if (boost::iequals(scopedefinition, "Configurable"))
  {

    return SCOPEDEFINITION_CONFIGURABLE;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(SCOPEDEFINITION scopedefinition)
{
  switch (scopedefinition)
  {
    case SCOPEDEFINITION_FIXED:
    {
    
      return std::string("Fixed");
    }
    case SCOPEDEFINITION_CONFIGURABLE:
    {
    
      return std::string("Configurable");
    }
    default:
    {

      throw std::runtime_error("Invalid SCOPEDEFINITIONTYPE");
    }
  }
}

}
