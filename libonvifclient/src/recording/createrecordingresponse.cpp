// createrecordingresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

CreateRecordingResponse::CreateRecordingResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingConfiguration& recordingconfiguration) :
  Response(client, localendpoint, latency, message),
  recordingconfiguration_(recordingconfiguration)
{

}

CreateRecordingResponse::CreateRecordingResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingConfiguration& recordingconfiguration, const boost::optional<std::string>& recordingtoken) :
  Response(client, localendpoint, latency, message),
  recordingconfiguration_(recordingconfiguration),
  recordingtoken_(recordingtoken)
{
  
}

CreateRecordingResponse::~CreateRecordingResponse()
{
  
}

}

}
