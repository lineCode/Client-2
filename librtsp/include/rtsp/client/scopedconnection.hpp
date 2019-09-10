// scopedconnection.hpp
//

#ifndef IDHOG64NW4XGFCJ5RP5AONOFZ1N69C386S
#define IDHOG64NW4XGFCJ5RP5AONOFZ1N69C386S

///// Includes /////

#include "connection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace client
{

///// Classes /////

class ScopedConnection
{
 public:
 
  ScopedConnection();
  ScopedConnection(const ScopedConnection&) = delete;
  ScopedConnection(ScopedConnection&& connection) noexcept;
  ScopedConnection(Connection&& connection) noexcept;
  ~ScopedConnection();

  void Close();

  bool IsConnected() const;
  
  ScopedConnection& operator=(ScopedConnection&& connection) noexcept;
  ScopedConnection& operator=(Connection&& connection) noexcept;

 private:

  Connection connection_;

};

}

}

#endif
