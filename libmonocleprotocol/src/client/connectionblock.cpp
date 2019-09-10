// connectionblock.cpp
//

///// Includes /////

#include "monocleprotocol/client/connectionblock.hpp"

///// Namespaces /////

namespace monocle
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
