// setntpresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetNTPResponse::SetNTPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp, const std::vector<NetworkHost>& ntpmanual) :
  Response(client, localendpoint, latency, message),
  fromdhcp_(fromdhcp),
  ntpmanual_(ntpmanual)
{

}

SetNTPResponse::~SetNTPResponse()
{

}

}

}
