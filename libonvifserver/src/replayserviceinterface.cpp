// replayserviceinterface.hpp
//

///// Includes /////

#include "onvifserver/replayserviceinterface.hpp"

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace replay
{

///// Globals /////

const std::string REPLAYNAMESPACEURI("http://www.onvif.org/ver10/replay/wsdl");

const std::string GETREPLAYCONFIGURATION("GetReplayConfiguration");
const std::string GETREPLAYURI("GetReplayUri");
const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");
const std::string SETREPLAYCONFIGURATION("SetReplayConfiguration");

///// Declarations /////

const std::string ReplayServiceInterface::XADDR_("/onvif/replay");

///// Methods /////

ReplayServiceInterface::ReplayServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 21, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE, REPLAY_NAMESPACE_ATTRIBUTE })
{

}

ReplayServiceInterface::~ReplayServiceInterface()
{
  Destroy();

}

int ReplayServiceInterface::Init()
{
  Destroy();
  return 0;
}

void ReplayServiceInterface::Destroy()
{

}

boost::shared_future<ServerResponse> ReplayServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if (namespaceuri != REPLAYNAMESPACEURI)
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing device namespace uri")).ToString()));
  }

  else if (action == GETREPLAYCONFIGURATION)
  {

    return boost::make_ready_future(GetReplayConfiguration());
  }
  else if (action == GETREPLAYURI)
  {
    const auto streamsetupnode = operationnode.select_node("*[local-name()='StreamSetup']");
    if (!streamsetupnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing StreamSetup argument")).ToString()));
    }

    const auto recordingtokennode = operationnode.select_node("*[local-name()='RecordingToken']");
    if (!recordingtokennode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing RecordingToken")).ToString()));
    }

    uint64_t recordingtoken = 0;
    try
    {
      recordingtoken = boost::lexical_cast<uint64_t>(recordingtokennode.node().text().get());

    }
    catch (const boost::bad_lexical_cast&)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NORECORDING, std::string("The RecordingToken does not reference an existing recording")).ToString()));
    }

    return boost::make_ready_future(GetReplayUri(localaddress, onvif::StreamSetup(streamsetupnode.node()), recordingtoken));
  }
  else if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else if (action == SETREPLAYCONFIGURATION)
  {
    const auto configurationnode = operationnode.select_node("*[local-name()='Configuration']");
    if (!configurationnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Configuration")).ToString()));
    }

    return boost::make_ready_future(SetReplayConfiguration(onvif::replay::ReplayConfiguration(configurationnode.node())));
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

}

}

}
