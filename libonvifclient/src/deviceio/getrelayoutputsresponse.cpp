// getrelayoutputsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetRelayOutputsResponse::GetRelayOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetRelayOutputsResponse::GetRelayOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<RelayOutput>& relayoutputs) :
  Response(client, localendpoint, latency, message),
  relayoutputs_(relayoutputs)
{

}

GetRelayOutputsResponse::~GetRelayOutputsResponse()
{

}

}

}
