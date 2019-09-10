// getreplayuriresponse.cpp
//

///// Includes /////

#include "onvifclient/replayclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace replay
{

///// Methods /////

GetReplayUriResponse::GetReplayUriResponse(boost::shared_ptr<ReplayClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StreamSetup& streamsetup, const std::string& recordingtoken) :
  Response(client, localendpoint, latency, message),
  streamsetup_(streamsetup),
  recordingtoken_(recordingtoken)
{

}

GetReplayUriResponse::GetReplayUriResponse(boost::shared_ptr<ReplayClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const StreamSetup& streamsetup, const std::string& recordingtoken, const boost::optional<std::string>& uri) :
  Response(client, localendpoint, latency, message),
  streamsetup_(streamsetup),
  recordingtoken_(recordingtoken),
  uri_(uri)
{

}

GetReplayUriResponse::~GetReplayUriResponse()
{

}

}

}
