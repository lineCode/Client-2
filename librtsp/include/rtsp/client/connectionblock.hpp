// connectionblock.hpp
//

#ifndef IDJD998ZC93CLBG5FLB8CGZIDWJ47ZGTDT
#define IDJD998ZC93CLBG5FLB8CGZIDWJ47ZGTDT

///// Includes /////

#include <mutex>

///// Namespaces /////

namespace rtsp
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