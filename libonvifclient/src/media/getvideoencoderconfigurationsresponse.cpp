// getvideoencoderconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetVideoEncoderConfigurationsResponse::GetVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetVideoEncoderConfigurationsResponse::GetVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoEncoderConfiguration>& videoencoderconfigurations) :
  Response(client, localendpoint, latency, message),
  configurations_(videoencoderconfigurations)
{

}

GetVideoEncoderConfigurationsResponse::~GetVideoEncoderConfigurationsResponse()
{

}

}

}
