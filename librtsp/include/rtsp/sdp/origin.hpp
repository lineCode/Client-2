// origin.hpp
//

#ifndef IDFTO4ST5E2P0NZCDDEHX5OUFG65RV6MMN
#define IDFTO4ST5E2P0NZCDDEHX5OUFG65RV6MMN

///// Includes /////

#include <boost/optional.hpp>
#include <stdint.h>
#include <string>

#include "addresstype.hpp"
#include "nettype.hpp"

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Classes /////

class Origin
{
public:

  Origin();
  Origin(const std::string& text);
  Origin(const std::string& username, uint64_t sessionid, uint64_t sessionversion, NETTYPE nettype, ADDRESSTYPE addresstype, const std::string& address);
  ~Origin();

  void Clear();
  bool IsValid() const;
  std::string ToString() const;

  bool operator==(const Origin& rhs) const;

  boost::optional<std::string> username_;
  boost::optional<uint64_t> sessionid_;
  boost::optional<uint64_t> sessionversion_;
  boost::optional<NETTYPE> nettype_;
  boost::optional<ADDRESSTYPE> addresstype_;
  boost::optional<std::string> address_; // This can be a hostname or IP4/IP6 address

};

}

}

#endif
