// stopexportrecordeddataresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

StopExportRecordedDataResponse::StopExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken) :
  Response(client, localendpoint, latency, message),
  operationtoken_(operationtoken)
{

}

StopExportRecordedDataResponse::StopExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken, const boost::optional<float>& progress, const boost::optional<ArrayOfFileProgress>& fileprogressstatus) :
  Response(client, localendpoint, latency, message),
  operationtoken_(operationtoken),
  progress_(progress),
  fileprogressstatus_(fileprogressstatus)
{
  
}

StopExportRecordedDataResponse::~StopExportRecordedDataResponse()
{
  
}

}

}
