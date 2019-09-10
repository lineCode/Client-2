// setmetadataconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

SetMetadataConfigurationResponse::SetMetadataConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const MetadataConfiguration& configuration) :
  Response(client, localendpoint, latency, message),
  configuration_(configuration)
{
  
}

SetMetadataConfigurationResponse::~SetMetadataConfigurationResponse()
{
  
}

}

}
