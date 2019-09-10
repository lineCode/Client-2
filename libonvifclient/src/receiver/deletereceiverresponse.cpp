// deletereceiverresponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////

DeleteReceiverResponse::DeleteReceiverResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken) :
  Response(client, localendpoint, latency, message),
  receivertoken_(receivertoken)
{
  
}

DeleteReceiverResponse::~DeleteReceiverResponse()
{
  
}

}

}
