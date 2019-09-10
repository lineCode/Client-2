// dummyservice.cpp
//

///// Includes /////

#include "dummyservice.hpp"

#include <utility/utility.hpp>

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Methods /////

DummyService::DummyService()
{

}

DummyService::~DummyService()
{

}

bool DummyService::IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const
{
  return restricted_;
}

bool DummyService::DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientnc, const std::string& clientcnonce, const std::string& clientqop, const std::string& clientresponse) const
{
  if (username_ != clientusername)
  {

    return false;
  }

  if (DigestResponse(utility::Md5(clientusername + std::string(":") + clientrealm + std::string(":") + password_), requesttype, clienturi, clientnonce, clientnc, clientcnonce, clientqop) != clientresponse)
  {

    return false;
  }

  return true;
}

boost::shared_future<ServerResponse> DummyService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::OK, response_));
}

}

}

}
