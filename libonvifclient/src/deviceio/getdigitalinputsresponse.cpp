// getdigitalinputsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetDigitalInputsResponse::GetDigitalInputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetDigitalInputsResponse::GetDigitalInputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<DigitalInput>& digitalinputs) :
  Response(client, localendpoint, latency, message),
  digitalinputs_(digitalinputs)
{

}

GetDigitalInputsResponse::~GetDigitalInputsResponse()
{

}

}

}
