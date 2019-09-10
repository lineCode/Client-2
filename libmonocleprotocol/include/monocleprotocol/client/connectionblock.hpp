// connectionblock.hpp
//

#ifndef ID249CZZZZZW353GIFX3CA8R1AZY10DGK7
#define ID249CZZZZZW353GIFX3CA8R1AZY10DGK7

///// Includes /////

#include <mutex>

///// Namespaces /////

namespace monocle
{

namespace client
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

}

#endif