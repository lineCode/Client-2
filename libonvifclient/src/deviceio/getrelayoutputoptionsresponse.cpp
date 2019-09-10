// getrelayoutputoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetRelayOutputOptionsResponse::GetRelayOutputOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& relayoutputtoken) :
  Response(client, localendpoint, latency, message),
  relayoutputtoken_(relayoutputtoken)
{

}

GetRelayOutputOptionsResponse::GetRelayOutputOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& relayoutputtoken, const std::vector<RelayOutputOptions>& relayoutputoptions) :
  Response(client, localendpoint, latency, message),
  relayoutputtoken_(relayoutputtoken),
  relayoutputoptions_(relayoutputoptions)
{

}

GetRelayOutputOptionsResponse::~GetRelayOutputOptionsResponse()
{

}

}

}
