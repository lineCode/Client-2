// getstreamuriresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetStreamUriResponse::GetStreamUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StreamSetup& streamsetup, const std::string& token) :
  Response(client, localendpoint, latency, message),
  streamsetup_(streamsetup),
  token_(token)
{

}

GetStreamUriResponse::GetStreamUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StreamSetup& streamsetup, const std::string& token, const boost::optional<MediaUri>& mediauri) :
  Response(client, localendpoint, latency, message),
  streamsetup_(streamsetup),
  token_(token),
  mediauri_(mediauri)
{

}

GetStreamUriResponse::~GetStreamUriResponse()
{

}

}

}
