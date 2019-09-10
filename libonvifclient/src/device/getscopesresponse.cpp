// getscopesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetScopesResponse::GetScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetScopesResponse::GetScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Scope>& scopes) :
  Response(client, localendpoint, latency, message),
  scopes_(scopes)
{

}

GetScopesResponse::~GetScopesResponse()
{

}

std::string GetScopesResponse::GetName() const
{
  // Get the names
  std::vector<std::string> names;
  for (const auto& scope : scopes_)
  {
    if (scope.scopeitem_.is_initialized() && (scope.scopeitem_->compare(0, 27, "onvif://www.onvif.org/name/") == 0))
    {
      names.push_back(UrlDecode(scope.scopeitem_->substr(27, std::string::npos)));

    }
  }

  if (names.empty())
  {

    return std::string();
  }

  return boost::algorithm::join(names, " ");
}

}

}
