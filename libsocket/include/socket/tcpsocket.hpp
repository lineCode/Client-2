// tcpsocket.hpp
//

#ifndef ID8FY0JQZA7OBVKELLTKBC0C95MMLSU3NN
#define ID8FY0JQZA7OBVKELLTKBC0C95MMLSU3NN

///// Includes /////

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>

#include "connection.hpp"
#include "socket.hpp"

///// Namespaces /////

namespace sock
{

///// Declarations /////

class ProxyParams;

///// Classes /////

class TcpSocket : public Socket
{
 public:

  TcpSocket(boost::asio::io_service& io);
  virtual ~TcpSocket();

  virtual boost::asio::ip::tcp::socket& GetSocket() override { return *socket_; }

  Connection Connect(const ProxyParams& proxyparams, const std::string& hostname, uint16_t port, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)>& connectcallback);
  void HandleResolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoints, const boost::shared_ptr<ConnectionBlock> connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)> connectcallback);
  void HandleConnect(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoints, const boost::shared_ptr<ConnectionBlock> connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)> connectcallback);
  void HandleConnect(const boost::system::error_code& err, const boost::shared_ptr<ConnectionBlock> connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)> connectcallback);

  
 protected:

  std::unique_ptr<boost::asio::ip::tcp::socket> socket_;

 private:

  void Emit(const boost::shared_ptr<ConnectionBlock>& connectionblock, const boost::function<void(const boost::system::error_code&, const boost::shared_ptr<ConnectionBlock>&)>& connectcallback, const boost::system::error_code& err) const;

};

}

#endif
