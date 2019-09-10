// eventserverinterface.hpp
//

///// Includes /////

#include "onvifserver/eventserviceinterface.hpp"

#include "onvifserver/soapfault.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace event
{

///// Globals /////

const std::string EVENTSNAMESPACEURI("http://www.onvif.org/ver10/events/wsdl");

const std::string CREATEPULLPOINTSUBSCRIPTION("CreatePullPointSubscription");
const std::string GETEVENTPROPERTIES("GetEventProperties");
const std::string GETSERVICECAPABILITIES("GetServiceCapabilities");
const std::string PULLMESSAGES("PullMessages");
const std::string RENEW("Renew");
const std::string SETSYNCHRONIZATIONPOINT("SetSynchronizationPoint");
const std::string UNSUBSCRIBE("Unsubscribe");

///// Declarations /////

const std::string EventServiceInterface::XADDR_("/onvif/events");

///// Methods /////

EventServiceInterface::EventServiceInterface(const int onvifauthenticationtimediff) :
  OnvifService(2, 21, onvifauthenticationtimediff, { SOAP_NAMESPACE_ATTRIBUTE, XS_NAMESPACE_ATTRIBUTE, SCHEMA_INSTANCE_NAMESPACE_ATTRIBUTE, BASEFAULT_NAMESPACE_ATTRIBUTE, RESOURCEFAULT_NAMESPACE_ATTRIBUTE, ERROR_NAMESPACE_ATTRIBUTE, DS_NAMESPACE_ATTRIBUTE, WSSE_NAMESPACE_ATTRIBUTE, C14N_NAMESPACE_ATTRIBUTE, XOP_NAMESPACE_ATTRIBUTE, RPC_NAMESPACE_ATTRIBUTE, TOPICS_NAMESPACE_ATTRIBUTE, SCHEMA_NAMESPACE_ATTRIBUTE, TYPES_NAMESPACE_ATTRIBUTE, EVENT_NAMESPACE_ATTRIBUTE, ADDRESSING_NAMESPACE_ATTRIBUTE, WSNT_NAMESPACE_ATTRIBUTE, WSTOP_NAMESPACE_ATTRIBUTE })
{

}

EventServiceInterface::~EventServiceInterface()
{
  Destroy();

}

int EventServiceInterface::Init()
{
  Destroy();
  return 0;
}

void EventServiceInterface::Destroy()
{

}

boost::shared_future<ServerResponse> EventServiceInterface::OnvifRequest(const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& namespaceuri, const std::string& action, const pugi::xml_node& headernode, const pugi::xml_node& operationnode, const std::map< std::string, std::vector<char> >& mtomdata)
{
  if ((namespaceuri != EVENTSNAMESPACEURI) && (namespaceuri != WSNT_NAMESPACE))
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_NAMESPACE, std::string("Error parsing event namespace uri")).ToString()));
  }
  
  if (action == CREATEPULLPOINTSUBSCRIPTION)
  {
    const auto messageidnode = headernode.select_node("*[local-name()='MessageID']");
    boost::optional<std::string> messageid;
    if (messageidnode)
    {
      messageid = messageidnode.node().text().get();

    }

    boost::optional<Element> topicexpression;
    boost::optional<Element> messagefilter;
    const auto filternode = operationnode.select_node("*[local-name()='Filter']");
    if (filternode)
    {
      const auto topicexpressionnode = filternode.node().select_node("*[local-name()='TopicExpression']");
      if (!topicexpressionnode)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing TopicExpression parameter")).ToString()));
      }
      topicexpression = Element(topicexpressionnode.node());

      const auto messagecontentnode = filternode.node().select_node("*[local-name()='MessageContent']");
      if (messagecontentnode)
      {
        messagefilter = Element(messagecontentnode.node());

      }
    }

    const auto initialterminationtimenode = operationnode.select_node("*[local-name()='InitialTerminationTime']");
    boost::optional<std::string> initialterminationtime;
    if (initialterminationtimenode)
    {
      initialterminationtime = initialterminationtimenode.node().text().get();

    }

    const auto subsciptionpolicynode = operationnode.select_node("*[local-name()='SubscriptionPolicy']");
    boost::optional<std::string> subsciptionpolicy;
    if (subsciptionpolicynode)
    {
      subsciptionpolicy = subsciptionpolicynode.node().text().get();

    }

    return boost::make_ready_future(CreatePullPointSubscription(localaddress, port, remoteaddress, messageid, topicexpression, messagefilter, initialterminationtime, subsciptionpolicy));
  }
  else  if (action == GETEVENTPROPERTIES)
  {
    const auto messageidnode = headernode.select_node("*[local-name()='MessageID']");
    boost::optional<std::string> messageid;
    if (messageidnode)
    {
      messageid = messageidnode.node().text().get();

    }

    return boost::make_ready_future(GetEventProperties(messageid));
  }
  else if (action == GETSERVICECAPABILITIES)
  {

    return boost::make_ready_future(GetServiceCapabilities());
  }
  else if (action == PULLMESSAGES)
  {
    const auto subscriptionidnode = headernode.select_node("*[local-name()='SubscriptionId']");
    boost::optional<uint64_t> subscriptionid;
    if (subscriptionidnode)
    {
      try
      {
        subscriptionid = boost::lexical_cast<uint64_t>(subscriptionidnode.node().text().get());

      }
      catch (...)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid SubscriptionId parameter")).ToString()));
      }
    }

    const auto messageidnode = headernode.select_node("*[local-name()='MessageID']");
    boost::optional<std::string> messageid;
    if (messageidnode)
    {
      messageid = messageidnode.node().text().get();

    }

    const auto timeoutnode = operationnode.select_node("*[local-name()='Timeout']");
    boost::optional<onvif::Duration> timeout;
    if (timeoutnode)
    {
      timeout = onvif::GetDuration(timeoutnode.node().text().get());
      if (!timeout.is_initialized())
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing Timeout parameter")).ToString()));
      }
    }

    const auto messagelimitnode = operationnode.select_node("*[local-name()='MessageLimit']");
    if (!messagelimitnode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing MessageLimit parameter")).ToString()));
    }

    return PullMessages(remoteaddress, subscriptionid, messageid, *timeout, messagelimitnode.node().text().as_int());
  }
  else if (action == RENEW)
  {
    const auto subscriptionidnode = headernode.select_node("*[local-name()='SubscriptionId']");
    boost::optional<uint64_t> subscriptionid;
    if (subscriptionidnode)
    {
      try
      {
        subscriptionid = boost::lexical_cast<uint64_t>(subscriptionidnode.node().text().get());

      }
      catch (...)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid SubscriptionId parameter")).ToString()));
      }
    }

    const auto messageidnode = headernode.select_node("*[local-name()='MessageID']");
    boost::optional<std::string> messageid;
    if (messageidnode)
    {
      messageid = messageidnode.node().text().get();

    }

    const auto terminationtimenode = operationnode.select_node("*[local-name()='TerminationTime']");
    if (!terminationtimenode)
    {

      return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Missing TerminationTime parameter")).ToString()));
    }

    return boost::make_ready_future(Renew(remoteaddress, subscriptionid, messageid, terminationtimenode.node().text().get()));
  }
  else if (action == SETSYNCHRONIZATIONPOINT)
  {
    const auto subscriptionidnode = headernode.select_node("*[local-name()='SubscriptionId']");
    boost::optional<uint64_t> subscriptionid;
    if (subscriptionidnode)
    {
      try
      {
        subscriptionid = boost::lexical_cast<uint64_t>(subscriptionidnode.node().text().get());

      }
      catch (...)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid SubscriptionId parameter")).ToString()));
      }
    }

    const auto messageidnode = headernode.select_node("*[local-name()='MessageID']");
    boost::optional<std::string> messageid;
    if (messageidnode)
    {
      messageid = messageidnode.node().text().get();

    }

    return boost::make_ready_future(SetSynchronizationPoint(remoteaddress, subscriptionid, messageid));
  }
  else if (action == UNSUBSCRIBE)
  {
    const auto subscriptionidnode = headernode.select_node("*[local-name()='SubscriptionId']");
    boost::optional<uint64_t> subscriptionid;
    if (subscriptionidnode)
    {
      try
      {
        subscriptionid = boost::lexical_cast<uint64_t>(subscriptionidnode.node().text().get());

      }
      catch (...)
      {

        return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_INVALIDARGS, std::string("Invalid SubscriptionId parameter")).ToString()));
      }
    }

    const auto messageidnode = headernode.select_node("*[local-name()='MessageID']");
    boost::optional<std::string> messageid;
    if (messageidnode)
    {
      messageid = messageidnode.node().text().get();

    }

    return boost::make_ready_future(Unsubscribe(remoteaddress, subscriptionid, messageid));
  }
  else
  {

    return boost::make_ready_future(ServerResponse(HTTPSTATUSCODE::BADREQUEST, { CONTENT_TYPE }, SoapFault(FAULT_UNKNOWNACTION, std::string("Unknown Action: ") + action).ToString()));
  }
}

}

}

}
