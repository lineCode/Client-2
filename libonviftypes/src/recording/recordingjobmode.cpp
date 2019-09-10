// recordingjobmode.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Functions /////

boost::optional<RECORDINGJOBMODE> GetRecordingJobMode(const std::string& recordingjobmode)
{
  if (boost::iequals(recordingjobmode, "Idle"))
  {

    return RECORDINGJOBMODE_IDLE;
  }
  else if (boost::iequals(recordingjobmode, "Active"))
  {

    return RECORDINGJOBMODE_ACTIVE;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(RECORDINGJOBMODE recordingjobmode)
{
  switch (recordingjobmode)
  {
    case RECORDINGJOBMODE_IDLE:
    {
    
      return std::string("Idle");
    }
    case RECORDINGJOBMODE_ACTIVE:
    {
    
      return std::string("Active");
    }
    default:
    {

      throw std::runtime_error("Invalid RECORDINGJOBMODE");
    }
  }
}

}

}
