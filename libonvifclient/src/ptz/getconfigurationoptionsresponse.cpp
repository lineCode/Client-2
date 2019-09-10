// getconfigurationoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

GetConfigurationOptionsResponse::GetConfigurationOptionsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken)
{

}

GetConfigurationOptionsResponse::GetConfigurationOptionsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& configurationtoken, const boost::optional<PTZConfigurationOptions>& ptzconfigurationoptions) :
  Response(client, localendpoint, latency, message),
  configurationtoken_(configurationtoken),
  ptzconfigurationoptions_(ptzconfigurationoptions)
{

}

GetConfigurationOptionsResponse::~GetConfigurationOptionsResponse()
{

}

}

}
