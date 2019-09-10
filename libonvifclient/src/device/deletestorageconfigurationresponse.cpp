// deletestorageconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

DeleteStorageConfigurationResponse::DeleteStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

DeleteStorageConfigurationResponse::~DeleteStorageConfigurationResponse()
{

}

}

}
