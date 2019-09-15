// tracktype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<TRACKTYPE> GetTrackType(const std::string& tracktype)
{
  if (boost::iequals(tracktype, "Video"))
  {

    return TRACKTYPE_VIDEO;
  }
  else if (boost::iequals(tracktype, "Audio"))
  {

    return TRACKTYPE_AUDIO;
  }
  else if (boost::iequals(tracktype, "Metadata"))
  {

    return TRACKTYPE_METADATA;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(TRACKTYPE tracktype)
{
  switch (tracktype)
  {
    case TRACKTYPE_VIDEO:
    {
    
      return std::string("Video");
    }
    case TRACKTYPE_AUDIO:
    {
    
      return std::string("Audio");
    }
    case TRACKTYPE_METADATA:
    {
    
      return std::string("Metadata");
    }
    default:
    {

      throw std::runtime_error("Invalid TRACKTYPE");
    }
  }
}

}
