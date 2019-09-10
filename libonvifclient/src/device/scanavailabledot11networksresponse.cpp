// scanavailabledot11networksresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

ScanAvailableDot11NetworksResponse::ScanAvailableDot11NetworksResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken)
{

}

ScanAvailableDot11NetworksResponse::ScanAvailableDot11NetworksResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const std::vector<Dot11AvailableNetworks>& networks) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken),
  networks_(networks)
{

}

ScanAvailableDot11NetworksResponse::~ScanAvailableDot11NetworksResponse()
{

}

}

}
