// setvideoencoderconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

SetVideoEncoderConfigurationResponse::SetVideoEncoderConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const VideoEncoderConfiguration& videoencoderconfiguration) :
  Response(client, localendpoint, latency, message),
  videoencoderconfiguration_(videoencoderconfiguration)
{

}

SetVideoEncoderConfigurationResponse::~SetVideoEncoderConfigurationResponse()
{

}

}

}
