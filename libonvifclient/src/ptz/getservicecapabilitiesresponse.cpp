// getservicecapabilitiesresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities) :
  Response(client, localendpoint, latency, message),
  capabilities_(capabilities)
{

}

GetServiceCapabilitiesResponse::~GetServiceCapabilitiesResponse()
{

}

}

}
