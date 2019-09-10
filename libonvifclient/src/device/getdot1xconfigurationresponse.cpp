// getdot1xconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDot1XConfigurationResponse::GetDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& dot1xconfigurationtoken) :
  Response(client, localendpoint, latency, message),
  dot1xconfigurationtoken_(dot1xconfigurationtoken)
{

}

GetDot1XConfigurationResponse::GetDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& dot1xconfigurationtoken, const boost::optional<Dot1XConfiguration>& dot1xconfiguration) :
  Response(client, localendpoint, latency, message),
  dot1xconfigurationtoken_(dot1xconfigurationtoken),
  dot1xconfiguration_(dot1xconfiguration)
{
  
}

GetDot1XConfigurationResponse::~GetDot1XConfigurationResponse()
{
  
}

}

}
