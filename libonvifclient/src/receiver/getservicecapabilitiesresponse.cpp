// getservicecapabilitiesresponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////

GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetServiceCapabilitiesResponse::GetServiceCapabilitiesResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities) :
  Response(client, localendpoint, latency, message),
  capabilities_(capabilities)
{
  
}

GetServiceCapabilitiesResponse::~GetServiceCapabilitiesResponse()
{
  
}

}

}
