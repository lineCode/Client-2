// dummyrecordingservice.hpp
//

///// Includes /////

#include "dummyrecordingservice.hpp"

#include <pugixml.hpp>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Methods /////

DummyRecordingService::DummyRecordingService()
{
  
}

DummyRecordingService::~DummyRecordingService()
{

}

boost::shared_future<server::ServerResponse> DummyRecordingService::Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata)
{
  pugi::xml_document doc;
  if (!doc.load_buffer(content.data(), content.size()))
  {

    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  std::string response;
  if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateRecording']"))
  {
    response = std::string("<trc:CreateRecordingResponse><trc:RecordingToken>") + createrecording_ + std::string("</trc:RecordingToken></trc:CreateRecordingResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateRecordingJob']"))
  {
    response = std::string("<trc:CreateRecordingJobResponse><trc:JobToken>") + createrecordingjobjobtoken_ + std::string("</trc:JobToken>") + createrecordingjobjobconfiguration_.ToXml("trc:JobConfiguration") + std::string("</trc:CreateRecordingJobResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateTrack']"))
  {
    response = std::string("<trc:CreateTrackResponse><trc:TrackToken>") + createtrack_ + std::string("</trc:TrackToken></trc:CreateTrackResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteRecording']"))
  {
    response = std::string("<trc:DeleteRecordingResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteRecordingJob']"))
  {
    response = std::string("<trc:DeleteRecordingJobResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteTrack']"))
  {
    response = std::string("<trc:DeleteTrackResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ExportRecordedData']"))
  {
    response = std::string("<trc:ExportRecordedDataResponse><tt:OperationToken>") + exportrecordeddataoperationtoken_ + std::string("</tt:OperationToken>") + onvif::ToXml("tt:FileNames", exportrecordeddatafilenames_) + std::string("</trc:ExportRecordedDataResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetExportRecordedDataState']"))
  {
    response = std::string("<trc:GetExportRecordedDataStateResponse>") + onvif::ToXml("tt:Progress", boost::optional<float>(getexportrecordeddatastateprogress_)) + getexportrecordeddatastatefileprogressstatus_.ToXml("tt:FileProgressStatus") + std::string("</trc:GetExportRecordedDataStateResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingConfiguration']"))
  {
    response = std::string("<trc:GetRecordingConfigurationResponse>") + getrecordingconfiguration_.ToXml("tt:RecordingConfiguration") + std::string("</trc:GetRecordingConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingJobConfiguration']"))
  {
    response = std::string("<trc:GetRecordingJobConfigurationResponse>") + getrecordingjobconfiguration_.ToXml("tt:JobConfiguration") + std::string("</trc:GetRecordingJobConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingJobs']"))
  {
    response = std::string("<trc:GetRecordingJobsResponse>") + onvif::ToXml("tt:JobItem", getrecordingjobs_) + std::string("</trc:GetRecordingJobsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingJobState']"))
  {
    response = std::string("<trc:GetRecordingJobStateResponse>") + getrecordingjobstate_.ToXml("tt:State") + std::string("</trc:GetRecordingJobStateResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingOptions']"))
  {
    response = std::string("<trc:GetRecordingOptionsResponse>") + getrecordingoptions_.ToXml("tt:Options") + std::string("</trc:GetRecordingOptionsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordings']"))
  {
    response = std::string("<trc:GetRecordingsResponse>") + onvif::ToXml("tt:RecordingItem", getrecordings_) + std::string("</trc:GetRecordingsResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilities']"))
  {
    response = std::string("<trc:GetServiceCapabilitiesResponse>") + getservicecapabilities_.ToXml("tt:Capabilities") + std::string("</trc:GetServiceCapabilitiesResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetTrackConfiguration']"))
  {
    response = std::string("<trc:GetTrackConfigurationResponse>") + gettrackconfiguration_.ToXml("trc:TrackConfiguration") + std::string("</trc:GetTrackConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRecordingConfiguration']"))
  {
    response = std::string("<trc:SetRecordingConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRecordingJobConfiguration']"))
  {
    response = std::string("<trc:SetRecordingJobConfigurationResponse>") + setrecordingjobconfiguration_.ToXml("tt:JobConfiguration") + std::string("</trc:SetRecordingJobConfigurationResponse>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRecordingJobMode']"))
  {
    response = std::string("<trc:SetRecordingJobModeResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetTrackConfiguration']"))
  {
    response = std::string("<trc:SetTrackConfigurationResponse/>");

  }
  else if (doc.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StopExportRecordedData']"))
  {
    response = std::string("<trc:StopExportRecordedDataResponse>") + onvif::ToXml("tt:Progress", boost::optional<float>(stopexportrecordeddataprogress_)) + stopexportrecordeddatafileprogressstatus_.ToXml("tt:FileProgressStatus") + std::string("</trc:StopExportRecordedDataResponse>");

  }
  else
  {
  
    return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::BADREQUEST, std::string("Error"), std::string("Error 400 Bad Request")));
  }

  return boost::make_ready_future(server::ServerResponse(server::HTTPSTATUSCODE::OK, std::string("<?xml version=\"1.0\" encoding=\"UTF-8\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:trc=\"http://www.onvif.org/ver10/recording/wsdl\"><SOAP-ENV:Body>") + response + std::string("</SOAP-ENV:Body></SOAP-ENV:Envelope>")));
}

}

}
