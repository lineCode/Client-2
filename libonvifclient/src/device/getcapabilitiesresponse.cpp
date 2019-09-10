// getcapabilitiesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetCapabilitiesResponse::GetCapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, CAPABILITYCATEGORY capabilitycategory) :
  Response(client, localendpoint, latency, message),
  capabilitycategory_(capabilitycategory)
{

}

GetCapabilitiesResponse::GetCapabilitiesResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, CAPABILITYCATEGORY capabilitycategory, const boost::optional<Capabilities>& capabilities) :
  Response(client, localendpoint, latency, message),
  capabilitycategory_(capabilitycategory),
  capabilities_(capabilities)
{
  
}

GetCapabilitiesResponse::~GetCapabilitiesResponse()
{

}

}

}
