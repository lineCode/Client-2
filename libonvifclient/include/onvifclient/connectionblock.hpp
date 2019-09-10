// connectionblock.hpp
//

#ifndef ID249CXKXXGW353GIFX3CA8R1AZY10DGK7
#define ID249CXKXXGW353GIFX3CA8R1AZY10DGK7

///// Includes /////

#include <mutex>

///// Namespaces /////

namespace onvif
{

///// Classes /////

class ConnectionBlock
{
 public:

  ConnectionBlock();
  ConnectionBlock(bool connected);
  ~ConnectionBlock();

  bool connected_;
  std::mutex mutex_;

};

}

#endif