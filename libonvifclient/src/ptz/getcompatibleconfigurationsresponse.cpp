// getcompatibleconfigurationsresponse.cpp
//

///// Includes /////

#include "onvifclient/ptzclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace ptz
{

///// Methods /////

GetCompatibleConfigurationsResponse::GetCompatibleConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken)
{

}

GetCompatibleConfigurationsResponse::GetCompatibleConfigurationsResponse(boost::shared_ptr<PTZClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& profiletoken, const std::vector<PTZConfiguration>& ptzconfigurations) :
  Response(client, localendpoint, latency, message),
  profiletoken_(profiletoken),
  ptzconfigurations_(ptzconfigurations)
{

}

GetCompatibleConfigurationsResponse::~GetCompatibleConfigurationsResponse()
{

}

}

}
