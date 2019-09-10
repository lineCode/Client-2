// getaudiosourceconfigurationoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  profiletoken_(profiletoken)
{

}

GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<std::string>& configurationtoken, const boost::optional<std::string>& profiletoken, const boost::optional<AudioSourceConfigurationOptions>& options) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  profiletoken_(profiletoken),
  options_(options)
{

}

GetAudioSourceConfigurationOptionsResponse::~GetAudioSourceConfigurationOptionsResponse()
{

}

}

}
