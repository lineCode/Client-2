// setaudiosourceconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

SetAudioSourceConfigurationResponse::SetAudioSourceConfigurationResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const AudioSourceConfiguration& configuration, bool forcepersistence) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration),
  forcepersistence_(forcepersistence)
{

}

SetAudioSourceConfigurationResponse::~SetAudioSourceConfigurationResponse()
{

}

}

}
