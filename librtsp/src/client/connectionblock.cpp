// connectionblock.cpp
//

///// Includes /////

#include "rtsp/client/connectionblock.hpp"

///// Namespaces /////

namespace rtsp
{

namespace client
{

///// Methods /////

ConnectionBlock::ConnectionBlock() :
  connected_(false)
{

}

ConnectionBlock::ConnectionBlock(bool connected) :
  connected_(connected)
{

}

ConnectionBlock::~ConnectionBlock()
{

}

}

}
