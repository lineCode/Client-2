// getsystembackupresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetSystemBackupResponse::GetSystemBackupResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetSystemBackupResponse::GetSystemBackupResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<BackupFile>& backupfiles) :
  Response(client, localendpoint, latency, message),
  backupfiles_(backupfiles)
{

}

GetSystemBackupResponse::~GetSystemBackupResponse()
{

}

}

}
