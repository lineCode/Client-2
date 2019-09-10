// addaudiodecoderconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

AddAudioDecoderConfigurationResponse::AddAudioDecoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::string& configurationtoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  configurationtoken_(configurationtoken)
{

}

AddAudioDecoderConfigurationResponse::~AddAudioDecoderConfigurationResponse()
{

}

}

}
