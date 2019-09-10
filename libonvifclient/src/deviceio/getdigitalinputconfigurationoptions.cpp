// getdigitalinputconfigurationoptions.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetDigitalInputConfigurationOptionsResponse::GetDigitalInputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetDigitalInputConfigurationOptionsResponse::GetDigitalInputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const std::vector<DIGITALIDLESTATE>& idlestate) :
  Response(client, localendpoint, latency, message),
  token_(token),
  idlestate_(idlestate)
{

}

GetDigitalInputConfigurationOptionsResponse::~GetDigitalInputConfigurationOptionsResponse()
{

}

}

}
