// getrecordsingsresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetRecordingsResponse::GetRecordingsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) :
  Response(client, localendpoint, latency, message)
{

}

GetRecordingsResponse::GetRecordingsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<recording::GetRecordingsResponseItem>& recordingitem) :
  Response(client, localendpoint, latency, message),
  recordingitem_(recordingitem)
{
  
}

GetRecordingsResponse::~GetRecordingsResponse()
{
  
}

}

}
