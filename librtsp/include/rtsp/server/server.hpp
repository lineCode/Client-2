// server.hpp
//

#ifndef IDG0ZY448MKE3VU72JDZAN9KA8VENUP10I
#define IDG0ZY448MKE3VU72JDZAN9KA8VENUP10I

///// Includes /////

#include <boost/asio.hpp>

#include <boost/regex.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <stdint.h>
#include <thread>
#include <vector>

#include "stream.hpp"

///// Declerations /////

namespace utility
{
class IoServicePool;
}

///// Namespaces /////

namespace rtsp
{

namespace server
{

///// Declarations /////

class Connection;
class HTTPConnection;
class RTSPConnection;
class Stream;
class UserConnection;

///// Classes /////

class Server : public boost::enable_shared_from_this<Server>
{
 public:

  Server(const std::string& realm, utility::IoServicePool& ioservicepool);
  virtual ~Server();
  
  int Init(const boost::asio::ip::tcp& tcp, const std::vector<uint16_t>& ports, const std::vector<uint16_t> httpports);
  void Destroy();

  inline std::vector< boost::shared_ptr<Stream> > GetStreams() const { std::lock_guard<std::mutex> lock(streamsmutex_); return streams_; }
  boost::shared_ptr<Stream> GetStream(const std::string& path) const;
  void SetBasicAuthentication(const bool basicauthentication); // Will set BASIC authentication for all current streams
  void SetDigestAuthentication(const bool digestauthentication); // Will set DIGEST authentication for all current streams

  virtual boost::shared_ptr<UserConnection> CreateConnection(); // This can be overridden to create a user defined Connection type
  boost::shared_ptr<HTTPConnection> GetHTTPConnection(const std::string& xsessioncookie) const;
  bool AddHTTPConnection(const boost::shared_ptr<HTTPConnection>& connection);
  boost::shared_ptr<HTTPConnection> TakeHTTPConnection(const std::string& xsessioncookie);
  void DestroyRTSPConnection(const boost::shared_ptr<RTSPConnection>& connection);
  void DestroyHTTPConnection(const boost::shared_ptr<HTTPConnection>& connection);
  void DestroyConnection(const boost::shared_ptr<Connection>& connection);

  int AddStream(const boost::shared_ptr<Stream>& stream);
  int RemoveStream(const boost::shared_ptr<Stream>& stream);

  inline utility::IoServicePool& GetIOServicePool() { return ioservicepool_; }

  const std::string& GetRealm() const { return realm_; }

  std::vector<uint16_t> GetPorts() const;
  std::vector<uint16_t> GetHTTPPorts() const;

 private:

  void RTSPAccept(boost::asio::ip::tcp::acceptor* acceptor);
  void HandleRTSPAccept(const boost::system::error_code& error, boost::shared_ptr<RTSPConnection> connection, boost::asio::ip::tcp::acceptor* acceptor);

  void HTTPAccept(boost::asio::ip::tcp::acceptor* acceptor);
  void HandleHTTPAccept(const boost::system::error_code& error, boost::shared_ptr<HTTPConnection> connection, boost::asio::ip::tcp::acceptor* acceptor);

  const std::string realm_;

  utility::IoServicePool& ioservicepool_;

  std::mt19937 rng_;
  std::uniform_int_distribution<unsigned int> distribution_;

  mutable std::recursive_mutex acceptorsmutex_;
  std::vector< boost::shared_ptr<boost::asio::ip::tcp::acceptor> > acceptors_;
  std::vector< boost::shared_ptr<boost::asio::ip::tcp::acceptor> > httpacceptors_;

  mutable std::mutex streamsmutex_;
  std::vector< boost::shared_ptr<Stream> > streams_;

  mutable std::mutex rtspmutex_;
  std::vector< boost::shared_ptr<RTSPConnection> > rtspconnections_;

  mutable std::mutex httpmutex_;
  std::vector< boost::shared_ptr<HTTPConnection> > httpconnections_; // This include awaiting GET connections and all POST connections

};

}

}

#endif
