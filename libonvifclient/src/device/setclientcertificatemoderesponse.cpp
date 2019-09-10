// setclientcertificatemoderesponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetClientCertificateModeResponse::SetClientCertificateModeResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, bool enabled) :
  Response(client, localendpoint, latency, message),
  enabled_(enabled)
{

}

SetClientCertificateModeResponse::~SetClientCertificateModeResponse()
{

}

}

}
