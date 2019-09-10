// getreceiverresponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////
  
GetReceiversResponse::GetReceiversResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}
  
GetReceiversResponse::GetReceiversResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<Receiver>& receivers) :
  Response(client, localendpoint, latency, message),
  receivers_(receivers)
{
  
}

GetReceiversResponse::~GetReceiversResponse()
{
  
}

}

}
