// streamtype.cpp
//

///// Includes /////

#include "onviftypes/onviftypes.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

///// Functions /////

boost::optional<STREAMTYPE> GetStreamType(const std::string& streamtype)
{
  if (boost::iequals(streamtype, "RTP-Unicast"))
  {

    return STREAM_RTPUNICAST;
  }
  else if (boost::iequals(streamtype, "RTP-Multicast"))
  {

    return STREAM_RTPMULTICAST;
  }
  else
  {
  
    return boost::none;
  }
}

std::string ToString(STREAMTYPE streamtype)
{
  switch (streamtype)
  {
    case STREAM_RTPUNICAST:
    {
    
      return std::string("RTP-Unicast");
    }
    case STREAM_RTPMULTICAST:
    {
    
      return std::string("RTP-Multicast");
    }
    default:
    {

      throw std::runtime_error("Invalid STREAMTYPE");
    }
  }
}

}
