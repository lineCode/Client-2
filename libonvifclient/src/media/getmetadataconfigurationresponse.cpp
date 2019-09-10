// getmetadataconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

GetMetadataConfigurationResponse::GetMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken)
{

}

GetMetadataConfigurationResponse::GetMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<MetadataConfiguration>& configuration) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  configuration_(configuration)
{

}

GetMetadataConfigurationResponse::~GetMetadataConfigurationResponse()
{

}

}

}
