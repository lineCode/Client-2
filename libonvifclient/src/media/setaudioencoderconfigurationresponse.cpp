// setaudioencoderconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

SetAudioEncoderConfigurationResponse::SetAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioEncoderConfiguration& configuration) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration)
{
  
}

SetAudioEncoderConfigurationResponse::~SetAudioEncoderConfigurationResponse()
{
  
}

}

}
