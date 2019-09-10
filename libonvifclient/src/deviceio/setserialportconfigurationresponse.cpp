// setserialportconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

SetSerialPortConfigurationResponse::SetSerialPortConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const SerialPortConfiguration& serialportconfiguration, bool forcepersistence) :
  Response(client, localendpoint, latency, message),
  serialportconfiguration_(serialportconfiguration),
  forcepersistence_(forcepersistence)
{

}

SetSerialPortConfigurationResponse::~SetSerialPortConfigurationResponse()
{

}

}

}
