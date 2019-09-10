// getcompatiblevideoencoderconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetCompatibleVideoEncoderConfigurationsResponse::GetCompatibleVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

GetCompatibleVideoEncoderConfigurationsResponse::GetCompatibleVideoEncoderConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<VideoEncoderConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  configurations_(configurations)
{

}

GetCompatibleVideoEncoderConfigurationsResponse::~GetCompatibleVideoEncoderConfigurationsResponse()
{

}

}

}
