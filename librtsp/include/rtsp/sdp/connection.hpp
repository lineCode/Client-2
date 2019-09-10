// connection.hpp
//

#ifndef IDISK2DNIKJFUZOJH2YPF8JR3ZXP22KDM0
#define IDISK2DNIKJFUZOJH2YPF8JR3ZXP22KDM0

///// Includes /////

#include <boost/asio.hpp>

#include <assert.h>
#include <boost/optional.hpp>
#include <string>
#include <vector>

#include "addresstype.hpp"
#include "nettype.hpp"

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Structures /////

struct CONNECTIONDATA
{
  CONNECTIONDATA() :
    nettype_(NETTYPE_INVALID), addresstype_(ADDRESSTYPE_INVALID), numaddresses_(1)
  {

  }

  CONNECTIONDATA(NETTYPE nettype, ADDRESSTYPE addresstype, const boost::asio::ip::address& address, const boost::optional<int>& ttl, int numaddresses) :
    nettype_(nettype),
    addresstype_(addresstype),
    address_(address),
    ttl_(ttl),
    numaddresses_(numaddresses)
  {

  }

  bool IsValid() const
  {
    if ((nettype_ == NETTYPE_INVALID) || (addresstype_ == ADDRESSTYPE_INVALID))
    {

      return false;
    }

    if (address_.is_multicast() && address_.is_v4())
    {
      if (!ttl_.is_initialized() || (*ttl_ < 0) || (*ttl_ > 255))
      {

        return false;
      }
    }

    if ((addresstype_ == ADDRESSTYPE_IP4) && !address_.is_v4())
    {

      return false;
    }

    if ((addresstype_ == ADDRESSTYPE_IP6) && !address_.is_v6())
    {

      return false;
    }

    return true;
  }

  std::string ToString() const
  {
    if (!IsValid())
    {

      return std::string();
    }

    std::string address = address_.to_string();
    if (address_.is_multicast())
    {
      if (address_.is_v4())
      {
        if (ttl_.is_initialized())
        {
          address += std::string("/") + std::to_string(*ttl_) + std::string("/") + std::to_string(numaddresses_);

        }
      }
      else if (address_.is_v6())
      {
        address += std::string("/") + std::to_string(numaddresses_);

      }
      else
      {
        assert(false);

      }
    }

    return (std::string("c=") + sdp::ToString(nettype_) + std::string(" ") + sdp::ToString(addresstype_) + std::string(" ") + address);
  }

  bool operator==(const CONNECTIONDATA& rhs) const
  {
    return ((nettype_ == rhs.nettype_) && (addresstype_ == rhs.addresstype_) && (address_ == rhs.address_) && (ttl_ == rhs.ttl_) && (numaddresses_ == rhs.numaddresses_));
  }

  NETTYPE nettype_;
  ADDRESSTYPE addresstype_;
  boost::asio::ip::address address_;
  boost::optional<int> ttl_;
  int numaddresses_;
};

///// Classes /////

class Connection
{
public:

  Connection();
  Connection(const std::vector<CONNECTIONDATA>& connections);
  Connection(const std::vector<std::string>& connections);
  ~Connection();

  void AddConnection(const std::string& connection);

  void Clear();
  bool IsValid() const;
  std::vector<std::string> ToString() const;

  bool operator==(const Connection& rhs) const;

  std::vector<CONNECTIONDATA> connections_;

};

}

}

#endif
