// getaudiooutputconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken) :
  Response(client, localendpoint, latency, message),
  audiooutputtoken_(audiooutputtoken)
{

}

GetAudioOutputConfigurationResponse::GetAudioOutputConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& audiooutputtoken, const boost::optional<AudioOutputConfiguration>& audiooutputconfiguration) :
  Response(client, localendpoint, latency, message),
  audiooutputtoken_(audiooutputtoken),
  audiooutputconfiguration_(audiooutputconfiguration)
{

}

GetAudioOutputConfigurationResponse::~GetAudioOutputConfigurationResponse()
{

}

}

}
