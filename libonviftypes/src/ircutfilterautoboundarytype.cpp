// ircutfilterautoboundarytype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<IRCUTFILTERAUTOBOUNDARYTYPE> GetIrCutFilterAutoBoundaryType(const std::string& ircutfilterautoboundarytype)
{
  if (boost::iequals(ircutfilterautoboundarytype, "Common"))
  {

    return IRCUTFILTERAUTOBOUNDARYTYPE_COMMON;
  }
  else if (boost::iequals(ircutfilterautoboundarytype, "ToOn"))
  {

    return IRCUTFILTERAUTOBOUNDARYTYPE_TOON;
  }
  else if (boost::iequals(ircutfilterautoboundarytype, "ToOff"))
  {

    return IRCUTFILTERAUTOBOUNDARYTYPE_TOOFF;
  }
  else if (boost::iequals(ircutfilterautoboundarytype, "Extended"))
  {

    return IRCUTFILTERAUTOBOUNDARYTYPE_EXTENDED;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(IRCUTFILTERAUTOBOUNDARYTYPE ircutfilterautoboundarytype)
{
  switch (ircutfilterautoboundarytype)
  {
    case IRCUTFILTERAUTOBOUNDARYTYPE_COMMON:
    {
    
      return std::string("Common");
    }
    case IRCUTFILTERAUTOBOUNDARYTYPE_TOON:
    {
    
      return std::string("ToOn");
    }
    case IRCUTFILTERAUTOBOUNDARYTYPE_TOOFF:
    {
    
      return std::string("ToOff");
    }
    case IRCUTFILTERAUTOBOUNDARYTYPE_EXTENDED:
    {
    
      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid IRCUTFILTERAUTOBOUNDARYTYPE");
    }
  }
}

}
