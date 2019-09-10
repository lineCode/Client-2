// getcertificatesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetCertificatesResponse::GetCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetCertificatesResponse::GetCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& certificates) :
  Response(client, localendpoint, latency, message),
  certificates_(certificates)
{

}

GetCertificatesResponse::~GetCertificatesResponse()
{

}

}

}
