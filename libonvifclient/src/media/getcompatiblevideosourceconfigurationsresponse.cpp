// getcompatiblevideosourceconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetCompatibleVideoSourceConfigurationsResponse::GetCompatibleVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

GetCompatibleVideoSourceConfigurationsResponse::GetCompatibleVideoSourceConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<VideoSourceConfiguration>& compatiblevideosourceconfigurations) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  configurations_(compatiblevideosourceconfigurations)
{

}

GetCompatibleVideoSourceConfigurationsResponse::~GetCompatibleVideoSourceConfigurationsResponse()
{

}

}

}
