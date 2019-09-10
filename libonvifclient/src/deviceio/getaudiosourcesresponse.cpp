// getaudiosourcesresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetAudioSourcesResponse::GetAudioSourcesResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAudioSourcesResponse::GetAudioSourcesResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetAudioSourcesResponse::~GetAudioSourcesResponse()
{

}

}

}
