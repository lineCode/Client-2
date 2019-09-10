// getconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

GetConfigurationResponse::GetConfigurationResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& ptzconfigurationtoken) :
  Response(client, localendpoint, latency, message),
  ptzconfigurationtoken_(ptzconfigurationtoken)
{

}

GetConfigurationResponse::GetConfigurationResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& ptzconfigurationtoken, const boost::optional<PTZConfiguration>& ptzconfiguration) :
  Response(client, localendpoint, latency, message),
  ptzconfiguration_(ptzconfiguration),
  ptzconfigurationtoken_(ptzconfigurationtoken)
{

}

GetConfigurationResponse::~GetConfigurationResponse()
{

}

}

}
