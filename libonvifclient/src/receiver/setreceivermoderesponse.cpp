// setreceivermoderesponse.cpp
//

///// Includes /////

#include "onvifclient/receiverclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace receiver
{

///// Methods /////

SetReceiverModeResponse::SetReceiverModeResponse(boost::shared_ptr<ReceiverClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& receivertoken, const RECEIVERMODE mode) :
  Response(client, localendpoint, latency, message),
  receivertoken_(receivertoken),
  mode_(mode)
{
  
}

SetReceiverModeResponse::~SetReceiverModeResponse()
{
  
}

}

}
