// connectionblock.hpp
//

#ifndef IDTGQ1HH10AAEIUCUPWMTQZOKWQZD04ZY4
#define IDTGQ1HH10AAEIUCUPWMTQZOKWQZD04ZY4

///// Includes /////

#include <mutex>

///// Namespaces /////

namespace sock
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