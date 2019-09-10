// connectionblock.cpp
//

///// Includes /////

#include "socket/connectionblock.hpp"

///// Namespaces /////

namespace sock
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
