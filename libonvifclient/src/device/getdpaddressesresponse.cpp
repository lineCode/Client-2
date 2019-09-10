// getdpaddressesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDPAddressesResponse::GetDPAddressesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDPAddressesResponse::GetDPAddressesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkHost>& dpaddress) :
  Response(client, localendpoint, latency, message),
  dpaddress_(dpaddress)
{

}

GetDPAddressesResponse::~GetDPAddressesResponse()
{

}

}

}
