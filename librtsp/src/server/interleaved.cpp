// interleaved.cpp
//

///// Includes /////

#include "rtsp/server/interleaved.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

InterleavedChannel::InterleavedChannel() :
  type_(INTERLEAVEDCHANNEL_NONE)
{

}

void InterleavedChannel::Reset()
{
  session_.reset();
  type_ = INTERLEAVEDCHANNEL_NONE;
}

}

}
