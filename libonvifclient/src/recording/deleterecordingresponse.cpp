// deleterecordingresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

DeleteRecordingResponse::DeleteRecordingResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken)
{

}

DeleteRecordingResponse::~DeleteRecordingResponse()
{
  
}

}

}
