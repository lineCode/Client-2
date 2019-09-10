// getaudioencoderconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioEncoderConfigurationResponse::GetAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken)
{

}

GetAudioEncoderConfigurationResponse::GetAudioEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<AudioEncoderConfiguration>& configuration) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  configuration_(configuration)
{

}

GetAudioEncoderConfigurationResponse::~GetAudioEncoderConfigurationResponse()
{

}

}

}
