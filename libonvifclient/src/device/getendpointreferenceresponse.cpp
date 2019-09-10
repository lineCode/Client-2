// getendpointreferenceresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetEndpointReferenceResponse::GetEndpointReferenceResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetEndpointReferenceResponse::GetEndpointReferenceResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& guid) :
  Response(client, localendpoint, latency, message), guid_(guid)
{

}

GetEndpointReferenceResponse::~GetEndpointReferenceResponse()
{

}

}

}
