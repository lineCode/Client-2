// getserialportsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetSerialPortsResponse::GetSerialPortsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetSerialPortsResponse::GetSerialPortsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<SerialPort>& serialport) :
  Response(client, localendpoint, latency, message),
  serialport_(serialport)
{

}

GetSerialPortsResponse::~GetSerialPortsResponse()
{

}

}

}
