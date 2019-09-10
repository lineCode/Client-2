// createpullpointsubscriptionresponse.cpp
//

///// Includes /////

#include "onvifclient/eventclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace event
{

///// Methods /////

CreatePullPointSubscriptionResponse::CreatePullPointSubscriptionResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy) :
  Response(client, localendpoint, latency, message),
  filter_(filter),
  initialterminationtime_(initialterminationtime),
  subscriptionpolicy_(subscriptionpolicy)
{

}

CreatePullPointSubscriptionResponse::CreatePullPointSubscriptionResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& filter, const boost::optional<std::string>& initialterminationtime, const boost::optional<std::string>& subscriptionpolicy, const boost::optional<onvif::ws::EndpointReferenceType>& subscriptionreference, const boost::optional<std::string>& currenttime, const boost::optional<std::string>& terminationtime) :
  Response(client, localendpoint, latency, message),
  filter_(filter),
  initialterminationtime_(initialterminationtime),
  subscriptionpolicy_(subscriptionpolicy),
  subscriptionreference_(subscriptionreference),
  currenttime_(currenttime),
  terminationtime_(terminationtime)
{

}

CreatePullPointSubscriptionResponse::~CreatePullPointSubscriptionResponse()
{

}

}

}
