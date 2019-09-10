// mode.cpp
//

///// Includes /////

#include "rtsp/headers/mode.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace headers
{

///// Functions /////

std::string ToString(MODE request)
{
  switch (request)
  {
  case MODE_PLAY:
  {

    return std::string("PLAY");
  }
  case MODE_RECORD:
  {

    return std::string("RECORD");
  }
  default:
  {

    return std::string();
  }
  }
}

MODE ModeTypeFromString(const std::string& request)
{
  if (boost::algorithm::iequals(request, "PLAY"))
  {

    return MODE_PLAY;
  }
  else if (boost::algorithm::iequals(request, "RECORD"))
  {

    return MODE_RECORD;
  }
  else
  {

    return MODE_INVALID;
  }
}

}

}
