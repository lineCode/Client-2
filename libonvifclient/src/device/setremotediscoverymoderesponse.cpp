// setremotediscoverymoderesponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetRemoteDiscoveryModeResponse::SetRemoteDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DISCOVERYMODE remotediscoverymode) :
  Response(client, localendpoint, latency, message),
  remotediscoverymode_(remotediscoverymode)
{

}

SetRemoteDiscoveryModeResponse::~SetRemoteDiscoveryModeResponse()
{

}

}

}
