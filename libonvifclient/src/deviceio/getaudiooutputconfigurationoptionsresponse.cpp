// getaudiooutputconfigurationoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetAudioOutputConfigurationOptionsResponse::GetAudioOutputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken) :
  Response(client, localendpoint, latency, message),
  audiooutputtoken_(audiooutputtoken)
{

}

GetAudioOutputConfigurationOptionsResponse::GetAudioOutputConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken, const boost::optional<AudioOutputConfigurationOptions>& audiooutputoptions) :
  Response(client, localendpoint, latency, message),
  audiooutputtoken_(audiooutputtoken),
  audiooutputoptions_(audiooutputoptions)
{

}

GetAudioOutputConfigurationOptionsResponse::~GetAudioOutputConfigurationOptionsResponse()
{

}

}

}
