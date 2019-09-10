// service.hpp
//

///// Includes /////

#include "onvifserver/service.hpp"

#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

///// Functions /////

std::string ToString(evhttp_cmd_type requesttype)
{
  switch (requesttype)
  {
    case EVHTTP_REQ_GET:
    {

      return std::string("GET");
    }
    case EVHTTP_REQ_POST:
    {

      return std::string("POST");
    }
    default:
    {

      return std::string();
    }
  }
}

///// Methods /////

Service::Service()
{

}

Service::~Service()
{

}

void Service::Update()
{

}

std::string Service::DigestResponse(const std::string& ha1, evhttp_cmd_type requesttype, const std::string& uri, const std::string& nonce, const std::string& clientnc, const std::string& cnonce, const std::string& qop) const
{
  return utility::Md5(ha1 + std::string(":") + nonce + std::string(":") + clientnc + std::string(":") + cnonce + std::string(":") + qop + std::string(":") + utility::Md5(ToString(requesttype) + std::string(":") + uri));
}

}

}
