// exposurepriority.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<EXPOSUREPRIORITY> GetExposurePriority(const std::string& exposurepriority)
{
  if (boost::iequals(exposurepriority, "LowNoise"))
  {

    return EXPOSUREPRIORITY_LOWNOISE;
  }
  else if (boost::iequals(exposurepriority, "FrameRate"))
  {

    return EXPOSUREPRIORITY_FRAMERATE;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(EXPOSUREPRIORITY exposurepriority)
{
  switch (exposurepriority)
  {
    case EXPOSUREPRIORITY_LOWNOISE:
    {

      return std::string("LowNoise");
    }
    case EXPOSUREPRIORITY_FRAMERATE:
    {

      return std::string("FrameRate");
    }
    default:
    {

      throw std::runtime_error("Invalid EXPOSUREPRIORITY");
    }
  }
}

}
