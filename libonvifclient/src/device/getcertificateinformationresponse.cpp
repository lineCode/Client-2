// getcertificateinformationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetCertificateInformationResponse::GetCertificateInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid)
{

}

GetCertificateInformationResponse::GetCertificateInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid, const boost::optional<CertificateInformation>& certificateinformation) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid),
  certificateinformation_(certificateinformation)
{

}

GetCertificateInformationResponse::~GetCertificateInformationResponse()
{

}

}

}
