// socket.cpp
//

///// Includes /////

#include "socket/socket.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/beast.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/regex.hpp>
#include <memory>
#include <utility/utility.hpp>

///// Namespaces /////

namespace sock
{

///// Enumerations /////

enum class SOCKS5AUTHENTICATION : unsigned char
{
  NONE = 0x00,
  USERNAMEPASSWORD = 0x02
};

///// Methods /////

HTTPPARAMS::HTTPPARAMS(const std::string& hostname, const uint16_t port, const std::string& desthostname, const uint16_t destport, const std::string& username, const std::string& password) :
  hostname_(hostname),
  port_(port),
  desthostname_(desthostname),
  destport_(destport),
  username_(username),
  password_(password)
{

}

SOCKSPARAMS::SOCKSPARAMS(const std::string& username, const std::string& password) :
  username_(username),
  password_(password)
{

}

Socket::Socket(boost::asio::io_service& io) :
  resolver_(io)
{

}

Socket::~Socket()
{

}

void Socket::HTTPHandleProxyResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback)
{
  if (error)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }

  GetSocket().async_connect(endpoints->endpoint(), boost::bind(&Socket::HTTPHandleConnect, shared_from_this(), boost::asio::placeholders::error, httpparams, connectcallback));
}

void Socket::HTTPHandleConnect(const boost::system::error_code& error, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback)
{
  if (error)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }
  
  boost::beast::http::request<boost::beast::http::empty_body> request;
  request.method(boost::beast::http::verb::connect);
  request.target(httpparams.desthostname_ + ":" + std::to_string(httpparams.destport_));
  request.insert(boost::beast::http::field::host, httpparams.hostname_ + ":" + std::to_string(httpparams.port_));
  request.keep_alive(true);
  boost::system::error_code er;
  boost::beast::http::write(GetSocket(), request, er);
  if (er)
  {
    connectcallback(er);
    return;
  }

  boost::shared_ptr<boost::beast::flat_buffer> buffer = boost::make_shared<boost::beast::flat_buffer>();
  boost::shared_ptr< boost::beast::http::response_parser<boost::beast::http::empty_body> > httpresponseparser = boost::make_shared< boost::beast::http::response_parser<boost::beast::http::empty_body> >();
  httpresponseparser->skip(true);
  boost::beast::http::async_read_some(GetSocket(), *buffer, *httpresponseparser, std::bind(&Socket::HTTPHandleProxyRead, shared_from_this(), std::placeholders::_1, buffer, httpresponseparser, httpparams, connectcallback));
}

void Socket::HTTPHandleProxyRead(const boost::system::error_code& err, boost::shared_ptr<boost::beast::flat_buffer> buffer, boost::shared_ptr< boost::beast::http::response_parser<boost::beast::http::empty_body> > httpresponseparser, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback)
{
  if (err)
  {
    connectcallback(err);
    return;
  }

  if (!httpresponseparser->is_done())
  {
    boost::beast::http::async_read_some(GetSocket(), *buffer, *httpresponseparser, std::bind(&Socket::HTTPHandleProxyRead, shared_from_this(), std::placeholders::_1, buffer, httpresponseparser, httpparams, connectcallback));
    return;
  }

  if (httpresponseparser->get().result() == boost::beast::http::status::ok)
  {
    connectcallback(boost::system::error_code()); // Success! No authentication required
    return;
  }
  else if (httpresponseparser->get().result() != boost::beast::http::status::proxy_authentication_required)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }

  auto proxyauthenticate = httpresponseparser->get().find(boost::beast::http::field::proxy_authenticate);
  if (proxyauthenticate == httpresponseparser->get().end())
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }

  const std::string proxyauthenticatevalue = proxyauthenticate->value().to_string();
  if (!boost::algorithm::istarts_with(proxyauthenticatevalue, "digest ")) // Only accepting digest
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }

  std::vector<std::string> fields;
  const std::string value = proxyauthenticatevalue.substr(7);
  boost::algorithm::split(fields, value, boost::is_any_of(","), boost::algorithm::token_compress_on);
  std::for_each(fields.begin(), fields.end(), [](std::string& field) { boost::algorithm::trim(field); });

  std::vector<std::string>::const_iterator realmfield = std::find_if(fields.cbegin(), fields.cend(), [](const std::string& field){ return boost::istarts_with(field, "realm="); });
  std::vector<std::string>::const_iterator qopfield = std::find_if(fields.cbegin(), fields.cend(), [](const std::string& field) { return boost::istarts_with(field, "qop="); });
  std::vector<std::string>::const_iterator noncefield = std::find_if(fields.cbegin(), fields.cend(), [](const std::string& field) { return boost::istarts_with(field, "nonce="); });
  if ((realmfield == fields.cend()) || (qopfield == fields.cend()) || (noncefield == fields.cend()))
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }

  std::string realmvalue = realmfield->substr(6);
  std::string qopvalue = qopfield->substr(4);
  std::string noncevalue = noncefield->substr(6);

  boost::algorithm::trim(realmvalue);
  boost::algorithm::trim_if(realmvalue, boost::is_any_of("\""));
  boost::algorithm::trim(qopvalue);
  boost::algorithm::trim_if(qopvalue, boost::is_any_of("\""));
  boost::algorithm::trim(noncevalue);
  boost::algorithm::trim_if(noncevalue, boost::is_any_of("\""));

  if (qopvalue != "auth")
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }

  const std::string clientnc("000001");
  const std::string clientcnonce = utility::GenerateRandomString(16);
  const std::string uri = httpparams.desthostname_ + ":" + std::to_string(httpparams.destport_);
  const std::string response = utility::Md5(utility::Md5(httpparams.username_ + ":" + realmvalue + ":" + httpparams.password_) + ":" + noncevalue + ":" + clientnc + ":" + clientcnonce + ":" + qopvalue + ":" + utility::Md5("CONNECT:" + uri));

  boost::beast::http::request<boost::beast::http::empty_body> request;
  request.method(boost::beast::http::verb::connect);
  request.target(httpparams.desthostname_ + ":" + std::to_string(httpparams.destport_));
  request.insert(boost::beast::http::field::host, httpparams.hostname_ + ":" + std::to_string(httpparams.port_));

  request.insert(boost::beast::http::field::proxy_authorization, std::string("Digest realm=\"") + realmvalue + "\", qop=auth, nonce=\"" + noncevalue + "\", response=\"" + response + "\", username=\"" + httpparams.username_ + "\", uri=\"" + uri + "\", nc=\"" + clientnc + "\", cnonce=\"" + clientcnonce + "\"");
  request.keep_alive(true);
  boost::system::error_code er;
  boost::beast::http::write(GetSocket(), request, er);
  if (er)
  {
    connectcallback(er);
    return;
  }

  httpresponseparser = boost::make_shared< boost::beast::http::response_parser<boost::beast::http::empty_body> >();
  httpresponseparser->skip(true);
  boost::beast::http::async_read_some(GetSocket(), *buffer, *httpresponseparser, std::bind(&Socket::HTTPHandleProxyAuthenticateRead, shared_from_this(), std::placeholders::_1, buffer, httpresponseparser, httpparams, connectcallback));
}

void Socket::HTTPHandleProxyAuthenticateRead(const boost::system::error_code& err, boost::shared_ptr<boost::beast::flat_buffer> buffer, boost::shared_ptr< boost::beast::http::response_parser<boost::beast::http::empty_body> > httpresponseparser, const HTTPPARAMS httpparams, const boost::function<void(const boost::system::error_code&)> connectcallback)
{
  if (err)
  {
    connectcallback(err);
    return;
  }

  if (!httpresponseparser->is_done())
  {
    boost::beast::http::async_read_some(GetSocket(), *buffer, *httpresponseparser, std::bind(&Socket::HTTPHandleProxyAuthenticateRead, shared_from_this(), std::placeholders::_1, buffer, httpresponseparser, httpparams, connectcallback));
    return;
  }

  if (httpresponseparser->get().result() == boost::beast::http::status::ok)
  {
    connectcallback(boost::system::error_code());
    return;
  }
  else
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error));
    return;
  }
}

void Socket::SocksHandleDestResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, const std::string& proxyhostname, unsigned short proxyport, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback)
{
  if (error)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  // Collect all the resolved addresses
  std::deque<DESTADDRESS> destaddresses;
  for (; endpoints != boost::asio::ip::tcp::resolver::iterator(); ++endpoints)
  {
    SOCKS5DESTINATION destinationtype = SOCKS5DESTINATION::INVALID;
    std::vector<unsigned char> data;
    if (endpoints->endpoint().protocol() == boost::asio::ip::tcp::v4())
    {
      auto bytes = endpoints->endpoint().address().to_v4().to_bytes();
      destaddresses.push_back({ SOCKS5DESTINATION::IPV4, std::vector<unsigned char>(bytes.begin(), bytes.end()), endpoints->endpoint().port() });
    }
    else if (endpoints->endpoint().protocol() == boost::asio::ip::tcp::v6())
    {
      auto bytes = endpoints->endpoint().address().to_v6().to_bytes();
      destaddresses.push_back({ SOCKS5DESTINATION::IPV6, std::vector<unsigned char>(bytes.begin(), bytes.end()), endpoints->endpoint().port() });
    }
  }

  if (destaddresses.empty())
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }
  
  resolver_.async_resolve(boost::asio::ip::tcp::resolver::query(proxyhostname, std::to_string(proxyport)), boost::bind(&Socket::SocksHandleProxyResolve, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, routingtype, destaddresses, socksparams, connectcallback));
}

void Socket::SocksHandleProxyResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback)
{
  if (error)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }
  
  GetSocket().async_connect(endpoints->endpoint(), boost::bind(&Socket::SocksHandleConnect, shared_from_this(), boost::asio::placeholders::error, endpoints, routingtype, destaddresses, socksparams, connectcallback));
}

void Socket::SocksHandleConnect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback)
{
  if (error)
  {
    // Try again with another resolved endpoint if there are any
    ++endpoints;
    if (endpoints == boost::asio::ip::tcp::resolver::iterator())
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
      return;
    }

    GetSocket().close();
    GetSocket().async_connect(endpoints->endpoint(), boost::bind(&Socket::SocksHandleConnect, shared_from_this(), boost::asio::placeholders::error, endpoints, routingtype, destaddresses, socksparams, connectcallback));
    return;
  }

  SocksConnect(endpoints, routingtype, destaddresses, socksparams, connectcallback);
}

void Socket::SocksHandleRead(const boost::system::error_code& error, boost::shared_ptr< std::array<unsigned char, 65536> > buffer, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback)
{
  if (error)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  if (buffer->at(0) != 0x05)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }
  
  switch (static_cast<SOCKS5AUTHENTICATION>(buffer->at(1)))
  {
    case SOCKS5AUTHENTICATION::NONE:
    {
      
      break;
    }
    case SOCKS5AUTHENTICATION::USERNAMEPASSWORD:
    {
      if (!socksparams.username_.is_initialized())
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      // Send
      size_t length = 3 + socksparams.username_->length() + socksparams.password_->length();
      std::unique_ptr<unsigned char[]> auth = std::make_unique<unsigned char[]>(length);
      auth[0] = 0x01;
      auth[1] = static_cast<unsigned char>(socksparams.username_->length());
      memcpy(&auth[2], socksparams.username_->data(), socksparams.username_->length());
      if (socksparams.password_.is_initialized())
      {
        auth[2 + socksparams.username_->length()] = static_cast<unsigned char>(socksparams.password_->length());
        memcpy(&auth[3 + socksparams.username_->length()], socksparams.password_->data(), socksparams.password_->size());
      }
      else
      {
        auth[2 + socksparams.username_->length()] = 0;
      
      }

      boost::system::error_code err;
      boost::asio::write(GetSocket(), boost::asio::buffer(auth.get(), length), err);
      if (err)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      // Receive
      boost::system::error_code ec;
      GetSocket().read_some(boost::asio::buffer(*buffer, buffer->size()), ec);
      if (ec)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      if (buffer->at(0) != 0x01)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      if (buffer->at(1) != 0x00)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      break;
    }
    default:
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
      return;
    }
  }
  
  // Request the connection
  std::vector<unsigned char> request =
  {
    0x05, // Version
    static_cast<unsigned char>(routingtype), // Stream
    0x00, // Reserved
    static_cast<unsigned char>(destaddresses.front().destinationtype_)
  };

  request.insert(request.end(), destaddresses.front().address_.begin(), destaddresses.front().address_.end());

  request.push_back(destaddresses.front().port_ >> 8);
  request.push_back(destaddresses.front().port_ & 0xFF);
  
  destaddresses.pop_front();

  boost::system::error_code err;
  boost::asio::write(GetSocket(), boost::asio::buffer(request.data(), request.size()), err);
  if (err)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  GetSocket().async_read_some(boost::asio::buffer(*buffer, 4), boost::bind(&Socket::SocksHandleResponse, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, buffer, endpoints, routingtype, destaddresses, socksparams, connectcallback));
}

void Socket::SocksHandleResponse(const boost::system::error_code& error, std::size_t numbytes, boost::shared_ptr< std::array<unsigned char, 65536> > buffer, boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, std::deque<DESTADDRESS> destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)> connectcallback)
{
  if (error)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  if (buffer->at(0) != 0x05)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  if (buffer->at(1) != 0x00)
  {
    // If we don't have any alternative addresses, give up
    if (destaddresses.empty())
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
      return;
    }
    
    // Attemp to connect to an alternative destination address
    GetSocket().close();
    GetSocket().async_connect(endpoints->endpoint(), boost::bind(&Socket::SocksHandleConnect, shared_from_this(), boost::asio::placeholders::error, endpoints, routingtype, destaddresses, socksparams, connectcallback));
    return;
  }

  if (buffer->at(2) != 0x00)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  // Address
  boost::system::error_code err;
  std::string address;
  switch (static_cast<SOCKS5DESTINATION>(buffer->at(3)))
  {
    case SOCKS5DESTINATION::IPV4:
    {
      GetSocket().read_some(boost::asio::buffer(*buffer, 4), err);
      if (err)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      try
      {
        boost::asio::ip::address_v4 addr({ buffer->at(0), buffer->at(1), buffer->at(2), buffer->at(3) });
        address = addr.to_string();
      }
      catch (...)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      break;
    }
    case SOCKS5DESTINATION::IPV6:
    {
      GetSocket().read_some(boost::asio::buffer(*buffer, 16), err);
      if (err)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      try
      {
        boost::asio::ip::address_v6 addr({ buffer->at(0), buffer->at(1), buffer->at(2), buffer->at(3), buffer->at(4), buffer->at(5), buffer->at(6), buffer->at(7), buffer->at(8), buffer->at(9), buffer->at(10), buffer->at(11), buffer->at(12), buffer->at(13), buffer->at(14), buffer->at(15) });
        address = addr.to_string();
      }
      catch (...)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }

      break;
    }
    case SOCKS5DESTINATION::DOMAIN:
    {
      GetSocket().read_some(boost::asio::buffer(*buffer, 1), err);
      if (err)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }
      
      GetSocket().read_some(boost::asio::buffer(*buffer, buffer->at(0)), err);
      if (err)
      {
        connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
        return;
      }
      
      address = std::string(reinterpret_cast<char*>(&buffer->at(1)), buffer->at(0));
      break;
    }
    default:
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
      return;
    }
  }

  // Port
  GetSocket().read_some(boost::asio::buffer(*buffer, 2), err);
  if (err)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  connectcallback(boost::system::errc::make_error_code(boost::system::errc::success), address, ((buffer->at(0) << 8) | (buffer->at(1))));
}

void Socket::HTTPConnect(const std::string& hostname, unsigned short port, const std::string& destaddress, unsigned short destport, const std::string& username, const std::string& password, const boost::function<void(const boost::system::error_code&)>& connectcallback)
{
  boost::system::error_code ec;
  const boost::asio::ip::address address = boost::asio::ip::address::from_string(hostname, ec);
  if (ec)
  {
    resolver_.async_resolve(boost::asio::ip::tcp::resolver::query(hostname, std::to_string(port)), boost::bind(&Socket::HTTPHandleProxyResolve, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, HTTPPARAMS(hostname, port, destaddress, destport, username, password), connectcallback));

  }
  else
  {
    GetSocket().async_connect(boost::asio::ip::tcp::endpoint(address, port), boost::bind(&Socket::HTTPHandleConnect, shared_from_this(), boost::asio::placeholders::error, HTTPPARAMS(hostname, port, destaddress, destport, username, password), connectcallback));

  }
}

void Socket::SocksConnect(ROUTINGTYPE routingtype, const std::string& hostname, uint16_t port, const std::string& desthostname, uint16_t destport, bool remotedns, const std::string& username, const std::string& password, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)>& connectcallback)
{
  // Get the byte form of the address and its type
  boost::system::error_code ec;
  const boost::asio::ip::address address = boost::asio::ip::address::from_string(desthostname, ec);
  if (!ec)
  {
    DESTADDRESS destaddress;
    if (address.is_v4())
    {
      auto bytes = address.to_v4().to_bytes();
      destaddress = { SOCKS5DESTINATION::IPV4, std::vector<unsigned char>(bytes.begin(), bytes.end()), destport };
    }
    else if (address.is_v6())
    {
      auto bytes = address.to_v6().to_bytes();
      destaddress = { SOCKS5DESTINATION::IPV6, std::vector<unsigned char>(bytes.begin(), bytes.end()), destport };
    }

    resolver_.async_resolve(boost::asio::ip::tcp::resolver::query(hostname, std::to_string(port)), boost::bind(&Socket::SocksHandleProxyResolve, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, routingtype, std::deque<DESTADDRESS>({ destaddress }), SOCKSPARAMS(username, password), connectcallback));
  }
  else
  {
    // Max hostname size for SOCKS5 is 1 byte
    if (desthostname.size() > 255)
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
      return;
    }

    if (remotedns)
    {
      resolver_.async_resolve(boost::asio::ip::tcp::resolver::query(desthostname, std::to_string(destport)), boost::bind(&Socket::SocksHandleDestResolve, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, routingtype, hostname, port, SOCKSPARAMS(username, password), connectcallback));

    }
    else
    {
      std::vector<unsigned char> domain;
      domain.push_back(static_cast<unsigned char>(desthostname.size()));
      domain.insert(domain.end(), desthostname.begin(), desthostname.end());

      DESTADDRESS destaddress = { SOCKS5DESTINATION::DOMAIN, domain, destport };
      resolver_.async_resolve(boost::asio::ip::tcp::resolver::query(hostname, std::to_string(port)), boost::bind(&Socket::SocksHandleProxyResolve, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::iterator, routingtype, std::deque<DESTADDRESS>({ destaddress }), SOCKSPARAMS(username, password), connectcallback));
    }
  }
}

void Socket::SocksConnect(boost::asio::ip::tcp::resolver::iterator endpoints, ROUTINGTYPE routingtype, const std::deque<DESTADDRESS>& destaddresses, const SOCKSPARAMS socksparams, const boost::function<void(const boost::system::error_code&, const std::string&, uint16_t)>& connectcallback)
{
  std::array<char, 4> greeting;
  unsigned int length = 0;
  if (socksparams.username_.is_initialized())
  {
    if ((socksparams.username_->size() > 255) || (socksparams.password_.is_initialized() && (socksparams.password_->size() > 255)))
    {
      connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
      return;
    }

    length = 4;
    greeting = std::array<char, 4>(
    {
      0x05, // Version
      2, // Number of authentication methods
      static_cast<char>(SOCKS5AUTHENTICATION::NONE),
      static_cast<char>(SOCKS5AUTHENTICATION::USERNAMEPASSWORD)
    });
  }
  else
  {
    length = 3;
    greeting = std::array<char, 4>(
    {
      0x05, // Version
      1, // Number of authentication methods
      static_cast<char>(SOCKS5AUTHENTICATION::NONE)
    });
  }
  
  boost::system::error_code err;
  boost::asio::write(GetSocket(), boost::asio::buffer(greeting, length), err);
  if (err)
  {
    connectcallback(boost::system::errc::make_error_code(boost::system::errc::protocol_error), std::string(), 0);
    return;
  }

  boost::shared_ptr< std::array<unsigned char, 65536> > buffer = boost::make_shared< std::array<unsigned char, 65536> >();
  boost::asio::async_read(GetSocket(), boost::asio::buffer(*buffer, 2), boost::bind(&Socket::SocksHandleRead, shared_from_this(), boost::asio::placeholders::error, buffer, endpoints, routingtype, destaddresses, socksparams, connectcallback));
}

}
