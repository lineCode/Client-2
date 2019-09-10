// getaudiooutputsresponse.cpp
//

///// Includes /////

#include "onvifclient/deviceioclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace deviceio
{

///// Methods /////

GetAudioOutputsResponse::GetAudioOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetAudioOutputsResponse::GetAudioOutputsResponse(boost::shared_ptr<DeviceIOClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<std::string>& token) :
  Response(client, localendpoint, latency, message),
  token_(token)
{

}

GetAudioOutputsResponse::~GetAudioOutputsResponse()
{

}

}

}
