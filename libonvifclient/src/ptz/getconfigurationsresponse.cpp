// getconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

GetConfigurationsResponse::GetConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetConfigurationsResponse::GetConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<PTZConfiguration>& ptzconfiguration) :
  Response(client, localendpoint, latency, message),
  ptzconfiguration_(ptzconfiguration)
{

}

GetConfigurationsResponse::~GetConfigurationsResponse()
{

}

}

}
