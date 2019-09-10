// stopresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

StopResponse::StopResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, bool pantilt, bool zoom) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  pantilt_(pantilt),
  zoom_(zoom)
{

}

StopResponse::~StopResponse()
{

}

}

}
