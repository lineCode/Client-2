// exportrecordeddataresponse.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Methods /////

ExportRecordedDataResponse::ExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ws::DateTime startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination) :
  Response(client, localendpoint, latency, message),
  startpoint_(startpoint),
  endpoint_(endpoint),
  searchscope_(searchscope),
  fileformat_(fileformat),
  storagedestination_(storagedestination)
{

}

ExportRecordedDataResponse::ExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ws::DateTime startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination, const boost::optional<std::string>& operationtoken, const std::vector<std::string>& filenames) :
  Response(client, localendpoint, latency, message),
  startpoint_(startpoint),
  endpoint_(endpoint),
  searchscope_(searchscope),
  fileformat_(fileformat),
  storagedestination_(storagedestination),
  operationtoken_(operationtoken),
  filenames_(filenames)
{
  
}

ExportRecordedDataResponse::~ExportRecordedDataResponse()
{
  
}

}

}
