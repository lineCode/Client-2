// setrecordingjobmoderesponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////
  
SetRecordingJobModeResponse::SetRecordingJobModeResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const RECORDINGJOBMODE& mode) :
  Response(client, localendpoint, latency, message),
  jobtoken_(jobtoken),
  mode_(mode)
{
  
}

SetRecordingJobModeResponse::~SetRecordingJobModeResponse()
{
  
}

}

}
