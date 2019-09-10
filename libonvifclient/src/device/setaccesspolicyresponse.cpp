// setaccesspolicyresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetAccessPolicyResponse::SetAccessPolicyResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<BinaryData>& policyfile) :
  Response(client, localendpoint, latency, message),
  policyfile_(policyfile)
{

}

SetAccessPolicyResponse::~SetAccessPolicyResponse()
{

}

}

}
