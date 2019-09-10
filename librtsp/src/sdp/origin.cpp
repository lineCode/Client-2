// origin.cpp
//

///// Includes /////

#include "rtsp/sdp/origin.hpp"

#include <boost/regex.hpp>

///// Namespaces /////

namespace rtsp
{

namespace sdp
{

///// Methods /////

Origin::Origin()
{

}

Origin::Origin(const std::string& text)
{
  static const boost::regex regex("[\\s]*o[\\s]*=[\\s]*([\\w-]+)[\\s]*([\\d]+)[\\s]*([\\w]+)[\\s]*([\\w]+)[\\s]*([\\w]+)[\\s]*([\\w\\.]+)");
  boost::smatch match;
  if (!boost::regex_search(text, match, regex))
  {

    return;
  }

  username_ = match[1];
  sessionid_ = std::stoull(match[2]);
  sessionversion_ = std::stoull(match[3]);

  nettype_ = NetTypeFromString(match[4]);
  addresstype_ = AddressTypeFromString(match[5]);
  address_ = match[6];
}

Origin::Origin(const std::string& username, uint64_t sessionid, uint64_t sessionversion, NETTYPE nettype, ADDRESSTYPE addresstype, const std::string& address) :
  username_(username),
  sessionid_(sessionid),
  sessionversion_(sessionversion),
  nettype_(nettype),
  addresstype_(addresstype),
  address_(address)
{

}

Origin::~Origin()
{

}

void Origin::Clear()
{
  username_.reset();
  sessionid_.reset();
  sessionversion_.reset();
  nettype_.reset();
  addresstype_.reset();
  address_.reset();
}

bool Origin::IsValid() const
{
  return (nettype_.is_initialized() && (nettype_ != NETTYPE_INVALID) && sessionid_.is_initialized() && sessionversion_.is_initialized() && addresstype_.is_initialized() && (addresstype_ != ADDRESSTYPE_INVALID) && address_.is_initialized() && !address_->empty());
}

std::string Origin::ToString() const
{
  if (!IsValid())
  {

    return std::string();
  }

  std::string username = (!username_.is_initialized() || username_->empty()) ? std::string("-") : *username_;

  return (std::string("o=") + username + std::string(" ") + std::to_string(*sessionid_) + std::string(" ") + std::to_string(*sessionversion_) + std::string(" ") + sdp::ToString(*nettype_) + std::string(" ") + sdp::ToString(*addresstype_) + std::string(" ") + *address_);
}

bool Origin::operator==(const Origin& rhs) const
{
  return ((username_ == rhs.username_) && (sessionid_ == rhs.sessionid_) && (sessionversion_ == rhs.sessionversion_) && (nettype_ == rhs.nettype_) && (addresstype_ == rhs.addresstype_) && (address_ == rhs.address_));
}

}

}
