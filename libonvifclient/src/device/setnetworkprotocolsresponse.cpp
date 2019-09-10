// setnetworkprotocolsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetNetworkProtocolsResponse::SetNetworkProtocolsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkProtocol>& networkprotocols) :
  Response(client, localendpoint, latency, message),
  networkprotocols_(networkprotocols)
{

}

SetNetworkProtocolsResponse::~SetNetworkProtocolsResponse()
{

}

}

}
