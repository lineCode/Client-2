// scopedconnection.cpp
//

///// Includes /////

#include "socket/scopedconnection.hpp"

///// Namespaces /////

namespace sock
{

///// Methods /////

ScopedConnection::ScopedConnection()
{

}

ScopedConnection::ScopedConnection(ScopedConnection&& scopedconnection) noexcept :
  connection_(std::move(scopedconnection.connection_))
{

}

ScopedConnection::ScopedConnection(Connection&& connection) noexcept :
  connection_(std::move(connection))
{

}

ScopedConnection::~ScopedConnection()
{
  connection_.Close();

}

void ScopedConnection::Close()
{
  connection_.Close();

}

bool ScopedConnection::IsConnected() const
{
  
  return connection_.IsConnected();
}

ScopedConnection& ScopedConnection::operator=(ScopedConnection&& scopedconnection) noexcept
{
  connection_ = std::move(scopedconnection.connection_);
  return (*this);
}

ScopedConnection& ScopedConnection::operator=(Connection&& connection) noexcept
{
  connection_ = std::move(connection);
  return (*this);
}

}
