// getdeviceinformationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

GetDeviceInformationResponse::GetDeviceInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDeviceInformationResponse::GetDeviceInformationResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& manufacturer, const boost::optional<std::string>& model, const boost::optional<std::string>& firmwareversion, const boost::optional<std::string>& serialnumber, const boost::optional<std::string>& hardwareid) :
  Response(client, localendpoint, latency, message),
  manufacturer_(manufacturer),
  model_(model),
  firmwareversion_(firmwareversion),
  serialnumber_(serialnumber),
  hardwareid_(hardwareid)
{

}

GetDeviceInformationResponse::~GetDeviceInformationResponse()
{

}

}

}
