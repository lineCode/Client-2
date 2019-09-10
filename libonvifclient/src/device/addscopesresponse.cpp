// addscopesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

AddScopesResponse::AddScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& scopes) :
  Response(client, localendpoint, latency, message),
  scopes_(scopes)
{

}

AddScopesResponse::~AddScopesResponse()
{

}

}

}
