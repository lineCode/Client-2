// createstorageconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

CreateStorageConfigurationResponse::CreateStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StorageConfigurationData& storageconfiguration) :
  Response(client, localendpoint, latency, message),
  storageconfiguration_(storageconfiguration)
{

}

CreateStorageConfigurationResponse::CreateStorageConfigurationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StorageConfigurationData& storageconfiguration, const boost::optional<std::string>& token) :
  Response(client, localendpoint, latency, message),
  storageconfiguration_(storageconfiguration),
  token_(token)
{

}

CreateStorageConfigurationResponse::~CreateStorageConfigurationResponse()
{

}

}

}
