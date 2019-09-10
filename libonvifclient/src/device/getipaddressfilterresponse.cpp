// getipaddressfilterresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetIPAddressFilterResponse::GetIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetIPAddressFilterResponse::GetIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<IPAddressFilter> ipaddressfilter) :
  Response(client, localendpoint, latency, message),
  ipaddressfilter_(ipaddressfilter)
{

}

GetIPAddressFilterResponse::~GetIPAddressFilterResponse()
{

}

}

}
