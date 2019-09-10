// getcompatibleaudiooutputconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetCompatibleAudioOutputConfigurationsResponse::GetCompatibleAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

GetCompatibleAudioOutputConfigurationsResponse::GetCompatibleAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioOutputConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  configurations_(configurations)
{

}

GetCompatibleAudioOutputConfigurationsResponse::~GetCompatibleAudioOutputConfigurationsResponse()
{

}

}

}
