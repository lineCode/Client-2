// loadcertificatewithprivatekeyresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

LoadCertificateWithPrivateKeyResponse::LoadCertificateWithPrivateKeyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<CertificateWithPrivateKey>& certificatewithprivatekey) :
  Response(client, localendpoint, latency, message),
  certificatewithprivatekey_(certificatewithprivatekey)
{

}

LoadCertificateWithPrivateKeyResponse::~LoadCertificateWithPrivateKeyResponse()
{

}

}

}
