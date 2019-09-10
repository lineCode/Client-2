// getservicecapabilitiesresponse.cpp
//

///// Includes /////

#include "onvifclient/analyticsclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace analytics
{

///// Methods /////

GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(boost::shared_ptr<AnalyticsClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(boost::shared_ptr<AnalyticsClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities) :
  Response(client, localendpoint, latency, message),
  capabilities_(capabilities)
{

}

GetServiceCapabilitiesResponse::~GetServiceCapabilitiesResponse()
{

}

}

}
