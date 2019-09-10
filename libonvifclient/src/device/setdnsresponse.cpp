// setdnsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetDNSResponse::SetDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool fromdhcp, const std::vector<std::string>& searchdomain, const std::vector<IPAddress>& dnsmanual) :
  Response(client, localendpoint, latency, message),
  fromdhcp_(fromdhcp),
  searchdomain_(searchdomain),
  dnsmanual_(dnsmanual)
{

}

SetDNSResponse::~SetDNSResponse()
{

}

}

}
