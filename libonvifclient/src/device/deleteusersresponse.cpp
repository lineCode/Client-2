// deleteusersresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

DeleteUsersResponse::DeleteUsersResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& usernames) :
  Response(client, localendpoint, latency, message),
  usernames_(usernames)
{

}

DeleteUsersResponse::~DeleteUsersResponse()
{

}

}

}
