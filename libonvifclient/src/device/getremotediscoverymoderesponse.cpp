// getremotediscoverymoderesponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetRemoteDiscoveryModeResponse::GetRemoteDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetRemoteDiscoveryModeResponse::GetRemoteDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DISCOVERYMODE> remotediscoverymode) :
  Response(client, localendpoint, latency, message),
  remotediscoverymode_(remotediscoverymode)
{

}

GetRemoteDiscoveryModeResponse::~GetRemoteDiscoveryModeResponse()
{

}

}

}
