// setdiscoverymoderesponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetDiscoveryModeResponse::SetDiscoveryModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DISCOVERYMODE discoverymode) :
  Response(client, localendpoint, latency, message),
  discoverymode_(discoverymode)
{

}

SetDiscoveryModeResponse::~SetDiscoveryModeResponse()
{

}

}

}
