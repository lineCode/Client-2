// createcertificateresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

CreateCertificateResponse::CreateCertificateResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid),
  subject_(subject),
  validnotbefore_(validnotbefore),
  validnotafter_(validnotafter)
{

}

CreateCertificateResponse::CreateCertificateResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& certificateid, const boost::optional<std::string>& subject, const boost::optional<onvif::ws::DateTime>& validnotbefore, const boost::optional<onvif::ws::DateTime>& validnotafter, const boost::optional<Certificate>& nvtcertificate) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid),
  subject_(subject),
  validnotbefore_(validnotbefore),
  validnotafter_(validnotafter),
  nvtcertificate_(nvtcertificate)
{

}

CreateCertificateResponse::~CreateCertificateResponse()
{

}

}

}
