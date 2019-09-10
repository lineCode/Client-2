// setzeroconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetZeroConfigurationResponse::SetZeroConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, bool enabled) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken),
  enabled_(enabled)
{

}

SetZeroConfigurationResponse::~SetZeroConfigurationResponse()
{

}

}

}
