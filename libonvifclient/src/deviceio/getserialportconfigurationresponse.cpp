// getserialportconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetSerialPortConfigurationResponse::GetSerialPortConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken) :
  Response(client, localendpoint, latency, message),
  serialporttoken_(serialporttoken)
{

}

GetSerialPortConfigurationResponse::GetSerialPortConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken, const boost::optional<SerialPortConfiguration>& serialportconfiguration) :
  Response(client, localendpoint, latency, message),
  serialporttoken_(serialporttoken),
  serialportconfiguration_(serialportconfiguration)
{

}

GetSerialPortConfigurationResponse::~GetSerialPortConfigurationResponse()
{

}

}

}
