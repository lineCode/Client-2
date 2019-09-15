// mpeg4profile.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<MPEG4PROFILE> GetMpeg4Profile(const std::string& mpeg4profile)
{
  if (boost::iequals(mpeg4profile, "SP"))
  {

    return MPEG4PROFILE_SP;
  }
  else if (boost::iequals(mpeg4profile, "ASP"))
  {

    return MPEG4PROFILE_ASP;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(MPEG4PROFILE mpeg4profile)
{
  switch (mpeg4profile)
  {
    case MPEG4PROFILE_SP:
    {
    
      return std::string("SP");
    }
    case MPEG4PROFILE_ASP:
    {
    
      return std::string("ASP");
    }
    default:
    {

      throw std::runtime_error("Invalid MPEG4PROFILE");
    }
  }
}

}
