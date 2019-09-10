// loadcacertificatesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

LoadCACertificatesResponse::LoadCACertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Certificate>& cacertificate) :
  Response(client, localendpoint, latency, message),
  cacertificate_(cacertificate)
{

}

LoadCACertificatesResponse::~LoadCACertificatesResponse()
{

}

}

}
