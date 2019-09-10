// getstorageconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetStorageConfigurationsResponse::GetStorageConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetStorageConfigurationsResponse::GetStorageConfigurationsResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<StorageConfiguration>& storageconfigurations) :
  Response(client, localendpoint, latency, message),
  storageconfigurations_(storageconfigurations)
{

}

GetStorageConfigurationsResponse::~GetStorageConfigurationsResponse()
{

}

}

}
