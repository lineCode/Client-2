// getrecordingjobstateresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetRecordingJobStateResponse::GetRecordingJobStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken) :
  Response(client, localendpoint, latency, message),
  jobtoken_(jobtoken)
{

}

GetRecordingJobStateResponse::GetRecordingJobStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message,  const std::string& jobtoken, const boost::optional<RecordingJobStateInformation>& state) :
  Response(client, localendpoint, latency, message),
  jobtoken_(jobtoken),
  state_(state)
{
  
}

GetRecordingJobStateResponse::~GetRecordingJobStateResponse()
{
  
}

}

}
