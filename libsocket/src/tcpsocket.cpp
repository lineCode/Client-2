// tcpsocket.cpp
//

///// Includes /////

#include "socket/tcpsocket.hpp"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#include "socket/proxyparams.hpp"

///// Namespaces /////

namespace sock
{

///// Methods /////

TcpSocket::TcpSocket(boost::asio::io_service& io) :
  Socket(io),
  socket_(std::make_unique<boost::asio::ip::tcp::socket>(io))
{

}

TcpSocket::~TcpSocket()
{
  
}

Connection TcpSocket::Connect(const ProxyParams& proxyparams, const std::string& hostname, uint16_t port, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)>& connectcallback)
{
  if (!proxyparams.IsValid())
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::invalid_argument), boost::shared_ptr<ConnectionBlock>());
    return Connection();
  }

  switch (proxyparams.GetType())
  {
    case PROXYTYPE_NONE:
    {
      boost::system::error_code err;
      boost::asio::ip::address address = boost::asio::ip::address::from_string(hostname, err); // If we already have an address that does not require resolving, just attempt to connect to it
      if (err)
      {
        boost::asio::ip::tcp::resolver::query query(hostname, std::to_string(port));
        boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
        resolver_.async_resolve(query, boost::bind(&TcpSocket::HandleResolve, boost::static_pointer_cast<TcpSocket>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::iterator, connectionblock, connectcallback));
        return connectionblock;
      }
      else
      {
        boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
        socket_->async_connect(boost::asio::ip::tcp::endpoint(address, port), boost::bind(&TcpSocket::HandleConnect, boost::static_pointer_cast<TcpSocket>(shared_from_this()), boost::asio::placeholders::error, connectionblock, connectcallback));
        return connectionblock;
      }
      break;
    }
    case PROXYTYPE_HTTP:
    {
      boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
      HTTPConnect(proxyparams.GetHostname(), proxyparams.GetPort(), hostname, port, proxyparams.GetUsername(), proxyparams.GetPassword(), [this, connectionblock, connectcallback](const boost::system::error_code& err) mutable
      {
        Emit(connectionblock, connectcallback, err);

      });
      return connectionblock;
    }
    case PROXYTYPE_SOCKS5:
    {
      boost::shared_ptr<ConnectionBlock> connectionblock = boost::make_shared<ConnectionBlock>(true);
      SocksConnect(ROUTINGTYPE::TCP, proxyparams.GetHostname(), proxyparams.GetPort(), hostname, port, proxyparams.GetRemoteDns(), proxyparams.GetUsername(), proxyparams.GetPassword(), [this, connectionblock, connectcallback](const boost::system::error_code& err, const std::string&, uint16_t) mutable
      {
        Emit(connectionblock, connectcallback, err);

      });
      return connectionblock;
    }
    default:
    {

      return Connection();
    }
  }
}

void TcpSocket::HandleResolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoints, const boost::shared_ptr<ConnectionBlock> connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)> connectcallback)
{
  if (err)
  {
    Emit(connectionblock, connectcallback, err);
    return;
  }

  socket_->async_connect(endpoints->endpoint(), boost::bind(&TcpSocket::HandleConnect, boost::static_pointer_cast<TcpSocket>(shared_from_this()), boost::asio::placeholders::error, endpoints, connectionblock, connectcallback));
}

void TcpSocket::HandleConnect(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoints, const boost::shared_ptr<ConnectionBlock> connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)> connectcallback)
{
  if (err)
  {
    // Try again with another resolved endpoint if there are any
    ++endpoints;
    if (endpoints == boost::asio::ip::tcp::resolver::iterator())
    {
      Emit(connectionblock, connectcallback, err);
      return;
    }

    boost::system::error_code err;
    socket_->close(err);
    socket_->async_connect(endpoints->endpoint(), boost::bind(&TcpSocket::HandleConnect, boost::static_pointer_cast<TcpSocket>(shared_from_this()), boost::asio::placeholders::error, endpoints, connectionblock, connectcallback));
    return;
  }

  Emit(connectionblock, connectcallback, err);
}

void TcpSocket::HandleConnect(const boost::system::error_code& err, const boost::shared_ptr<ConnectionBlock> connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)> connectcallback)
{
  Emit(connectionblock, connectcallback, err);

}

void TcpSocket::Emit(const boost::shared_ptr<ConnectionBlock>& connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)>& connectcallback, const boost::system::error_code& err) const
{
  std::lock_guard<std::mutex> lock(connectionblock->mutex_);
  if (connectionblock->connected_)
  {
    connectionblock->connected_ = false;
    connectcallback(err, connectionblock);
  }
}

}
