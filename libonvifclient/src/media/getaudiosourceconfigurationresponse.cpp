// getaudiosourceconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken)
{

}

GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioSourceConfiguration>& configuration) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  configuration_(configuration)
{

}

GetAudioSourceConfigurationResponse::~GetAudioSourceConfigurationResponse()
{

}

}

}
