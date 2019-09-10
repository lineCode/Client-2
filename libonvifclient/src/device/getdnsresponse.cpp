// getdnsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDNSResponse::GetDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDNSResponse::GetDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DNSInformation> dnsinformation) :
  Response(client, localendpoint, latency, message),
  dnsinformation_(dnsinformation)
{

}

GetDNSResponse::~GetDNSResponse()
{

}

}

}
