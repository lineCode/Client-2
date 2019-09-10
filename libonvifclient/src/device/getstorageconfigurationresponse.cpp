// getstorageconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetStorageConfigurationResponse::GetStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetStorageConfigurationResponse::GetStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<StorageConfiguration>& storageconfiguration) :
  Response(client, localendpoint, latency, message),
  token_(token),
  storageconfiguration_(storageconfiguration)
{

}

GetStorageConfigurationResponse::~GetStorageConfigurationResponse()
{

}

}

}
