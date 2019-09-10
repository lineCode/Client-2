// getexportrecordeddatastateresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

GetExportRecordedDataStateResponse::GetExportRecordedDataStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken) :
  Response(client, localendpoint, latency, message),
  operationtoken_(operationtoken)
{

}

GetExportRecordedDataStateResponse::GetExportRecordedDataStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken, const boost::optional<float>& progress, const boost::optional<ArrayOfFileProgress>& fileprogresstatus) :
  Response(client, localendpoint, latency, message),
  operationtoken_(operationtoken),
  progress_(progress),
  fileprogresstatus_(fileprogresstatus)
{
  
}

GetExportRecordedDataStateResponse::~GetExportRecordedDataStateResponse()
{
  
}

}

}
