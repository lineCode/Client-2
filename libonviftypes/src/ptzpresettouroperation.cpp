// ptzpresettouroperation.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<PTZPRESETTOUROPERATION> GetPTZPresetTourOperation(const std::string& ptzpresettouroperation)
{
  if (boost::iequals(ptzpresettouroperation, "Start"))
  {

    return PTZPRESETTOUROPERATION_START;
  }
  else if (boost::iequals(ptzpresettouroperation, "Stop"))
  {

    return PTZPRESETTOUROPERATION_STOP;
  }
  else if (boost::iequals(ptzpresettouroperation, "Pause"))
  {

    return PTZPRESETTOUROPERATION_PAUSE;
  }
  else if (boost::iequals(ptzpresettouroperation, "Extended"))
  {

    return PTZPRESETTOUROPERATION_EXTENDED;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(PTZPRESETTOUROPERATION ptzpresettouroperation)
{
  switch (ptzpresettouroperation)
  {
    case PTZPRESETTOUROPERATION_START:
    {
    
      return std::string("Start");
    }
    case PTZPRESETTOUROPERATION_STOP:
    {
    
      return std::string("Stop");
    }
    case PTZPRESETTOUROPERATION_PAUSE:
    {
    
      return std::string("Pause");
    }
    case PTZPRESETTOUROPERATION_EXTENDED:
    {
    
      return std::string("Extended");
    }
    default:
    {

      throw std::runtime_error("Invalid H264PROFILE");
    }
  }
}

}
