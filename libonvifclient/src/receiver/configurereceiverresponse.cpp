// configurereceiverresponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////
  
ConfigureReceiverResponse::ConfigureReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const ReceiverConfiguration& configuration) :
  Response(client, localendpoint, latency, message),
  receivertoken_(receivertoken),
  configuration_(configuration)
{
  
}

ConfigureReceiverResponse::~ConfigureReceiverResponse()
{
  
}

}

}
