// setaudiosourceconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

SetAudioSourceConfigurationResponse::SetAudioSourceConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioSourceConfiguration& configuration) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration)
{
  
}

SetAudioSourceConfigurationResponse::~SetAudioSourceConfigurationResponse()
{
  
}

}

}
