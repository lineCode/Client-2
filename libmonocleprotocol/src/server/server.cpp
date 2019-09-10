// server.hpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "monocleprotocol/server/server.hpp"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <thread>
#include <utility/ioservicepool.hpp>
#include <utility/utility.hpp>

#include "monocleprotocol/server/connection.hpp"

///// Namespaces /////

namespace monocle
{

namespace server
{

///// Globals /////

const uint16_t DEFAULTPORT = 9854;

///// Methods /////

Server::Server(utility::IoServicePool& ioservicepool) :
  ioservicepool_(ioservicepool)
{

}

Server::~Server()
{
  Destroy();

}

int Server::Init(const std::vector<uint16_t>& v4ports, const std::vector<uint16_t>& v6ports)
{
  Destroy();

  // Setup ports
  for (const uint16_t port : v4ports)
  {
    if (InitAcceptor(boost::asio::ip::tcp::v4(), port))
    {

      return 1;
    }
  }

  for (const uint16_t port : v6ports)
  {
    if (InitAcceptor(boost::asio::ip::tcp::v6(), port))
    {

      return 2;
    }
  }

  return 0;
}

void Server::Destroy()
{
  {
    std::lock_guard<std::mutex> lock(mutex_);
    for (boost::shared_ptr<Connection>& connection : connections_)
    {
      connection->SendGoodbye();
      connection->Destroy();
      connection->Disconnected();
    }
    connections_.clear();
    for (boost::shared_ptr<boost::asio::ip::tcp::acceptor>& acceptor : acceptors_)
    {
      boost::system::error_code err;
      acceptor->close(err);
    }
    acceptors_.clear();
  }
}

void Server::DestroyConnection(const boost::shared_ptr<Connection>& connection)
{
  std::lock_guard<std::mutex> lock(mutex_);
  std::vector< boost::shared_ptr<Connection> >::iterator c = std::find(connections_.begin(), connections_.end(), connection);
  if (c == connections_.end())
  {

    return;
  }
  (*c)->Disconnected();
  (*c)->Destroy();
  connections_.erase(c);
}

int Server::InitAcceptor(const boost::asio::ip::tcp protocol, const uint16_t port)
{
  boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor = boost::make_shared<boost::asio::ip::tcp::acceptor>(ioservicepool_.GetIoService());
  boost::system::error_code err;
  if (acceptor->open(protocol, err))
  {

    return 1;
  }

  acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), err);
  if (err)
  {

    return 2;
  }

  if (acceptor->bind(boost::asio::ip::tcp::endpoint(protocol, port), err))
  {

    return 3;
  }

  if (acceptor->listen(SOMAXCONN, err))
  {

    return 4;
  }

  Accept(acceptor);
  {
    std::lock_guard<std::mutex> lock(mutex_);
    acceptors_.push_back(acceptor);
  }
  return 0;
}

void Server::Accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor)
{
  boost::shared_ptr<Connection> connection = CreateConnection(ioservicepool_.GetIoService());
  acceptor->async_accept(connection->GetSocket(), boost::bind(&Server::HandleAccept, shared_from_this(), boost::asio::placeholders::error, connection, acceptor));
}

void Server::HandleAccept(const boost::system::error_code& err, boost::shared_ptr<Connection> connection, boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor)
{
  if (err)
  {
    // Something has gone severely wrong, perhaps a malicious user?
    return;
  }
  if (!connection->Connected())
  {
    std::lock_guard<std::mutex> lock(mutex_);
    connections_.push_back(connection);
  }
  Accept(acceptor);
}

}

}
