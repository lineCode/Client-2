// wsdiscoverclient.hpp
//

#ifndef IDK72Y2U3Z3U57XOYKJP1TQAZBYYRDPW7P
#define IDK72Y2U3Z3U57XOYKJP1TQAZBYYRDPW7P

///// Includes /////

#include <boost/asio.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <wsdapi.h>
#endif

///// Namespaces /////

namespace onvif
{

namespace wsdiscover
{

///// Declarations /////

class NotificationSink;

///// Classes /////

class WsDiscoverClient : public boost::enable_shared_from_this<WsDiscoverClient>
{
 friend class NotificationSink;

 public:

  WsDiscoverClient(boost::asio::io_service& io, unsigned short port = 3702);
  ~WsDiscoverClient();

  int Init();
  int Destroy();

  int Broadcast();
  
  void HandleReceive(const boost::system::error_code& error, const size_t receivedbytes);

  boost::signals2::signal<void(const std::vector<std::string>&, const std::vector<std::string>&)> hello_; // Address, Scopes

 private:

#ifdef _WIN32
  int InitWindows();
  void DestroyWindows();
#endif
  void Parse(const size_t receivedbytes);

  const boost::asio::ip::udp::endpoint broadcast_;

  std::recursive_mutex mutex_;

#ifdef _WIN32
  IWSDiscoveryProvider* provider_;
  NotificationSink* sink_;
  IWSDiscoveryProviderNotify* notify_;
  bool wsdapi_; // Was WSDAPI initialisation successful
#endif

  boost::asio::ip::udp::socket socketv4_;
  boost::asio::ip::udp::endpoint endpointv4_;
  std::array<char, 65536> buffer_; // Maximum UDP packet size 64kb

  boost::uuids::random_generator randomgenerator_;

};

}

}

#endif
