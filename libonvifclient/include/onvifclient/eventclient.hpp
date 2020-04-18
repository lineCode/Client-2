// eventclient.hpp
//

#ifndef IDC1ECTYFLZG2D63F0WJUBXZTSKGFO4R7F
#define IDC1ECTYFLZG2D63F0WJUBXZTSKGFO4R7F

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace event
{

///// Enumerations /////

enum EVENTOPERATION
{
  EVENTOPERATION_CREATEPULLPOINTSUBSCRIPTION,
  EVENTOPERATION_GETEVENTPROPERTIES,
  EVENTOPERATION_GETSERVICECAPABILITIES,
  EVENTOPERATION_PULLMESSAGES
};

///// Declarations /////

class EventClient;
class EventSignals;

///// Classes /////

class CreatePullPointSubscriptionResponse : public Response<EventClient>
{
 public:

  CreatePullPointSubscriptionResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy);
  CreatePullPointSubscriptionResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy, const boost::optional<onvif::ws::EndpointReferenceType>& subscriptionreference, const boost::optional<std::string>& currenttime, const boost::optional<std::string>& terminationtime);
  virtual ~CreatePullPointSubscriptionResponse();

  boost::optional<std::string> filter_;
  boost::optional<std::string> initialterminationtime_;
  boost::optional<std::string> subscriptionpolicy_;

  boost::optional<onvif::ws::EndpointReferenceType> subscriptionreference_;
  boost::optional<std::string> currenttime_;
  boost::optional<std::string> terminationtime_;

};

class GetEventPropertiesResponse : public Response<EventClient>
{
 public:

  GetEventPropertiesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetEventPropertiesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& topicnamespacelocation, const boost::optional<bool>& fixedtopicset, const boost::optional<ws::TopicSet>& topicset, const std::vector<std::string>& topicexpressiondialect, const std::vector<std::string>& messagecontentfilterdialect, const std::vector<std::string>& producerpropertiesfilterdialect, const std::vector<std::string>& messagecontentschemalocation);
  virtual ~GetEventPropertiesResponse();

  std::vector<std::string> topicnamespacelocation_;
  boost::optional<bool> fixedtopicset_;
  boost::optional<ws::TopicSet> topicset_;
  std::vector<std::string> topicexpressiondialect_;
  std::vector<std::string> messagecontentfilterdialect_;
  std::vector<std::string> producerpropertiesfilterdialect_;
  std::vector<std::string> messagecontentschemalocation_;

};

class GetServiceCapabilitiesResponse : public Response<EventClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class PullMessagesResponse : public Response<EventClient>
{
 public:

  PullMessagesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const onvif::Duration& timeout, const int messagelimit);
  PullMessagesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const onvif::Duration& timeout, const int messagelimit, const boost::optional<onvif::DateTime>& currenttime, const boost::optional<onvif::DateTime>& terminationtime, const std::vector<Element>& notificationmessage);
  virtual ~PullMessagesResponse();

  onvif::Duration timeout_;
  int messagelimit_;

  boost::optional<onvif::DateTime> currenttime_;
  boost::optional<onvif::DateTime> terminationtime_;
  std::vector<Element> notificationmessage_;

};

class EventClient : public Client<EVENTOPERATION>, public boost::enable_shared_from_this<EventClient>
{
  friend Signal< EVENTOPERATION, EventClient, CreatePullPointSubscriptionResponse, boost::optional<std::string>, boost::optional<std::string>, boost::optional<std::string> >;
  friend Signal<EVENTOPERATION, EventClient, GetEventPropertiesResponse>;
  friend Signal<EVENTOPERATION, EventClient, GetServiceCapabilitiesResponse>;
  friend Signal<EVENTOPERATION, EventClient, PullMessagesResponse, onvif::Duration, int>;

 public:
  
  using Client::Update;

  EventClient(const boost::shared_ptr<std::recursive_mutex>& mutex);
  virtual ~EventClient();

  virtual int Init(const sock::ProxyParams& proxyparams, const std::string& address, const std::string& username, const std::string& password, const unsigned int maxconcurrentrequests, const bool forcehttpauthentication, const bool forbidreuse) override;
  virtual void Destroy() override;

  // Requests
  void CreatePullPointSubscription(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy);
  void GetEventProperties();
  void GetServiceCapabilities();
  void PullMessages(const std::vector<Element>& referenceparameters, const std::string& to, const onvif::Duration& timeout, const int messagelimit);
  
  // Callback requests
  Connection CreatePullPointSubscriptionCallback(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy, boost::function<void(const CreatePullPointSubscriptionResponse&)> callback);
  Connection GetEventPropertiesCallback(boost::function<void(const GetEventPropertiesResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection PullMessagesCallback(const std::vector<Element>& referenceparameters, const std::string& to, const onvif::Duration& timeout, const int messagelimit, boost::function<void(const PullMessagesResponse&)> callback);

  // Future requests
  boost::unique_future<CreatePullPointSubscriptionResponse> CreatePullPointSubscriptionFuture(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy);
  boost::unique_future<GetEventPropertiesResponse> GetEventPropertiesFuture();
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<PullMessagesResponse> PullMessagesFuture(const std::vector<Element>& referenceparameters, const std::string& to, const onvif::Duration& timeout, const int messagelimit);

  // Boost Signals2
  boost::signals2::signal<void(const CreatePullPointSubscriptionResponse&)>& CreatePullPointSubscriptionSignal();
  boost::signals2::signal<void(const GetEventPropertiesResponse&)>& GetEventPropertiesSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const PullMessagesResponse&)>& PullMessagesSignal();

 private:

  virtual void Update(EVENTOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(EVENTOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string CreatePullPointSubscriptionBody(const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy);
  std::string GetEventPropertiesBody();
  std::string GetServiceCapabilitiesBody();
  std::string PullMessagesBody(const onvif::Duration& timeout, const int messagelimit);

  // Signals
  EventSignals* signals_;

};

}

}

#endif
