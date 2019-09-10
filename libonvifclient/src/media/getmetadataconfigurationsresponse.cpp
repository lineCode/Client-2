// getmetadataconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetMetadataConfigurationsResponse::GetMetadataConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetMetadataConfigurationsResponse::GetMetadataConfigurationsResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<MetadataConfiguration>& configurations) :
  Response(client, localendpoint, latency, message),
  configurations_(configurations)
{

}

GetMetadataConfigurationsResponse::~GetMetadataConfigurationsResponse()
{

}

}

}
