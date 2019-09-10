// proxyparams.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include "socket/proxyparams.hpp"

///// Namespaces /////

namespace sock
{

///// Methods /////

ProxyParams::ProxyParams() :
  type_(sock::PROXYTYPE_NONE),
  port_(0),
  remotedns_(false)
{

}

ProxyParams::ProxyParams(sock::PROXYTYPE type, const std::string& hostname, uint16_t port, bool remotedns, const std::string& username, const std::string& password) :
  type_(type),
  hostname_(hostname),
  port_(port),
  remotedns_(remotedns),
  username_(username),
  password_(password)
{


}

ProxyParams::~ProxyParams()
{

}

void ProxyParams::Clear()
{
  type_ = sock::PROXYTYPE_NONE;
  hostname_.clear();
  port_ = 0;
  remotedns_ = false;
  username_.clear();
  password_.clear();
}

bool ProxyParams::IsValid() const
{
  switch (type_)
  {
    case sock::PROXYTYPE_NONE:
    {

      return true;
    }
    case sock::PROXYTYPE_HTTP:
    {
      if (hostname_.size() && (port_ != 0))
      {

        return true;
      }
    }
    case sock::PROXYTYPE_SOCKS5:
    {
      if (hostname_.size() && (port_ != 0))
      {

        return true;
      }
    }
  }

  return false;
}

std::string ProxyParams::ToString()
{
  if (hostname_.empty())
  {

    return std::string();
  }

  // Schema
  std::string url;
  switch (type_)
  {
    case sock::PROXYTYPE_HTTP:
    {
      url += "http://";
      break;
    }
    case sock::PROXYTYPE_SOCKS5:
    {
      if (remotedns_)
      {
        url += "socks5h://";

      }
      else
      {
        url += "socks5://";

      }
      break;
    }
    default:
    {

      return std::string();
    }
  }

  // Username:password
  if (username_.size() && password_.size())
  {
    url += username_ + ':' + password_ + '@';

  }

  // Hostname:port
  url += hostname_;

  if (port_)
  {
    url += ':' + std::to_string(port_);

  }

  return url;
}

bool ProxyParams::operator==(const ProxyParams& rhs) const
{
  return ((type_ == rhs.type_) && (hostname_ == rhs.hostname_) && (port_ == rhs.port_) && (remotedns_ == rhs.remotedns_) && (username_ == rhs.username_) && (password_ == rhs.password_));
}

}
