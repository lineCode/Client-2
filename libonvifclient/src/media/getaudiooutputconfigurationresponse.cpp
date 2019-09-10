// getaudiooutputconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken)
{

}

GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioOutputConfiguration>& configuration) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  configuration_(configuration)
{

}

GetAudioOutputConfigurationResponse::~GetAudioOutputConfigurationResponse()
{

}

}

}
