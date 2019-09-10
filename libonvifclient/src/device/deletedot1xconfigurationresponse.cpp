// deletedot1xconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

DeleteDot1XConfigurationResponse::DeleteDot1XConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& dot1xconfigurationtoken) :
  Response(client, localendpoint, latency, message),
  dot1xconfigurationtoken_(dot1xconfigurationtoken)
{

}

DeleteDot1XConfigurationResponse::~DeleteDot1XConfigurationResponse()
{

}

}

}
