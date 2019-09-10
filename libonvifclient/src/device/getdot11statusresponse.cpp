// getdot11statusresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDot11StatusResponse::GetDot11StatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken)
{

}

GetDot11StatusResponse::GetDot11StatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& interfacetoken, const boost::optional<Dot11Status>& status) :
  Response(client, localendpoint, latency, message),
  interfacetoken_(interfacetoken),
  status_(status)
{

}

GetDot11StatusResponse::~GetDot11StatusResponse()
{

}

}

}
