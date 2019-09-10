// getsystemurisresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetSystemUrisResponse::GetSystemUrisResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetSystemUrisResponse::GetSystemUrisResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<SystemLogUriList>& systemloguris, const boost::optional<std::string>& supportinfouri, const boost::optional<std::string>& systembackupuri) :
  Response(client, localendpoint, latency, message),
  systemloguris_(systemloguris),
  supportinfouri_(supportinfouri),
  systembackupuri_(systembackupuri)
{

}

GetSystemUrisResponse::~GetSystemUrisResponse()
{

}

}

}
