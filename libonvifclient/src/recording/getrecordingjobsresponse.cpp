// getrecordingjobsresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetRecordingJobsResponse::GetRecordingJobsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetRecordingJobsResponse::GetRecordingJobsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<recording::GetRecordingJobsResponseItem>& jobitem) :
  Response(client, localendpoint, latency, message),
  jobitem_(jobitem)
{
  
}

GetRecordingJobsResponse::~GetRecordingJobsResponse()
{
  
}

}

}
