// sethostnameresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SetHostnameResponse::SetHostnameResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& name) :
  Response(client, localendpoint, latency, message),
  name_(name)
{

}

SetHostnameResponse::~SetHostnameResponse()
{

}

}

}
