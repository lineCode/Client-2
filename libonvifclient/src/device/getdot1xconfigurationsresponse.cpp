// getdot1xconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDot1XConfigurationsResponse::GetDot1XConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDot1XConfigurationsResponse::GetDot1XConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Dot1XConfiguration>& dot1xconfiguration) :
  Response(client, localendpoint, latency, message),
  dot1xconfiguration_(dot1xconfiguration)
{
  
}

GetDot1XConfigurationsResponse::~GetDot1XConfigurationsResponse()
{
  
}

}

}
