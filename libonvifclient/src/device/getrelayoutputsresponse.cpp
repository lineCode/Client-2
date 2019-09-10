// getrelayoutputsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetRelayOutputsResponse::GetRelayOutputsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetRelayOutputsResponse::GetRelayOutputsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<RelayOutput>& relayoutputs) :
  Response(client, localendpoint, latency, message),
  relayoutputs_(relayoutputs)
{

}

GetRelayOutputsResponse::~GetRelayOutputsResponse()
{

}

}

}
