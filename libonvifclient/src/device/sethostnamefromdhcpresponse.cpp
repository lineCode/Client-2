// sethostnamefromdhcpresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetHostnameFromDHCPResponse::SetHostnameFromDHCPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp) :
  Response(client, localendpoint, latency, message),
  fromdhcp_(fromdhcp)
{

}

SetHostnameFromDHCPResponse::SetHostnameFromDHCPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp, const boost::optional<bool>& rebootneeded) :
  Response(client, localendpoint, latency, message),
  fromdhcp_(fromdhcp),
  rebootneeded_(rebootneeded)
{

}

SetHostnameFromDHCPResponse::~SetHostnameFromDHCPResponse()
{

}

}

}
