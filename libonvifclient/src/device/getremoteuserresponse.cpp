// getremoteuserresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetRemoteUserResponse::GetRemoteUserResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetRemoteUserResponse::GetRemoteUserResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, boost::optional<RemoteUser> remoteuser) :
  Response(client, localendpoint, latency, message),
  remoteuser_(remoteuser)
{

}

GetRemoteUserResponse::~GetRemoteUserResponse()
{

}

}

}
