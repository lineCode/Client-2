// getdynamicdnsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDynamicDNSResponse::GetDynamicDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDynamicDNSResponse::GetDynamicDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<DynamicDNSInformation> dynamicdnsinformation) :
  Response(client, localendpoint, latency, message),
  dynamicdnsinformation_(dynamicdnsinformation)
{

}

GetDynamicDNSResponse::~GetDynamicDNSResponse()
{

}

}

}
