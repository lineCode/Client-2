// setrelayoutputsettingsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetRelayOutputSettingsResponse::SetRelayOutputSettingsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& relayoutputtoken, const RelayOutputSettings& properties) :
  Response(client, localendpoint, latency, message),
  relayoutputtoken_(relayoutputtoken),
  properties_(properties)
{

}

SetRelayOutputSettingsResponse::~SetRelayOutputSettingsResponse()
{

}

}

}
