// getsnapshoturiresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetSnapshotUriResponse::GetSnapshotUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetSnapshotUriResponse::GetSnapshotUriResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<MediaUri>& mediauri) :
  Response(client, localendpoint, latency, message),
  token_(token),
  mediauri_(mediauri)
{

}

GetSnapshotUriResponse::~GetSnapshotUriResponse()
{

}

}

}
