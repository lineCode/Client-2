// scopedconnection.hpp
//

#ifndef IDMEEZJJXQIRT30HXX3J44DJHUJAIWVIGT
#define IDMEEZJJXQIRT30HXX3J44DJHUJAIWVIGT

///// Includes /////

#include "connection.hpp"

///// Namespaces /////

namespace onvif
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

#endif
