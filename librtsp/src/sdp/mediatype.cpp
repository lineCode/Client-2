// mediatype.cpp
//

///// Includes /////

#include "rtsp/sdp/mediatype.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Functions /////

std::string ToString(MEDIATYPE mediatype)
{
  switch (mediatype)
  {
  case MEDIATYPE_AUDIO:
  {

    return std::string("audio");
  }
  case MEDIATYPE_VIDEO:
  {

    return std::string("video");
  }
  case MEDIATYPE_TEXT:
  {

    return std::string("text");
  }
  case MEDIATYPE_APPLICATION:
  {

    return std::string("application");
  }
  case MEDIATYPE_MESSAGE:
  {

    return std::string("message");
  }
  default:
  {

    return std::string();
  }
  }
}

MEDIATYPE MediaTypeFromString(const std::string& mediatype)
{
  if (boost::algorithm::iequals(mediatype, "audio"))
  {

    return MEDIATYPE_AUDIO;
  }
  else if (boost::algorithm::iequals(mediatype, "video"))
  {

    return MEDIATYPE_VIDEO;
  }
  else if (boost::algorithm::iequals(mediatype, "text"))
  {

    return MEDIATYPE_TEXT;
  }
  else if (boost::algorithm::iequals(mediatype, "application"))
  {

    return MEDIATYPE_APPLICATION;
  }
  else if (boost::algorithm::iequals(mediatype, "message"))
  {

    return MEDIATYPE_MESSAGE;
  }
  else
  {

    return MEDIATYPE_INVALID;
  }
}

}

}
