// setrecordingjobconfigurationresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////
  
SetRecordingJobConfigurationResponse::SetRecordingJobConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration) :
  Response(client, localendpoint, latency, message),
  jobtoken_(jobtoken),
  jobconfiguration_(jobconfiguration)
{
  
}

SetRecordingJobConfigurationResponse::SetRecordingJobConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration, const boost::optional<RecordingJobConfiguration>& jobconfigurationresponse) :
  Response(client, localendpoint, latency, message),
  jobtoken_(jobtoken),
  jobconfiguration_(jobconfiguration),
  jobconfigurationresponse_(jobconfigurationresponse)
{
  
}

SetRecordingJobConfigurationResponse::~SetRecordingJobConfigurationResponse()
{
  
}

}

}
