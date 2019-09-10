// removeptzconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/mediaclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace media
{

///// Methods /////

RemovePTZConfigurationResponse::RemovePTZConfigurationResponse(boost::shared_ptr<MediaClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

RemovePTZConfigurationResponse::~RemovePTZConfigurationResponse()
{

}

}

}
