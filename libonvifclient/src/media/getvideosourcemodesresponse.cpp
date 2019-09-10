// getvideosourcemodesresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetVideoSourceModesResponse::GetVideoSourceModesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetVideoSourceModesResponse::GetVideoSourceModesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& token, const std::vector<media::VideoSourceMode>& videosourcemodes) :
  Response(client, localendpoint, latency, message),
  token_(token),
  videosourcemodes_(videosourcemodes)
{

}

GetVideoSourceModesResponse::~GetVideoSourceModesResponse()
{

}

}

}
