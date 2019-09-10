// setaudiooutputconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

SetAudioOutputConfigurationResponse::SetAudioOutputConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioOutputConfiguration& configuration) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration)
{
  
}

SetAudioOutputConfigurationResponse::~SetAudioOutputConfigurationResponse()
{
  
}

}

}
