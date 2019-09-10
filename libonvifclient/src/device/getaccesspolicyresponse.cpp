// getaccesspolicyresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetAccessPolicyResponse::GetAccessPolicyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAccessPolicyResponse::GetAccessPolicyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<BinaryData> policyfile) :
  Response(client, localendpoint, latency, message),
  policyfile_(policyfile)
{

}

GetAccessPolicyResponse::~GetAccessPolicyResponse()
{

}

}

}
