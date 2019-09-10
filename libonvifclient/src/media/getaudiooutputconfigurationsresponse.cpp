// getaudiooutputconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetAudioOutputConfigurationsResponse::GetAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAudioOutputConfigurationsResponse::GetAudioOutputConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<AudioOutputConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  configurations_(configurations)
{

}

GetAudioOutputConfigurationsResponse::~GetAudioOutputConfigurationsResponse()
{

}

}

}
