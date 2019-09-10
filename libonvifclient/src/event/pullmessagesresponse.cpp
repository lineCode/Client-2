// pullmessagesresponse.cpp
//

///// Includes /////

#include "onvifclient/eventclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace event
{

///// Methods /////

PullMessagesResponse::PullMessagesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const onvif::Duration& timeout, const int messagelimit) :
  Response(client, localendpoint, latency, message),
  timeout_(timeout),
  messagelimit_(messagelimit)
{

}

PullMessagesResponse::PullMessagesResponse(boost::shared_ptr<EventClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const onvif::Duration& timeout, const int messagelimit, const boost::optional<onvif::DateTime>& currenttime, const boost::optional<onvif::DateTime>& terminationtime, const std::vector<Element>& notificationmessage) :
  Response(client, localendpoint, latency, message),
  timeout_(timeout),
  messagelimit_(messagelimit),
  currenttime_(currenttime),
  terminationtime_(terminationtime),
  notificationmessage_(notificationmessage)
{

}

PullMessagesResponse::~PullMessagesResponse()
{

}

}

}
