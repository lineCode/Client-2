// getreceiverstateresponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////
  
GetReceiverStateResponse::GetReceiverStateResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken) :
  Response(client, localendpoint, latency, message),
  receivertoken_(receivertoken)
{

}
  
GetReceiverStateResponse::GetReceiverStateResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const boost::optional<ReceiverStateInformation>& receiverstate) :
  Response(client, localendpoint, latency, message),
  receivertoken_(receivertoken),
  receiverstate_(receiverstate)
{
  
}

GetReceiverStateResponse::~GetReceiverStateResponse()
{
  
}

}

}
