// getcompatibleaudiodecoderconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetCompatibleAudioDecoderConfigurationsResponse::GetCompatibleAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

GetCompatibleAudioDecoderConfigurationsResponse::GetCompatibleAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<AudioDecoderConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  configurations_(configurations)
{

}

GetCompatibleAudioDecoderConfigurationsResponse::~GetCompatibleAudioDecoderConfigurationsResponse()
{

}

}

}
