// getvideoanalyticsconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetVideoAnalyticsConfigurationsResponse::GetVideoAnalyticsConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetVideoAnalyticsConfigurationsResponse::GetVideoAnalyticsConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<VideoAnalyticsConfiguration>& videoanalyticsconfigurations) :
  Response(client, localendpoint, latency, message),
  configurations_(videoanalyticsconfigurations)
{

}

GetVideoAnalyticsConfigurationsResponse::~GetVideoAnalyticsConfigurationsResponse()
{

}

}

}
