// setdynamicdnsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetDynamicDNSResponse::SetDynamicDNSResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, DYNAMICDNSTYPE type, const boost::optional<std::string>& name, const boost::optional<Duration>& ttl) :
  Response(client, localendpoint, latency, message),
  type_(type),
  name_(name),
  ttl_(ttl)
{

}

SetDynamicDNSResponse::~SetDynamicDNSResponse()
{

}

}

}
