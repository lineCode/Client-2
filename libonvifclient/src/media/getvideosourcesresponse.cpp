// getvideosourcesresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetVideoSourcesResponse::GetVideoSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetVideoSourcesResponse::GetVideoSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoSource>& videosources) :
  Response(client, localendpoint, latency, message),
  videosources_(videosources)
{

}

GetVideoSourcesResponse::~GetVideoSourcesResponse()
{

}

}

}
