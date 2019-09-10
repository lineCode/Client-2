// wsdiscoverserver.hpp
//

#ifndef IDVFYTIJ7E8DGSBORQ0O0VN8EJCU98821I
#define IDVFYTIJ7E8DGSBORQ0O0VN8EJCU98821I

///// Includes /////

#include <boost/asio.hpp>

#include <array>
#include <boost/enable_shared_from_this.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <Wsdapi.h>
#endif

///// Namespaces /////

namespace onvif
{

namespace wsdiscover
{

///// Classes /////

class WsDiscoverServer : public boost::enable_shared_from_this<WsDiscoverServer>
{
 public:
  
  WsDiscoverServer(boost::asio::io_service& io);
  ~WsDiscoverServer();

  int Init(const boost::asio::ip::address& address, unsigned short port, const bool enabled, const boost::uuids::uuid& wsaddress, const uint64_t instance, const std::vector<std::string>& scopes, const std::vector<std::string>& xaddrs);
  int Destroy();

  int Hello();
  int Bye();

  // These methods determine whether the server should respond to probes or not
  void Enable();
  void Disable();
  inline bool IsEnabled() const { return enabled_; }

  void SetScopes(const std::vector<std::string>& scopes);
  void AddScope(const std::string& scope);
  void AddScopes(const std::vector<std::string>& scopes);
  bool RemoveScope(const std::string& scope);
  void RemoveScopes(const std::vector<std::string>& scopes);
  inline const std::vector<std::string> GetScopes() const { return scopes_; }

  void AddXAddr(const std::string& xaddr);
  int RemoveXAddr(const std::string& xaddr);
  void SetXAddrs(const std::vector<std::string>& xaddrs);
  inline const std::vector<std::string>& GetXAddrs() const { return xaddrs_; }
  
  void HandleReceive(const boost::system::error_code& error, const size_t receivedbytes);

 private:
 
#ifdef _WIN32
  int InitWindows();
  int DestroyWindows();

  IWSDXMLContext* context_;
  IWSDDeviceHost* host_;
#endif

  void HandleReceiveData(const char* buffer, const size_t size);
  bool Filter(const std::vector<std::string>& scopes) const;
  std::string GetScopesString() const;
  std::string GetXAddrsString() const;

  std::mutex mutex_;
  
  boost::asio::ip::udp::socket socketv4_;
  std::array<char, 65536> buffer_; // Maximum UDP packet size 64kb
  boost::asio::ip::udp::endpoint senderendpoint_;

  boost::uuids::random_generator randomgenerator_;

  boost::asio::ip::udp::endpoint listenendpoint_;
  bool enabled_;
  boost::uuids::uuid wsaddress_;
  uint64_t instance_;
  std::vector<std::string> scopes_;
  std::vector<std::string> xaddrs_;
  unsigned int messagenumber_;
  unsigned int metadataversion_;

};

}

}

#endif
