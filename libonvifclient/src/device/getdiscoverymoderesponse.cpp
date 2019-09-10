// getdiscoverymoderesponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDiscoveryModeResponse::GetDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DISCOVERYMODE> discoverymode) :
  Response(client, localendpoint, latency, message),
  discoverymode_(discoverymode)
{

}

GetDiscoveryModeResponse::GetDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDiscoveryModeResponse::~GetDiscoveryModeResponse()
{

}

}

}
