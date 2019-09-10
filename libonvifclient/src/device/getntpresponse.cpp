// getntpresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetNTPResponse::GetNTPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetNTPResponse::GetNTPResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<NTPInformation> ntpinformation) :
  Response(client, localendpoint, latency, message),
  ntpinformation_(ntpinformation)
{

}

GetNTPResponse::~GetNTPResponse()
{

}

}

}
