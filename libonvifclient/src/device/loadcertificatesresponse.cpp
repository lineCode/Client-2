// loadcertificatesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

LoadCertificatesResponse::LoadCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& nvtcertificate) :
  Response(client, localendpoint, latency, message),
  nvtcertificate_(nvtcertificate)
{

}

LoadCertificatesResponse::~LoadCertificatesResponse()
{

}

}

}
