// setrelayoutputstateresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

SetRelayOutputStateResponse::SetRelayOutputStateResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string relayoutputtoken, RELAYLOGICALSTATE logicalstate) :
  Response(client, localendpoint, latency, message),
  relayoutputtoken_(relayoutputtoken),
  logicalstate_(logicalstate)
{

}

SetRelayOutputStateResponse::~SetRelayOutputStateResponse()
{

}

}

}
