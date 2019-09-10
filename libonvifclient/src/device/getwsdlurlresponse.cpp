// getwsdlurlresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetWsdlUrlResponse::GetWsdlUrlResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetWsdlUrlResponse::GetWsdlUrlResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& wsdlurl) :
  Response(client, localendpoint, latency, message),
  wsdlurl_(wsdlurl)
{

}

GetWsdlUrlResponse::~GetWsdlUrlResponse()
{

}

}

}
