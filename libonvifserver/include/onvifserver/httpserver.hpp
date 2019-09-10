// httpserver.hpp
//

#ifndef IDY4ZS1O3MQG57Z8CVETH5LQABI4NRAL9F
#define IDY4ZS1O3MQG57Z8CVETH5LQABI4NRAL9F

///// Includes /////

#include <boost/regex.hpp>
#include <chrono>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include "service.hpp"
#include "user.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Globals /////

const size_t MAXBUFFER = 65536; // The maximum buffer size we trust before sending back an error(anything requests larger than this are suspicious and this allows us to have a fixed sized buffer which is convenient)

///// Classes /////

class HttpServer
{
 public:

  static void GenericCallback(evhttp_request* request, void* arg);

  HttpServer(const std::string& realm);
  ~HttpServer();

  int Init(const std::string& address, const uint16_t port);
  int Destroy();

  int AddServer(const std::string& uri, Service* server);
  int RemoveServer(const std::string& uri);

  int Update();

  inline uint16_t GetPort() const { return port_; }

  void SetUserTimeout(const std::chrono::seconds& usertimeout) { std::lock_guard<std::mutex> lock(mutex_); usertimeout_ = usertimeout; }
  const std::chrono::seconds& GetUserTimeout() const { return usertimeout_; }

  void SetNonceTimeout(const std::chrono::seconds& noncetimeout) { std::lock_guard<std::mutex> lock(mutex_); noncetimeout_ = noncetimeout; }
  const std::chrono::seconds& GetNonceTimeout() const { return noncetimeout_; }

  void SetPrevNonceTimeout(const std::chrono::seconds& prevnoncetimeout) { std::lock_guard<std::mutex> lock(mutex_); prevnoncetimeout_ = prevnoncetimeout; }
  const std::chrono::seconds& GetPrevNonceTimeout() const { return prevnoncetimeout_; }
  

 private:

  void SendResponse(evhttp_request* request, const ServerResponse& response);
  Nonce GenerateNonce();

  static const std::string alphanumeric_;

  static const boost::regex contenttypemultipart_;
  static const boost::regex digest_;
  static const boost::regex parameters_;
  static const boost::regex contenttype_;
  static const boost::regex contentid_;

  const std::string realm_;

  std::mt19937 rng_;
  std::uniform_int_distribution<unsigned int> distribution_;

  event_base* base_;
  evhttp* http_;
  evhttp_bound_socket* handle_;
  uint16_t port_;

  char inbuffer_[MAXBUFFER];
  evbuffer* outbuffer_;

  std::mutex mutex_;

  std::chrono::seconds usertimeout_;
  std::chrono::seconds noncetimeout_;
  std::chrono::seconds prevnoncetimeout_;

  std::vector< std::pair<std::string, Service*> > servers_; // uri, server

  std::vector< std::pair< evhttp_request*, boost::shared_future<ServerResponse> > > responses_;

  std::vector<User> users_;

};

}

}

#endif
