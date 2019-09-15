// recordingjobstate.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Functions /////

boost::optional<RECORDINGJOBSTATE> GetRecordingJobState(const std::string& recordingjobstate)
{
  if (boost::iequals(recordingjobstate, "Idle"))
  {

    return RECORDINGJOBSTATE_IDLE;
  }
  else if (boost::iequals(recordingjobstate, "Active"))
  {

    return RECORDINGJOBSTATE_ACTIVE;
  }
  else if (boost::iequals(recordingjobstate, "PartiallyActive"))
  {

    return RECORDINGJOBSTATE_PARTIALLYACTIVE;
  }
  else if (boost::iequals(recordingjobstate, "Error"))
  {

    return RECORDINGJOBSTATE_ERROR;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(RECORDINGJOBSTATE recordingjobstate)
{
  switch (recordingjobstate)
  {
    case RECORDINGJOBSTATE_IDLE:
    {
    
      return std::string("Idle");
    }
    case RECORDINGJOBSTATE_ACTIVE:
    {

      return std::string("Active");
    }
    case RECORDINGJOBSTATE_PARTIALLYACTIVE:
    {

      return std::string("PartiallyActive");
    }
    case RECORDINGJOBSTATE_ERROR:
    {
    
      return std::string("Error");
    }
    default:
    {

      throw std::runtime_error("Invalid RECORDINGJOBSTATE");
    }
  }
}

}

}
