// getnetworkdefaultgatewayresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetNetworkDefaultGatewayResponse::GetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetNetworkDefaultGatewayResponse::GetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<NetworkGateway> networkgateway) :
  Response(client, localendpoint, latency, message),
  networkgateway_(networkgateway)
{

}

GetNetworkDefaultGatewayResponse::~GetNetworkDefaultGatewayResponse()
{

}

}

}
