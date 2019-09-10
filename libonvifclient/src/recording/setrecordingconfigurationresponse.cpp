// setrecordingconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////
  
SetRecordingConfigurationResponse::SetRecordingConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  recordingconfiguration_(recordingconfiguration)
{
  
}

SetRecordingConfigurationResponse::~SetRecordingConfigurationResponse()
{
  
}

}

}
