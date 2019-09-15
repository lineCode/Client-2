// h264profile.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<H264PROFILE> GetH264Profile(const std::string& h264profile)
{
  if (boost::iequals(h264profile, "Baseline"))
  {

    return H264PROFILE_BASELINE;
  }
  else if (boost::iequals(h264profile, "Main"))
  {

    return H264PROFILE_MAIN;
  }
  else if (boost::iequals(h264profile, "Extended"))
  {

    return H264PROFILE_EXTENDED;
  }
  else if (boost::iequals(h264profile, "High"))
  {

    return H264PROFILE_HIGH;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(H264PROFILE h264profile)
{
  switch (h264profile)
  {
    case H264PROFILE_BASELINE:
    {
    
      return std::string("Baseline");
    }
    case H264PROFILE_MAIN:
    {
    
      return std::string("Main");
    }
    case H264PROFILE_EXTENDED:
    {
    
      return std::string("Extended");
    }
    case H264PROFILE_HIGH:
    {
    
      return std::string("High");
    }
    default:
    {

      throw std::runtime_error("Invalid H264PROFILE");
    }
  }
}

}
