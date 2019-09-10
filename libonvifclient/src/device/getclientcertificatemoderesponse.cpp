// getclientcertificatemoderesponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetClientCertificateModeResponse::GetClientCertificateModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetClientCertificateModeResponse::GetClientCertificateModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<bool> enabled) :
  Response(client, localendpoint, latency, message),
  enabled_(enabled)
{

}

GetClientCertificateModeResponse::~GetClientCertificateModeResponse()
{

}

}

}
