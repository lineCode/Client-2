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

GetCertificatesStatusResponse::GetCertificatesStatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{
  
}

GetCertificatesStatusResponse::GetCertificatesStatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<CertificateStatus>& certificatesstatus) :
  Response(client, localendpoint, latency, message),
  certificatesstatus_(certificatesstatus)
{

}

GetCertificatesStatusResponse::~GetCertificatesStatusResponse()
{

}

}

}
