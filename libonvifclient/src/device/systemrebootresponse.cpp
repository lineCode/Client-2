// systemrebootresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SystemRebootResponse::SystemRebootResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

SystemRebootResponse::SystemRebootResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& rebootmessage) :
  Response(client, localendpoint, latency, message),
  rebootmessage_(rebootmessage)
{

}

SystemRebootResponse::~SystemRebootResponse()
{

}

}

}
