// setipaddressfilterresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetIPAddressFilterResponse::SetIPAddressFilterResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const IPAddressFilter& ipaddressfilter) :
  Response(client, localendpoint, latency, message),
  ipaddressfilter_(ipaddressfilter)
{

}

SetIPAddressFilterResponse::~SetIPAddressFilterResponse()
{

}

}

}
