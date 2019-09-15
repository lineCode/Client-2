// audioencoding.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<AUDIOENCODING> GetAudioEncoding(const std::string& audioencoding)
{
  if (boost::iequals(audioencoding, "G711"))
  {

    return AUDIOENCODING_G711;
  }
  else if (boost::iequals(audioencoding, "G726"))
  {

    return AUDIOENCODING_G726;
  }
  else if (boost::iequals(audioencoding, "AAC"))
  {

    return AUDIOENCODING_AAC;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(AUDIOENCODING audioencoding)
{
  switch (audioencoding)
  {
    case AUDIOENCODING_G711:
    {
    
      return std::string("G711");
    }
    case AUDIOENCODING_G726:
    {
    
      return std::string("G726");
    }
    case AUDIOENCODING_AAC:
    {
    
      return std::string("AAC");
    }
    default:
    {

      throw std::runtime_error("Invalid AUDIOENCODING");
    }
  }
}

}
