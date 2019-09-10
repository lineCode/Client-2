// setcertificatesstatusresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetCertificatesStatusResponse::SetCertificatesStatusResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<CertificateStatus>& certificatestatus) :
  Response(client, localendpoint, latency, message),
  certificatestatus_(certificatestatus)
{

}

SetCertificatesStatusResponse::~SetCertificatesStatusResponse()
{

}

}

}
