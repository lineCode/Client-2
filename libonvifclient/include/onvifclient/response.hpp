// response.hpp
//

#ifndef ID1MI9PC1ZGGN83HZ3QZIZUAP1EHYGOTGH
#define ID1MI9PC1ZGGN83HZ3QZIZUAP1EHYGOTGH

///// Includes /////

#include <boost/asio/ip/address.hpp>
#include <memory>
#include <stdint.h>
#include <string>

///// Namespaces /////

namespace onvif
{

///// Classes /////

template<typename Client>
class Response
{
 public:

  Response(boost::shared_ptr<Client>& client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
    client_(client),
    localendpoint_(localendpoint),
    latency_(latency),
    message_(message)
  {
  
  }

  virtual ~Response()
  {
  
  }

  inline const boost::asio::ip::address& GetLocalEndpoint() const { return localendpoint_; }
  inline const std::string& Message() const { return message_;  }
  inline bool Success() const { return message_.empty(); }
  inline bool Error() const { return !message_.empty(); }

  boost::shared_ptr<Client> client_;
  boost::asio::ip::address localendpoint_;
  int64_t latency_;
  std::string message_;
};

}

#endif