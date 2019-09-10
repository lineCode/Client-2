// getcompatibleaudiosourceconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetCompatibleAudioSourceConfigurationsResponse::GetCompatibleAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

GetCompatibleAudioSourceConfigurationsResponse::GetCompatibleAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioSourceConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  configurations_(configurations)
{

}

GetCompatibleAudioSourceConfigurationsResponse::~GetCompatibleAudioSourceConfigurationsResponse()
{

}

}

}
