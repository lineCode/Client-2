// deleterecordingjobresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

DeleteRecordingJobResponse::DeleteRecordingJobResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken) :
  Response(client, localendpoint, latency, message),
  jobtoken_(jobtoken)
{

}

DeleteRecordingJobResponse::~DeleteRecordingJobResponse()
{
  
}

}

}
