// removescopesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

RemoveScopesResponse::RemoveScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

RemoveScopesResponse::RemoveScopesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& scopes) :
  Response(client, localendpoint, latency, message),
  scopes_(scopes)
{

}

RemoveScopesResponse::~RemoveScopesResponse()
{

}

}

}
