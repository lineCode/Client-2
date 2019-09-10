// getserialportconfigurationoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetSerialPortConfigurationOptionsResponse::GetSerialPortConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken) :
  Response(client, localendpoint, latency, message),
  serialporttoken_(serialporttoken)
{

}

GetSerialPortConfigurationOptionsResponse::GetSerialPortConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& serialporttoken, const boost::optional<SerialPortConfigurationOptions>& serialportoptions) :
  Response(client, localendpoint, latency, message),
  serialporttoken_(serialporttoken),
  serialportoptions_(serialportoptions)
{

}

GetSerialPortConfigurationOptionsResponse::~GetSerialPortConfigurationOptionsResponse()
{

}

}

}
