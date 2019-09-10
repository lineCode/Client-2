// server.hpp
//

#ifndef IDF69P3OAV56LY5O7876Z6XTGJT0STS5UA
#define IDF69P3OAV56LY5O7876Z6XTGJT0STS5UA

///// Includes /////

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>
#include <mutex>
#include <stdint.h>

#include "monocleprotocol/monocleprotocol.hpp"

///// Declarations /////

namespace utility
{

class IoServicePool;

}

///// Namespaces /////

namespace monocle
{

namespace server
{

///// Declarations /////

extern const uint16_t DEFAULTPORT;

class Connection;

///// Classes /////

class Server : public boost::enable_shared_from_this<Server>
{
 public:

  Server(utility::IoServicePool& ioservicepool);
  virtual ~Server();

  int Init(const std::vector<uint16_t>& v4ports, const std::vector<uint16_t>& v6ports);
  void Destroy();

  virtual boost::shared_ptr<Connection> CreateConnection(boost::asio::io_service& io) = 0;

  void DestroyConnection(const boost::shared_ptr<Connection>& connection);

 protected:

  mutable std::mutex mutex_;

  std::vector< boost::shared_ptr<Connection> > connections_;

 private:

  int InitAcceptor(const boost::asio::ip::tcp protocol, const uint16_t port);
  void Accept(boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor);
  void HandleAccept(const boost::system::error_code& err, boost::shared_ptr<Connection> connection, boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor);

  utility::IoServicePool& ioservicepool_;
  std::vector< boost::shared_ptr<boost::asio::ip::tcp::acceptor> > acceptors_;

};

}

}

#endif
