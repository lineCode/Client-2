// createreceiverresponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////

CreateReceiverResponse::CreateReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ReceiverConfiguration& configuration) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration)
{

}

CreateReceiverResponse::CreateReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ReceiverConfiguration& configuration, const boost::optional<Receiver>& receiver) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration),
  receiver_(receiver)
{
  
}

CreateReceiverResponse::~CreateReceiverResponse()
{
  
}

}

}
