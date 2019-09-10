// getaudiosourceconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken) :
  Response(client, localendpoint, latency, message),
  audiosourcetoken_(audiosourcetoken)
{

}

GetAudioSourceConfigurationResponse::GetAudioSourceConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiosourcetoken, const boost::optional<AudioSourceConfiguration>& audiosourceconfiguration) :
  Response(client, localendpoint, latency, message),
  audiosourcetoken_(audiosourcetoken),
  audiosourceconfiguration_(audiosourceconfiguration)
{

}

GetAudioSourceConfigurationResponse::~GetAudioSourceConfigurationResponse()
{

}

}

}
