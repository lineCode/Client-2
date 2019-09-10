// setrelayoutputsettingsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

SetRelayOutputSettingsResponse::SetRelayOutputSettingsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RelayOutput& relayoutput) :
  Response(client, localendpoint, latency, message),
  relayoutput_(relayoutput)
{

}

SetRelayOutputSettingsResponse::~SetRelayOutputSettingsResponse()
{

}

}

}
