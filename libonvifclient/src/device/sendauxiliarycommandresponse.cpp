// sendauxiliarycommandresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace device
{

///// Methods /////

SendAuxiliaryCommandResponse::SendAuxiliaryCommandResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& auxiliarycommand) :
  Response(client, localendpoint, latency, message),
  auxiliarycommand_(auxiliarycommand)
{

}

SendAuxiliaryCommandResponse::SendAuxiliaryCommandResponse(boost::shared_ptr<DeviceClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& auxiliarycommand, const boost::optional<std::string>& auxiliarycommandresponse) :
  Response(client, localendpoint, latency, message),
  auxiliarycommand_(auxiliarycommand),
  auxiliarycommandresponse_(auxiliarycommandresponse)
{
  
}

SendAuxiliaryCommandResponse::~SendAuxiliaryCommandResponse()
{

}

}

}
