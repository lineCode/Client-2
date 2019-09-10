// receiverserviceinterface.hpp
//

///// Includes /////

#include "onvifserver/receiverserviceinterface.hpp"

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace receiver
{

///// Globals /////

const std::string RECEIVERNAMESPACEURI("http://www.onvif.org/ver10/receiver/wsdl");

const std::string CONFIGURERECEIVER("ConfigureReceiver");
const std::string CREATERECEIVER("CreateReceiver");
const std::string DELETERECEIVER("DeleteReceiver");
const std::string GETRECEIVER("GetReceiver");
const std::string GETRECEIVERS("GetReceivers");
const std::string GETRECEIVERSTATE("GetReceiverState");
const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");
const std::string SETRECEIVERMODE("SetReceiverMode");

///// Declarations /////

const std::string ReceiverServiceInterface::XADDR_("/onvif/receiver");

///// Methods /////

ReceiverServiceInterface::ReceiverServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 11, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE, RECEIVER_NAMESPACE_ATTRIBUTE })
{

}

ReceiverServiceInterface::~ReceiverServiceInterface()
{
  Destroy();

}

int ReceiverServiceInterface::Init()
{
  Destroy();
  return 0;
}

void ReceiverServiceInterface::Destroy()
{

}

boost::shared_future<ServerResponse> ReceiverServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (namespaceuri != RECEIVERNAMESPACEURI)
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing receiver namespace uri")).ToString()));
  }

  if (action == CONFIGURERECEIVER)
  {
    const auto receivertokennode = operationnode.select_node("*[local-name()='ReceiverToken']");
    if (!receivertokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("Invalid ReceiverToken")).ToString()));
    }

    uint64_t receivertoken = 0;
    try
    {
      receivertoken = boost::lexical_cast<uint64_t>(receivertokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));
    }

    const auto configurationnode = operationnode.select_node("*[local-name()='Configuration']");
    if (!configurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Configuration argument")).ToString()));
    }

    return boost::make_ready_future(ConfigureReceiver(receivertoken, onvif::receiver::ReceiverConfiguration(configurationnode.node())));
  }
  else if (action == CREATERECEIVER)
  {
    const auto configurationnode = operationnode.select_node("*[local-name()='Configuration']");
    if (!configurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Configuration argument")).ToString()));
    }

    return boost::make_ready_future(CreateReceiver(onvif::receiver::ReceiverConfiguration(configurationnode.node())));
  }
  if (action == DELETERECEIVER)
  {
    const auto receivertokennode = operationnode.select_node("*[local-name()='ReceiverToken']");
    if (!receivertokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("Invalid ReceiverToken")).ToString()));
    }

    uint64_t receivertoken = 0;
    try
    {
      receivertoken = boost::lexical_cast<uint64_t>(receivertokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));
    }

    return boost::make_ready_future(DeleteReceiver(receivertoken));
  }
  else if (action == GETRECEIVER)
  {
    const auto receivertokennode = operationnode.select_node("*[local-name()='ReceiverToken']");
    if (!receivertokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("Invalid ReceiverToken")).ToString()));
    }

    uint64_t receivertoken = 0;
    try
    {
      receivertoken = boost::lexical_cast<uint64_t>(receivertokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {
    
      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));  
    }

    return boost::make_ready_future(GetReceiver(receivertoken));
  }
  else if (action == GETRECEIVERS)
  {

    return boost::make_ready_future(GetReceivers());
  }
  if (action == GETRECEIVERSTATE)
  {
    const auto receivertokennode = operationnode.select_node("*[local-name()='ReceiverToken']");
    if (!receivertokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));
    }

    uint64_t receivertoken = 0;
    try
    {
      receivertoken = boost::lexical_cast<uint64_t>(receivertokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));
    }

    return boost::make_ready_future(GetReceiverState(receivertoken));
  }
  else if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else if (action == SETRECEIVERMODE)
  {
    const auto receivertokennode = operationnode.select_node("*[local-name()='ReceiverToken']");
    if (!receivertokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));
    }

    const auto modenode = operationnode.select_node("*[local-name()='Mode']");
    if (!modenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Mode argument")).ToString()));
    }

    const boost::optional<onvif::receiver::RECEIVERMODE> mode = onvif::receiver::GetReceiverMode(modenode.node().text().get());
    if (!mode.is_initialized())
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid Mode argument")).ToString()));
    }

    uint64_t receivertoken = 0;
    try
    {
      receivertoken = boost::lexical_cast<uint64_t>(receivertokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNTOKEN, std::string("The receiver indicated by ReceiverToken does not exist.")).ToString()));
    }

    return boost::make_ready_future(SetReceiverMode(receivertoken, *mode));
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

}

}

}
