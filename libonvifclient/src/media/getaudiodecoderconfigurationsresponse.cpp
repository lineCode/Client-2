// getaudiodecoderconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioDecoderConfigurationsResponse::GetAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAudioDecoderConfigurationsResponse::GetAudioDecoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioDecoderConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  configurations_(configurations)
{

}

GetAudioDecoderConfigurationsResponse::~GetAudioDecoderConfigurationsResponse()
{

}

}

}
