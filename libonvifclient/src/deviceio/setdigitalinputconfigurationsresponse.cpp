// setdigitalinputconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

SetDigitalInputConfigurationsResponse::SetDigitalInputConfigurationsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<DigitalInput>& digitalinputs) :
  Response(client, localendpoint, latency, message),
  digitalinputs_(digitalinputs)
{

}

SetDigitalInputConfigurationsResponse::~SetDigitalInputConfigurationsResponse()
{

}

}

}
