// getaudiosourceconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioSourceConfigurationsResponse::GetAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAudioSourceConfigurationsResponse::GetAudioSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioSourceConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  configurations_(configurations)
{

}

GetAudioSourceConfigurationsResponse::~GetAudioSourceConfigurationsResponse()
{

}

}

}
