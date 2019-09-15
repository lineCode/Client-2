// videoencoding.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<VIDEOENCODING> GetVideoEncoding(const std::string& text)
{
  if (text == std::string("JPEG"))
  {

    return VIDEOENCODING_JPEG;
  }
  else if (text == std::string("MPEG4"))
  {

    return VIDEOENCODING_MPEG4;
  }
  else if (text == std::string("H264"))
  {

    return VIDEOENCODING_H264;
  }
  else
  {

    return boost::none;
  }
}

std::string ToString(VIDEOENCODING videoencoding)
{
  switch (videoencoding)
  {
    case VIDEOENCODING_JPEG:
    {

      return std::string("JPEG");
    }
    case VIDEOENCODING_MPEG4:
    {

      return std::string("MPEG4");
    }
    case VIDEOENCODING_H264:
    {

      return std::string("H264");
    }
    default:
    {

      throw std::runtime_error("Invalid VIDEOENCODING");
    }
  }
}

}
