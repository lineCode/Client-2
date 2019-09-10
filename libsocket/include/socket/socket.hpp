// socket.hpp
//

#ifndef ID8FY0JQZA7OBVKELLTKBC0C95MMLSU3NZ
#define ID8FY0JQZA7OBVKELLTKBC0C95MMLSU3NZ

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <deque>
#include <stdint.h>
#include <string>

///// Defines /////

#undef DOMAIN

///// Namespaces /////

namespace sock
{

///// Constants /////

enum class SOCKS5DESTINATION : unsigned char
{
  INVALID = 0x00,
  IPV4 = 0x01,
  IPV6 = 0x04,
  DOMAIN = 0x03
};

///// Enumerations /////

enum class ROUTINGTYPE : unsigned char
{
  TCP = 0x01
  //TCPLISTEN = 0x02, // Currently unsupported because it is not required
  //UDP = 0x03 // Currently unsupported because it is not required
};

///// Structures /////

struct DESTADDRESS
{
  SOCKS5DESTINATION destinationtype_;
  std::vector<unsigned char> address_;
  uint16_t port_;
};

struct HTTPPARAMS
{
  HTTPPARAMS(const std::string& hostname, const uint16_t port, const std::string& desthostname, const uint16_t destport, const std::string& username, const std::string& password);

  std::string hostname_;
  uint16_t port_;
  std::string desthostname_;
  uint16_t destport_;
  std::string username_;
  std::string password_;
};

struct SOCKSPARAMS
{
  SOCKSPARAMS(const std::string& username, const std::string& password);

  boost::optional<std::string> username_;
  boost::optional<std::string> password_;

};

///// Classes /////

class Socket : public boost::enable_shared_from_this<Socket>
{
 public:

  Socket(boost::asio::io_service& io);
  virtual ~Socket();

  virtual boost::asio::ip::tcp::socket& GetSocket() = 0;

  void HTTPHandleProxyResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback);
  void HTTPHandleConnect(const boost::system::error_code& error, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback);
  void HTTPHandleProxyRead(const boost::system::error_code& err, boost::shared_ptr<boost::beast::flat_buffer> buffer, boost::shared_ptr< boost::beast::http::response_parser<boost::beast::http::empty_body> > httpresponseparser, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback);
  void HTTPHandleProxyAuthenticateRead(const boost::system::error_code& err, boost::shared_ptr<boost::beast::flat_buffer> buffer, boost::shared_ptr< boost::beast::http::response_parser<boost::beast::http::empty_body> > httpresponseparser, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback);

  void SocksHandleDestResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, const std::string& proxyhostname, unsigned short proxyport, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback);
  void SocksHandleProxyResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback);
  void SocksHandleConnect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback);
  void SocksHandleResponse(const boost::system::error_code& error, std::size_t numbytes, boost::shared_ptr< std::array<unsigned char, 65536> > buffer, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback);
  void SocksHandleRead(const boost::system::error_code& error, boost::shared_ptr< std::array<unsigned char, 65536> > buffer, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback);
  
 protected:

  void HTTPConnect(const std::string& hostname, unsigned short port, const std::string& destaddress, unsigned short destport, const std::string& username, const std::string& password, const boost::function<void(const boost::system::error_code&)>& connectcallback);
  void SocksConnect(ROUTINGTYPE routingtype, const std::string& hostname, uint16_t port, const std::string& desthostname, uint16_t destport, bool remotedns, const std::string& username, const std::string& password, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)>& connectcallback);

  boost::asio::ip::tcp::resolver resolver_; // This could potentially be passed through as a shared_ptr through the functions, but it is such a small class that I didn't bother


 private:
  
  void SocksConnect(boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, const std::deque<DESTADDRESS>& destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)>& connectcallback);
  
};

}

#endif
