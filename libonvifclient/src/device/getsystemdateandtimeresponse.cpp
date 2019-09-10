// getsystemdateandtimeresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetSystemDateAndTimeResponse::GetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetSystemDateAndTimeResponse::GetSystemDateAndTimeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<SystemDateTime> systemdatetime) :
  Response(client, localendpoint, latency, message),
  systemdatetime_(systemdatetime)
{

}

GetSystemDateAndTimeResponse::~GetSystemDateAndTimeResponse()
{

}

}

}
