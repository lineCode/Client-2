// continuousmoveresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

ContinuousMoveResponse::ContinuousMoveResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const PTZSpeed& velocity, const Duration& timeout) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  velocity_(velocity),
  timeout_(timeout)
{

}

ContinuousMoveResponse::~ContinuousMoveResponse()
{

}

}

}
