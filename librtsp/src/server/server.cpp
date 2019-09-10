// server.cpp
//

///// Includes /////

#include "rtsp/server/server.hpp"

#include <algorithm>
#include <boost/algorithm/string/regex.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/regex.hpp>
#include <network/uri.hpp>
#include <utility/ioservicepool.hpp>
#include <utility/utility.hpp>

#include "rtsp/rtcp/packet.hpp"
#include "rtsp/rtcp/packets.hpp"
#include "rtsp/rtsprequest.hpp"
#include "rtsp/server/httpconnection.hpp"
#include "rtsp/server/rtspconnection.hpp"
#include "rtsp/server/userconnection.hpp"

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Methods /////

Server::Server(const std::string& realm, utility::IoServicePool& ioservicepool) :
  realm_(realm),
  ioservicepool_(ioservicepool),
  rng_(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())),
  distribution_(0, static_cast<unsigned int>(61)) // This is a fixed value because it may get initialised before alphanumeric_ member that it uses
{

}

Server::~Server()
{
  Destroy();
  
}

int Server::Init(const boost::asio::ip::tcp& tcp, const std::vector<uint16_t>& ports, const std::vector<uint16_t> httpports)
{
  Destroy();

  for (const uint16_t port : ports)
  {
    boost::system::error_code err;
    boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor = boost::make_shared<boost::asio::ip::tcp::acceptor>(ioservicepool_.GetIoService());
    if (acceptor->open(tcp, err))
    {

      return 1;
    }

    if (acceptor->bind(boost::asio::ip::tcp::endpoint(tcp, port), err))
    {
      Destroy();
      return 2;
    }

    if (acceptor->listen(SOMAXCONN, err))
    {
      Destroy();
      return 3;
    }

    RTSPAccept(acceptor.get());

    {
      std::lock_guard<std::recursive_mutex> lock(acceptorsmutex_);
      acceptors_.emplace_back(std::move(acceptor));
    }
  }

  for (const uint16_t httpport : httpports)
  {
    boost::system::error_code err;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(ioservicepool_.GetIoService());
    if (acceptor->open(tcp, err))
    {

      return 1;
    }

    if (acceptor->bind(boost::asio::ip::tcp::endpoint(tcp, httpport), err))
    {
      Destroy();
      return 2;
    }

    if (acceptor->listen(SOMAXCONN, err))
    {
      Destroy();
      return 3;
    }

    HTTPAccept(acceptor.get());

    {
      std::lock_guard<std::recursive_mutex> lock(acceptorsmutex_);
      httpacceptors_.emplace_back(std::move(acceptor));
    }
  }

  return 0;
}

void Server::Destroy()
{
  {
    std::lock_guard<std::recursive_mutex> lock(acceptorsmutex_);
    for (auto& acceptor : acceptors_)
    {
      boost::system::error_code err;
      acceptor->close(err);
    }
    acceptors_.clear();
  }

  {
    std::lock_guard<std::mutex> lock(streamsmutex_);
    for (auto& stream : streams_)
    {
      stream->Destroy();

    }
    streams_.clear();
  }
  
  {
    std::lock_guard<std::mutex> lock(rtspmutex_);
    rtspconnections_.clear();
  }

  {
    std::lock_guard<std::mutex> lock(httpmutex_);
    httpconnections_.clear();
  }
}

boost::shared_ptr<Stream> Server::GetStream(const std::string& path) const
{
  std::lock_guard<std::mutex> lock(streamsmutex_);
  std::vector< boost::shared_ptr<Stream> >::const_iterator stream = std::find_if(streams_.cbegin(), streams_.cend(), [&path](const boost::shared_ptr<Stream>& stream) { return ((std::string("/") + stream->GetPath()) == path); });
  if (stream == streams_.cend())
  {

    return nullptr;
  }
  return *stream;
}

void Server::SetBasicAuthentication(const bool basicauthentication)
{
  std::lock_guard<std::mutex> lock(streamsmutex_);
  for (auto& stream : streams_)
  {
    stream->SetBasicAuthentication(basicauthentication);
  
  }
}

void Server::SetDigestAuthentication(const bool digestauthentication)
{
  std::lock_guard<std::mutex> lock(streamsmutex_);
  for (auto& stream : streams_)
  {
    stream->SetDigestAuthentication(digestauthentication);
  
  }
}

boost::shared_ptr<UserConnection> Server::CreateConnection()
{
  return nullptr;
}

boost::shared_ptr<HTTPConnection> Server::GetHTTPConnection(const std::string& xsessioncookie) const
{
  std::lock_guard<std::mutex> lock(httpmutex_);
  std::vector< boost::shared_ptr<HTTPConnection> >::const_iterator httpconnection = std::find_if(httpconnections_.cbegin(), httpconnections_.cend(), [&xsessioncookie](const boost::shared_ptr<HTTPConnection>& connection) { return (connection->GetSessionCookie() == xsessioncookie); });
  if (httpconnection == httpconnections_.cend())
  {

    return nullptr;
  }
  return *httpconnection;
}

void Server::DestroyRTSPConnection(const boost::shared_ptr<RTSPConnection>& connection)
{
  // Kill any interleaved channels in the session
  {
    std::lock_guard<std::mutex> lock(streamsmutex_);
    for (const auto& stream : streams_)
    {
      stream->ResetInterleavedConnections(connection);

    }
  }

  {
    std::lock_guard<std::mutex> lock(rtspmutex_);
    const std::vector< boost::shared_ptr<RTSPConnection> >::const_iterator i = std::find(rtspconnections_.begin(), rtspconnections_.end(), connection);
    if (i == rtspconnections_.end())
    {

      return;
    }
    (*i)->Destroy();
    rtspconnections_.erase(i);
  }
}

bool Server::AddHTTPConnection(const boost::shared_ptr<HTTPConnection>& connection)
{
  std::lock_guard<std::mutex> lock(httpmutex_);
  std::vector< boost::shared_ptr<HTTPConnection> >::const_iterator i = std::find(httpconnections_.cbegin(), httpconnections_.cend(), connection);
  if (i != httpconnections_.end())
  {

    return false;
  }

  // Make sure nobody is using this session cookie yet
  i = std::find_if(httpconnections_.cbegin(), httpconnections_.cend(), [&connection](const boost::shared_ptr<HTTPConnection>& i) { return (i->GetSessionCookie() == connection->GetSessionCookie()); });
  if (i != httpconnections_.end())
  {

    return false;
  }

  httpconnections_.emplace_back(connection);
  return true;
}

boost::shared_ptr<HTTPConnection> Server::TakeHTTPConnection(const std::string& xsessioncookie)
{
  std::lock_guard<std::mutex> lock(httpmutex_);
  std::vector< boost::shared_ptr<HTTPConnection> >::iterator i = std::find_if(httpconnections_.begin(), httpconnections_.end(), [&xsessioncookie](const boost::shared_ptr<HTTPConnection>& i) { return (i->GetSessionCookie() == xsessioncookie); });
  if (i == httpconnections_.end())
  {

    return nullptr;
  }
  boost::shared_ptr<HTTPConnection> connection = *i;
  httpconnections_.erase(i);
  return connection;
}

void Server::DestroyHTTPConnection(const boost::shared_ptr<HTTPConnection>& connection)
{
  // Kill any interleaved channels in the session
  {
    std::lock_guard<std::mutex> lock(streamsmutex_);
    for (const auto& stream : streams_)
    {
      stream->ResetInterleavedConnections(connection);

    }
  }

  {
    std::lock_guard<std::mutex> lock(httpmutex_);
    const std::vector< boost::shared_ptr<HTTPConnection> >::const_iterator i = std::find(httpconnections_.cbegin(), httpconnections_.cend(), connection);
    if (i == httpconnections_.end())
    {

      return;
    }
    (*i)->Destroy();
    boost::shared_ptr<HTTPConnection> getconnection = (*i)->GetGetConnection();
    httpconnections_.erase(i);
    if (getconnection)
    {
      DestroyHTTPConnection(getconnection);

    }
  }
}

void Server::DestroyConnection(const boost::shared_ptr<Connection>& connection)
{
  if (connection->GetConnectionType() == CONNECTION_RTSP)
  {
    DestroyRTSPConnection(boost::static_pointer_cast<RTSPConnection>(connection));

  }
  else if (connection->GetConnectionType() == CONNECTION_HTTP)
  {
    DestroyHTTPConnection(boost::static_pointer_cast<HTTPConnection>(connection));

  }
}

int Server::AddStream(const boost::shared_ptr<Stream>& stream)
{
  std::lock_guard<std::mutex> lock(streamsmutex_);
  for (const auto& i : streams_)
  {
    if (i->GetPath() == stream->GetPath())
    {
    
      return 1;
    }
  }
  streams_.push_back(stream);
  return 0;
}

int Server::RemoveStream(const boost::shared_ptr<Stream>& stream)
{
  std::lock_guard<std::mutex> lock(streamsmutex_);
  const std::vector< boost::shared_ptr<Stream> >::const_iterator i = std::find_if(streams_.begin(), streams_.end(), [stream](const boost::shared_ptr<Stream>& i){ return (i == stream); });
  if (i == streams_.end())
  {
    
    return 1;
  }
  streams_.erase(i);
  return 0;
}

std::vector<uint16_t> Server::GetPorts() const
{
  std::lock_guard<std::recursive_mutex> lock(acceptorsmutex_);
  std::vector<uint16_t> ports;
  for (const boost::shared_ptr<boost::asio::ip::tcp::acceptor>& acceptor : acceptors_)
  {
    boost::system::error_code err;
    const auto endpoint = acceptor->local_endpoint(err);
    if (err)
    {

      continue;
    }
    ports.push_back(endpoint.port());
  }
  return ports;
}

std::vector<uint16_t> Server::GetHTTPPorts() const
{
  std::lock_guard<std::recursive_mutex> lock(acceptorsmutex_);
  std::vector<uint16_t> httpports;
  for (const boost::shared_ptr<boost::asio::ip::tcp::acceptor>& httpacceptor : httpacceptors_)
  {
    boost::system::error_code err;
    const auto endpoint = httpacceptor->local_endpoint(err);
    if (err)
    {

      continue;
    }
    httpports.push_back(endpoint.port());
  }
  return httpports;
}

void Server::RTSPAccept(boost::asio::ip::tcp::acceptor* acceptor)
{
  boost::shared_ptr<RTSPConnection> connection = boost::make_shared<RTSPConnection>(ioservicepool_.GetIoService(), shared_from_this(), CreateConnection());
  acceptor->async_accept(connection->GetSocket(), boost::bind(&Server::HandleRTSPAccept, shared_from_this(), boost::asio::placeholders::error, connection, acceptor));
}

void Server::HandleRTSPAccept(const boost::system::error_code& error, boost::shared_ptr<RTSPConnection> connection, boost::asio::ip::tcp::acceptor* acceptor)
{
  if (error)
  {
    Destroy(); // We ignore this because it is so unlikely to occur when listening to 0.0.0.0
    return;
  }

  {
    std::lock_guard<std::mutex> lock(rtspmutex_);
    rtspconnections_.push_back(connection);
  }

  connection->Connected();

  RTSPAccept(acceptor);
}

void Server::HTTPAccept(boost::asio::ip::tcp::acceptor* acceptor)
{
  boost::shared_ptr<HTTPConnection> connection = boost::make_shared<HTTPConnection>(ioservicepool_.GetIoService(), shared_from_this(), CreateConnection());
  acceptor->async_accept(connection->GetSocket(), boost::bind(&Server::HandleHTTPAccept, shared_from_this(), boost::asio::placeholders::error, connection, acceptor));
}

void Server::HandleHTTPAccept(const boost::system::error_code& error, boost::shared_ptr<HTTPConnection> connection, boost::asio::ip::tcp::acceptor* acceptor)
{
  if (error)
  {
    Destroy(); // We ignore this because it is so unlikely to occur when listening to 0.0.0.0
    return;
  }

  {
    std::lock_guard<std::mutex> lock(httpmutex_);
    httpconnections_.push_back(connection);
  }

  connection->Connected();

  HTTPAccept(acceptor);
}

}

}
