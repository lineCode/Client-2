// getrecordingconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetRecordingConfigurationResponse::GetRecordingConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken)
{

}

GetRecordingConfigurationResponse::GetRecordingConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const boost::optional<RecordingConfiguration>& recordingconfiguration) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  recordingconfiguration_(recordingconfiguration)
{
  
}

GetRecordingConfigurationResponse::~GetRecordingConfigurationResponse()
{
  
}

}

}
