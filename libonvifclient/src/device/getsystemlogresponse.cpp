// getsystemlogresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetSystemLogResponse::GetSystemLogResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, SYSTEMLOGTYPE logtype) :
  Response(client, localendpoint, latency, message),
  LOGLEVEL_(logtype)
{

}

GetSystemLogResponse::GetSystemLogResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, SYSTEMLOGTYPE logtype, const boost::optional<SystemLog>& systemlog) :
  Response(client, localendpoint, latency, message),
  LOGLEVEL_(logtype),
  systemlog_(systemlog)
{

}

GetSystemLogResponse::~GetSystemLogResponse()
{

}

}

}
