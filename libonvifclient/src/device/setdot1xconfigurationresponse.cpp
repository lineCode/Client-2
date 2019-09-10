// setdot1xconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetDot1XConfigurationResponse::SetDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const Dot1XConfiguration& dot1xconfiguration) :
  Response(client, localendpoint, latency, message),
  dot1xconfiguration_(dot1xconfiguration)
{

}

SetDot1XConfigurationResponse::~SetDot1XConfigurationResponse()
{

}

}

}
