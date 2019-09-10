// setnetworkdefaultgatewayresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetNetworkDefaultGatewayResponse::SetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

SetNetworkDefaultGatewayResponse::SetNetworkDefaultGatewayResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& ipv4address, const std::vector<std::string>& ipv6address) :
  Response(client, localendpoint, latency, message),
  ipv4address_(ipv4address),
  ipv6address_(ipv6address)
{

}

SetNetworkDefaultGatewayResponse::~SetNetworkDefaultGatewayResponse()
{

}

}

}
