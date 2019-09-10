// getaudiosourcesresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioSourcesResponse::GetAudioSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAudioSourcesResponse::GetAudioSourcesResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioSource>& audiosources) :
  Response(client, localendpoint, latency, message),
  audiosources_(audiosources)
{

}

GetAudioSourcesResponse::~GetAudioSourcesResponse()
{

}

}

}
