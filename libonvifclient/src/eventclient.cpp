// eventclient.cpp
//

///// Includes /////

#include "onvifclient/eventclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace event
{

///// Classes /////

class EventSignals
{
 public:

  Signal< EVENTOPERATION, EventClient, CreatePullPointSubscriptionResponse, boost::optional<std::string>, boost::optional<std::string>, boost::optional<std::string> > createpullpointsubscription_;
  Signal<EVENTOPERATION, EventClient, GetEventPropertiesResponse> geteventproperties_;
  Signal<EVENTOPERATION, EventClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal<EVENTOPERATION, EventClient, PullMessagesResponse, onvif::Duration, int> pullmessages_;
};

///// Methods /////

EventClient::EventClient() :
  signals_(new EventSignals(
  {
    Signal< EVENTOPERATION, EventClient, CreatePullPointSubscriptionResponse, boost::optional<std::string>, boost::optional<std::string>, boost::optional<std::string> >(this, EVENTOPERATION_CREATEPULLPOINTSUBSCRIPTION, true, std::string("http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionRequest"), true),
    Signal<EVENTOPERATION, EventClient, GetEventPropertiesResponse>(this, EVENTOPERATION_GETEVENTPROPERTIES, true, std::string("http://www.onvif.org/ver10/events/wsdl/EventPortType/GetEventPropertiesRequest"), true),
    Signal<EVENTOPERATION, EventClient, GetServiceCapabilitiesResponse>(this, EVENTOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/events/wsdl/EventPortType/GetServiceCapabilitiesRequest"), true),
    Signal<EVENTOPERATION, EventClient, PullMessagesResponse, onvif::Duration, int>(this, EVENTOPERATION_PULLMESSAGES, true, std::string("http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesRequest"), true)
  }))
{

}

EventClient::~EventClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void EventClient::Destroy()
{
  Client::Destroy();

  signals_->createpullpointsubscription_.Destroy();
  signals_->geteventproperties_.Destroy();
  signals_->getservicecapabilities_.Destroy();
  signals_->pullmessages_.Destroy();
}

// Requests
void EventClient::CreatePullPointSubscription(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy)
{
  signals_->createpullpointsubscription_.Create(CreatePullPointSubscriptionBody(filter, initialterminationtime, subscriptionpolicy), filter, initialterminationtime, subscriptionpolicy);
}

void EventClient::GetEventProperties()
{
  signals_->geteventproperties_.Create(GetEventPropertiesBody());
}

void EventClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void EventClient::PullMessages(const std::vector<Element>& referenceparameters, const std::string& to, const onvif::Duration& timeout, const int messagelimit)
{
  signals_->pullmessages_.Create(referenceparameters, to, PullMessagesBody(timeout, messagelimit), std::map< std::string, std::vector<char> >(), timeout, messagelimit);
}

// Callbacks
Connection EventClient::CreatePullPointSubscriptionCallback(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy, boost::function<void(const CreatePullPointSubscriptionResponse&)> callback)
{
  return signals_->createpullpointsubscription_.CreateCallback(CreatePullPointSubscriptionBody(filter, initialterminationtime, subscriptionpolicy), callback, filter, initialterminationtime, subscriptionpolicy);
}

Connection EventClient::GetEventPropertiesCallback(boost::function<void(const GetEventPropertiesResponse&)> callback)
{
  return signals_->geteventproperties_.CreateCallback(GetEventPropertiesBody(), callback);
}

Connection EventClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection EventClient::PullMessagesCallback(const std::vector<Element>& referenceparameters, const std::string& to, const onvif::Duration& timeout, const int messagelimit, boost::function<void(const PullMessagesResponse&)> callback)
{
  return signals_->pullmessages_.CreateCallback(referenceparameters, to, PullMessagesBody(timeout, messagelimit), std::map< std::string, std::vector<char> >(), callback, timeout, messagelimit);
}

// Futures
boost::unique_future<CreatePullPointSubscriptionResponse> EventClient::CreatePullPointSubscriptionFuture(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy)
{
  return signals_->createpullpointsubscription_.CreateFuture(CreatePullPointSubscriptionBody(filter, initialterminationtime, subscriptionpolicy), filter, initialterminationtime, subscriptionpolicy);
}

boost::unique_future<GetEventPropertiesResponse> EventClient::GetEventPropertiesFuture()
{
  return signals_->geteventproperties_.CreateFuture(GetEventPropertiesBody());
}

boost::unique_future<GetServiceCapabilitiesResponse> EventClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<PullMessagesResponse> EventClient::PullMessagesFuture(const std::vector<Element>& referenceparameters, const std::string& to, const onvif::Duration& timeout, const int messagelimit)
{
  return signals_->pullmessages_.CreateFuture(referenceparameters, to, PullMessagesBody(timeout, messagelimit), std::map< std::string, std::vector<char> >(), timeout, messagelimit);
}

// Signals
boost::signals2::signal<void(const CreatePullPointSubscriptionResponse&)>& EventClient::CreatePullPointSubscriptionSignal()
{
  return signals_->createpullpointsubscription_.GetSignal();
}

boost::signals2::signal<void(const GetEventPropertiesResponse&)>& EventClient::GetEventPropertiesSignal()
{
  return signals_->geteventproperties_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& EventClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const PullMessagesResponse&)>& EventClient::PullMessagesSignal()
{
  return signals_->pullmessages_.GetSignal();
}

void EventClient::Update(EVENTOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case EVENTOPERATION_CREATEPULLPOINTSUBSCRIPTION:
    {
      auto createapullpointsubscriptionresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreatePullPointSubscriptionResponse']");
      if (!createapullpointsubscriptionresponse)
      {
        SignalError(EVENTOPERATION_CREATEPULLPOINTSUBSCRIPTION, handle, localendpoint, latency, std::string("/Envelope/Body/CreatePullPointSubscriptionResponse element not found"));
        break;
      }

      signals_->createpullpointsubscription_.Emit(handle, localendpoint, latency, std::string(), GetClass<ws::EndpointReferenceType>(createapullpointsubscriptionresponse, "*[local-name()='SubscriptionReference']"), GetText(createapullpointsubscriptionresponse, "*[local-name()='CurrentTime']"), GetText(createapullpointsubscriptionresponse, "*[local-name()='TerminationTime']"));
      break;
    }
    case EVENTOPERATION_GETEVENTPROPERTIES:
    {
      auto geteventpropertiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetEventPropertiesResponse']");
      if (!geteventpropertiesresponse)
      {
        SignalError(EVENTOPERATION_GETEVENTPROPERTIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetEventPropertiesResponse element not found"));
        break;
      }

      std::vector<std::string> topicnamespacelocations;
      for (const auto& topicnamespacelocation : geteventpropertiesresponse.node().select_nodes("*[local-name()='TopicNamespaceLocation']"))
      {
        topicnamespacelocations.push_back(topicnamespacelocation.node().text().as_string());

      }

      std::vector<std::string> messagecontentfilterdialects;
      for (const auto& messagecontentfilterdialect : geteventpropertiesresponse.node().select_nodes("*[local-name()='MessageContentFilterDialect']"))
      {
        messagecontentfilterdialects.push_back(messagecontentfilterdialect.node().text().as_string());

      }

      std::vector<std::string> topicexpressiondialects;
      for (const auto& topicexpressiondialect : geteventpropertiesresponse.node().select_nodes("*[local-name()='TopicExpressionDialect']"))
      {
        topicexpressiondialects.push_back(topicexpressiondialect.node().text().as_string());

      }

      std::vector<std::string> producerpropertiesfilterdialects;
      for (const auto& producerpropertiesfilterdialect : geteventpropertiesresponse.node().select_nodes("*[local-name()='ProducerPropertiesFilterDialect']"))
      {
        producerpropertiesfilterdialects.push_back(producerpropertiesfilterdialect.node().text().as_string());

      }

      std::vector<std::string> messagecontentschemalocations;
      for (const auto& messagecontentschemalocation : geteventpropertiesresponse.node().select_nodes("*[local-name()='MessageContentSchemaLocation']"))
      {
        messagecontentschemalocations.push_back(messagecontentschemalocation.node().text().as_string());

      }

      signals_->geteventproperties_.Emit(handle, localendpoint, latency, std::string(), topicnamespacelocations, GetBool(geteventpropertiesresponse, "*[local-name()='FixedTopicSet']"), GetClass<ws::TopicSet>(geteventpropertiesresponse, "*[local-name()='TopicSet']"), topicexpressiondialects, messagecontentfilterdialects, producerpropertiesfilterdialects, messagecontentschemalocations);
      break;
    }
    case EVENTOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(EVENTOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }

      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case EVENTOPERATION_PULLMESSAGES:
    {
      auto pullmessagesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='PullMessagesResponse']");
      if (!pullmessagesresponse)
      {
        SignalError(EVENTOPERATION_PULLMESSAGES, handle, localendpoint, latency, std::string("/Envelope/Body/PullMessagesResponse element not found"));
        break;
      }

      std::vector<Element> notificationmessages;
      for (const auto& notificationmessage : pullmessagesresponse.node().select_nodes("*[local-name()='NotificationMessage']"))
      {
        notificationmessages.push_back(Element(notificationmessage.node()));

      }

      signals_->pullmessages_.Emit(handle, localendpoint, latency, std::string(), GetClass<onvif::DateTime>(pullmessagesresponse, "*[local-name()='CurrentTime']"), GetClass<onvif::DateTime>(pullmessagesresponse, "*[local-name()='TerminationTime']"), notificationmessages);
      break;
    }
    default:
    {

      break;
    }
  }
}

void EventClient::SignalError(EVENTOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case EVENTOPERATION_CREATEPULLPOINTSUBSCRIPTION:
    {
      signals_->createpullpointsubscription_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case EVENTOPERATION_GETEVENTPROPERTIES:
    {
      signals_->geteventproperties_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case EVENTOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case EVENTOPERATION_PULLMESSAGES:
    {
      signals_->pullmessages_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string EventClient::CreatePullPointSubscriptionBody(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy)
{
  return std::string("<s:Body><tev:CreatePullPointSubscription>" + onvif::ToXml("wsnt:Filter", boost::optional<std::string>(filter)) + onvif::ToXml("wsnt:InitialTerminationTime", boost::optional<std::string>(initialterminationtime)) + onvif::ToXml("wsnt:SubscriptionPolicy", boost::optional<std::string>(subscriptionpolicy)) + "</tev:CreatePullPointSubscription></s:Body>");
}

std::string EventClient::GetEventPropertiesBody()
{
  return std::string("<s:Body><tev:GetEventProperties/></s:Body>");
}

std::string EventClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><tev:GetServiceCapabilities/></s:Body>");
}

std::string EventClient::PullMessagesBody(const onvif::Duration& timeout, const int messagelimit)
{
  return std::string("<s:Body><tev:PullMessages>" + onvif::ToXml("tev:Timeout", boost::optional<std::string>(timeout.ToString())) + onvif::ToXml("tev:MessageLimit", boost::optional<int>(messagelimit)) + "</tev:PullMessages></s:Body>");
}

}

}
