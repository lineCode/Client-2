// getvideosourceconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetVideoSourceConfigurationsResponse::GetVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetVideoSourceConfigurationsResponse::GetVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoSourceConfiguration>& videosourceconfigurations) :
  Response(client, localendpoint, latency, message),
  configurations_(videosourceconfigurations)
{

}

GetVideoSourceConfigurationsResponse::~GetVideoSourceConfigurationsResponse()
{

}

}

}
