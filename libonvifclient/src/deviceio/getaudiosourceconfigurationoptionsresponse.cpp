// getaudiosourceconfigurationoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken) :
  Response(client, localendpoint, latency, message),
  audiosourcetoken_(audiosourcetoken)
{

}

GetAudioSourceConfigurationOptionsResponse::GetAudioSourceConfigurationOptionsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken, const boost::optional<AudioSourceConfigurationOptions>& audiosourceoptions) :
  Response(client, localendpoint, latency, message),
  audiosourcetoken_(audiosourcetoken),
  audiosourceoptions_(audiosourceoptions)
{

}

GetAudioSourceConfigurationOptionsResponse::~GetAudioSourceConfigurationOptionsResponse()
{

}

}

}
