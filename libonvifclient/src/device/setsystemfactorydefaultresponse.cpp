// setsystemfactorydefaultresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetSystemFactoryDefaultResponse::SetSystemFactoryDefaultResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

SetSystemFactoryDefaultResponse::SetSystemFactoryDefaultResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, FACTORYDEFAULTTYPE factorydefault) :
  Response(client, localendpoint, latency, message),
  factorydefault_(factorydefault)
{

}

SetSystemFactoryDefaultResponse::~SetSystemFactoryDefaultResponse()
{

}

}

}
