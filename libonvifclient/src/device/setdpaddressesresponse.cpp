// setdpaddressesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetDPAddressesResponse::SetDPAddressesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<NetworkHost>& dpaddress) :
  Response(client, localendpoint, latency, message),
  dpaddress_(dpaddress)
{

}

SetDPAddressesResponse::~SetDPAddressesResponse()
{

}

}

}
