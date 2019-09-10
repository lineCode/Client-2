// recordingstatus.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<RECORDINGSTATUS> GetRecordingStatus(const std::string& recordingstatus)
{
  if (boost::iequals(recordingstatus, "Initiated"))
  {

    return RECORDINGSTATUS_INITIATED;
  }
  else if (boost::iequals(recordingstatus, "Recording"))
  {

    return RECORDINGSTATUS_RECORDING;
  }
  else if (boost::iequals(recordingstatus, "Stopped"))
  {

    return RECORDINGSTATUS_STOPPED;
  }
  else if (boost::iequals(recordingstatus, "Removing"))
  {

    return RECORDINGSTATUS_REMOVING;
  }
  else if (boost::iequals(recordingstatus, "Removed"))
  {

    return RECORDINGSTATUS_REMOVED;
  }
  else if (boost::iequals(recordingstatus, "Unknown"))
  {

    return RECORDINGSTATUS_UNKNOWN;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(RECORDINGSTATUS recordingstatus)
{
  switch (recordingstatus)
  {
    case RECORDINGSTATUS_INITIATED:
    {
    
      return std::string("Initiated");
    }
    case RECORDINGSTATUS_RECORDING:
    {
    
      return std::string("Recording");
    }
    case RECORDINGSTATUS_STOPPED:
    {
    
      return std::string("Stopped");
    }
    case RECORDINGSTATUS_REMOVING:
    {
    
      return std::string("Removing");
    }
    case RECORDINGSTATUS_REMOVED:
    {
    
      return std::string("Removed");
    }
    case RECORDINGSTATUS_UNKNOWN:
    {
    
      return std::string("Unknown");
    }
    default:
    {

      throw std::runtime_error("Invalid RECORDINGSTATUS");
    }
  }
}

}
