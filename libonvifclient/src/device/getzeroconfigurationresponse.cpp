// getzeroconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetZeroConfigurationResponse::GetZeroConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetZeroConfigurationResponse::GetZeroConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<NetworkZeroConfiguration>& zeroconfiguration) :
  Response(client, localendpoint, latency, message),
  zeroconfiguration_(zeroconfiguration)
{

}

GetZeroConfigurationResponse::~GetZeroConfigurationResponse()
{

}

}

}
