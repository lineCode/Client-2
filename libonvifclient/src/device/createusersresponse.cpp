// createusersresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

CreateUsersResponse::CreateUsersResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<User>& users) :
  Response(client, localendpoint, latency, message),
  users_(users)
{

}

CreateUsersResponse::~CreateUsersResponse()
{

}

}

}
