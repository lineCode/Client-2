// getrecordingoptionsresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetRecordingOptionsResponse::GetRecordingOptionsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken)
{

}

GetRecordingOptionsResponse::GetRecordingOptionsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const boost::optional<RecordingOptions>& options) :
  Response(client, localendpoint, latency, message),
  recordingtoken_(recordingtoken),
  options_(options)
{
  
}

GetRecordingOptionsResponse::~GetRecordingOptionsResponse()
{
  
}

}

}
