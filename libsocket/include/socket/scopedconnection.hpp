// scopedconnection.hpp
//

#ifndef IDPYYSMJ3NCANC2BR5EDE02UE3CRJ0T2E7
#define IDPYYSMJ3NCANC2BR5EDE02UE3CRJ0T2E7

///// Includes /////

#include "connection.hpp"

///// Namespaces /////

namespace sock
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
