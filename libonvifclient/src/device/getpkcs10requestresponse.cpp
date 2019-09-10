// getpkcs10requestresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetPkcs10RequestResponse::GetPkcs10RequestResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid, const boost::optional<std::string> subject, const boost::optional<BinaryData>& attributes) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid),
  subject_(subject),
  attributes_(attributes)
{

}

GetPkcs10RequestResponse::GetPkcs10RequestResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& certificateid, const boost::optional<std::string> subject, const boost::optional<BinaryData>& attributes, const boost::optional<BinaryData>& pkcs10request) :
  Response(client, localendpoint, latency, message),
  certificateid_(certificateid),
  subject_(subject),
  attributes_(attributes),
  pkcs10request_(pkcs10request)
{

}

GetPkcs10RequestResponse::~GetPkcs10RequestResponse()
{

}

}

}
