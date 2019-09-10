// deletecertificatesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

DeleteCertificatesResponse::DeleteCertificatesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& certificateid) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid)
{

}

DeleteCertificatesResponse::~DeleteCertificatesResponse()
{

}

}

}
