// connection.cpp
//

///// Includes /////

#include "onvifclient/connection.hpp"

///// Functions /////

void swap(onvif::Connection& lhs, onvif::Connection& rhs) noexcept
{
  auto tmp = std::move(lhs);
  lhs = std::move(rhs);
  rhs = std::move(tmp);
}

///// Namespaces /////

namespace onvif
{

///// Methods /////

Connection::Connection()
{

}

Connection::Connection(boost::shared_ptr<ConnectionBlock>& connectionblock) :
  connectionblock_(connectionblock)
{

}

Connection::Connection(Connection&& connection) noexcept :
  connectionblock_(std::move(connection.connectionblock_))
{
  connection.connectionblock_.reset();

}

Connection::~Connection()
{

}

void Connection::Close()
{
  boost::shared_ptr<ConnectionBlock> connectionblock = connectionblock_.lock();
  if (!connectionblock)
  {

    return;
  }

  std::lock_guard<std::mutex> lock(connectionblock->mutex_);
  connectionblock->connected_ = false;
}

bool Connection::IsConnected() const
{
  boost::shared_ptr<ConnectionBlock> connectionblock = connectionblock_.lock();
  if (!connectionblock)
  {

    return false;
  }

  std::lock_guard<std::mutex> lock(connectionblock->mutex_);
  return connectionblock->connected_;
}

Connection& Connection::operator=(Connection&& connection) noexcept
{
  connectionblock_ = connection.connectionblock_;
  connection.connectionblock_.reset();
  return (*this);
}

}
