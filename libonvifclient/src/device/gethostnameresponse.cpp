// gethostnameresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetHostnameResponse::GetHostnameResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetHostnameResponse::GetHostnameResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<HostnameInformation> hostnameinformation) :
  Response(client, localendpoint, latency, message),
  hostnameinformation_(hostnameinformation)
{

}

GetHostnameResponse::~GetHostnameResponse()
{

}

}

}
