// setaudiooutputconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

SetAudioOutputConfigurationResponse::SetAudioOutputConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioOutputConfiguration& configuration, bool forcepersistence) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration),
  forcepersistence_(forcepersistence)
{

}

SetAudioOutputConfigurationResponse::~SetAudioOutputConfigurationResponse()
{

}

}

}
