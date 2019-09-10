// createrecordingjobresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

CreateRecordingJobResponse::CreateRecordingJobResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingJobConfiguration& jobconfiguration) :
  Response(client, localendpoint, latency, message),
  jobconfiguration_(jobconfiguration)
{

}

CreateRecordingJobResponse::CreateRecordingJobResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingJobConfiguration& jobconfiguration, const boost::optional<std::string>& jobtoken, const boost::optional<RecordingJobConfiguration>& jobconfigurationresponse) :
  Response(client, localendpoint, latency, message),
  jobconfiguration_(jobconfiguration),
  jobtoken_(jobtoken),
  jobconfigurationresponse_(jobconfigurationresponse)
{
  
}

CreateRecordingJobResponse::~CreateRecordingJobResponse()
{
  
}

}

}
