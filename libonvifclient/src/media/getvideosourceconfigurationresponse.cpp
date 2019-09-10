// getvideosourceconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetVideoSourceConfigurationResponse::GetVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetVideoSourceConfigurationResponse::GetVideoSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const boost::optional<VideoSourceConfiguration>& videosourceconfiguration) :
  Response(client, localendpoint, latency, message),
  token_(token),
  videosourceconfiguration_(videosourceconfiguration)
{

}

GetVideoSourceConfigurationResponse::~GetVideoSourceConfigurationResponse()
{

}

}

}
