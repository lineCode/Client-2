// connectionblock.cpp
//

///// Includes /////

#include "onvifclient/connectionblock.hpp"

///// Namespaces /////

namespace onvif
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
