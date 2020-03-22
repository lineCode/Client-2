// recordingclient.cpp
//

///// Includes /////

#include "onvifclient/recordingclient.hpp"

#include <boost/algorithm/string.hpp>

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Classes /////

class RecordingSignals
{
 public:

  Signal<RECORDINGOPERATION, RecordingClient, CreateRecordingResponse, RecordingConfiguration> createrecording_;
  Signal<RECORDINGOPERATION, RecordingClient, CreateRecordingJobResponse, RecordingJobConfiguration> createrecordingjob_;
  Signal<RECORDINGOPERATION, RecordingClient, CreateTrackResponse, std::string, TrackConfiguration> createtrack_;
  Signal<RECORDINGOPERATION, RecordingClient, DeleteRecordingResponse, std::string> deleterecording_;
  Signal<RECORDINGOPERATION, RecordingClient, DeleteRecordingJobResponse, std::string> deleterecordingjob_;
  Signal<RECORDINGOPERATION, RecordingClient, DeleteTrackResponse, std::string, std::string> deletetrack_;
  Signal<RECORDINGOPERATION, RecordingClient, ExportRecordedDataResponse, ws::DateTime, ws::DateTime, SearchScope, std::string, StorageReferencePath> exportrecordeddata_;
  Signal<RECORDINGOPERATION, RecordingClient, GetExportRecordedDataStateResponse, std::string> getexportrecordeddatastate_;
  Signal<RECORDINGOPERATION, RecordingClient, GetRecordingConfigurationResponse, std::string> getrecordingconfiguration_;
  Signal<RECORDINGOPERATION, RecordingClient, GetRecordingJobConfigurationResponse, std::string> getrecordingjobconfiguration_;
  Signal< RECORDINGOPERATION, RecordingClient, GetRecordingJobsResponse> getrecordingjobs_;
  Signal<RECORDINGOPERATION, RecordingClient, GetRecordingJobStateResponse, std::string> getrecordingjobstate_;
  Signal<RECORDINGOPERATION, RecordingClient, GetRecordingOptionsResponse, std::string> getrecordingoptions_;
  Signal<RECORDINGOPERATION, RecordingClient, GetRecordingsResponse> getrecordings_;
  Signal<RECORDINGOPERATION, RecordingClient, GetServiceCapabilitiesResponse> getservicecapabilities_;
  Signal<RECORDINGOPERATION, RecordingClient, GetTrackConfigurationResponse, std::string, std::string> gettrackconfiguration_;
  Signal<RECORDINGOPERATION, RecordingClient, SetRecordingConfigurationResponse, std::string, RecordingConfiguration> setrecordingconfiguration_;
  Signal<RECORDINGOPERATION, RecordingClient, SetRecordingJobConfigurationResponse, std::string, RecordingJobConfiguration> setrecordingjobconfiguration_;
  Signal<RECORDINGOPERATION, RecordingClient, SetRecordingJobModeResponse, std::string, RECORDINGJOBMODE> setrecordingjobmode_;
  Signal<RECORDINGOPERATION, RecordingClient, SetTrackConfigurationResponse, std::string, std::string, TrackConfiguration> settrackconfiguration_;
  Signal<RECORDINGOPERATION, RecordingClient, StopExportRecordedDataResponse, std::string> stopexportrecordeddata_;

};

///// Methods /////

RecordingClient::RecordingClient(const boost::shared_ptr<std::recursive_mutex>& mutex) :
  Client(mutex),
  signals_(new RecordingSignals(
  {
    Signal<RECORDINGOPERATION, RecordingClient, CreateRecordingResponse, RecordingConfiguration>(this, RECORDINGOPERATION_CREATERECORDING, true, std::string("http://www.onvif.org/ver10/recording/wsdl/CreateRecording"), false),
    Signal<RECORDINGOPERATION, RecordingClient, CreateRecordingJobResponse, RecordingJobConfiguration>(this, RECORDINGOPERATION_CREATERECORDINGJOB, true, std::string("http://www.onvif.org/ver10/recording/wsdl/CreateRecordingJob"), false),
    Signal<RECORDINGOPERATION, RecordingClient, CreateTrackResponse, std::string, TrackConfiguration>(this, RECORDINGOPERATION_CREATETRACK, true, std::string("http://www.onvif.org/ver10/recording/wsdl/CreateTrack"), false),
    Signal<RECORDINGOPERATION, RecordingClient, DeleteRecordingResponse, std::string>(this, RECORDINGOPERATION_DELETERECORDING, true, std::string("http://www.onvif.org/ver10/recording/wsdl/DeleteRecording"), false),
    Signal<RECORDINGOPERATION, RecordingClient, DeleteRecordingJobResponse, std::string>(this, RECORDINGOPERATION_DELETERECORDINGJOB, true, std::string("http://www.onvif.org/ver10/recording/wsdl/DeleteRecordingJob"), false),
    Signal<RECORDINGOPERATION, RecordingClient, DeleteTrackResponse, std::string, std::string>(this, RECORDINGOPERATION_DELETETRACK, true, std::string("http://www.onvif.org/ver10/recording/wsdl/DeleteTrack"), false),
    Signal<RECORDINGOPERATION, RecordingClient, ExportRecordedDataResponse, ws::DateTime, ws::DateTime, SearchScope, std::string, StorageReferencePath>(this, RECORDINGOPERATION_EXPORTRECORDEDDATA, true, std::string("http://www.onvif.org/ver10/recording/wsdl/ExportRecordedData"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetExportRecordedDataStateResponse, std::string>(this, RECORDINGOPERATION_GETEXPORTRECORDEDDATASTATE, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetExportRecordedDataState"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetRecordingConfigurationResponse, std::string>(this, RECORDINGOPERATION_GETRECORDINGCONFIGURATION, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetRecordingConfiguration"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetRecordingJobConfigurationResponse, std::string>(this, RECORDINGOPERATION_GETRECORDINGJOBCONFIGURATION, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetRecordingJobConfiguration"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetRecordingJobsResponse>(this, RECORDINGOPERATION_GETRECORDINGJOBS, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetRecordingJobs"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetRecordingJobStateResponse, std::string>(this, RECORDINGOPERATION_GETRECORDINGJOBSTATE, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetRecordingJobState"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetRecordingOptionsResponse, std::string>(this, RECORDINGOPERATION_GETRECORDINGOPTIONS, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetRecordingOptions"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetRecordingsResponse>(this, RECORDINGOPERATION_GETRECORDINGS, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetRecordings"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetServiceCapabilitiesResponse>(this, RECORDINGOPERATION_GETSERVICECAPABILITIES, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetServiceCapabilities"), false),
    Signal<RECORDINGOPERATION, RecordingClient, GetTrackConfigurationResponse, std::string, std::string>(this, RECORDINGOPERATION_GETTRACKCONFIGURATION, true, std::string("http://www.onvif.org/ver10/recording/wsdl/GetTrackConfiguration"), false),
    Signal<RECORDINGOPERATION, RecordingClient, SetRecordingConfigurationResponse, std::string, RecordingConfiguration>(this, RECORDINGOPERATION_SETRECORDINGCONFIGURATION, true, std::string("http://www.onvif.org/ver10/recording/wsdl/SetRecordingConfiguration"), false),
    Signal<RECORDINGOPERATION, RecordingClient, SetRecordingJobConfigurationResponse, std::string, RecordingJobConfiguration>(this, RECORDINGOPERATION_SETRECORDINGJOBCONFIGURATION, true, std::string("http://www.onvif.org/ver10/recording/wsdl/SetRecordingJobConfiguration"), false),
    Signal<RECORDINGOPERATION, RecordingClient, SetRecordingJobModeResponse, std::string, RECORDINGJOBMODE>(this, RECORDINGOPERATION_SETRECORDINGJOBMODE, true, std::string("http://www.onvif.org/ver10/recording/wsdl/SetRecordingJobMode"), false),
    Signal<RECORDINGOPERATION, RecordingClient, SetTrackConfigurationResponse, std::string, std::string, TrackConfiguration>(this, RECORDINGOPERATION_SETTRACKCONFIGURATION, true, std::string("http://www.onvif.org/ver10/recording/wsdl/SetTrackConfiguration"), false),
    Signal<RECORDINGOPERATION, RecordingClient, StopExportRecordedDataResponse, std::string>(this, RECORDINGOPERATION_STOPEXPORTRECORDEDDATA, true, std::string("http://www.onvif.org/ver10/recording/wsdl/StopExportRecordedData"), false)
  }))
{

}

RecordingClient::~RecordingClient()
{
  if (signals_)
  {
    delete signals_;
    signals_ = nullptr;
  }
}

void RecordingClient::Destroy()
{
  Client::Destroy();

  signals_->createrecording_.Destroy();
  signals_->createrecordingjob_.Destroy();
  signals_->createtrack_.Destroy();
  signals_->deleterecording_.Destroy();
  signals_->deleterecordingjob_.Destroy();
  signals_->deletetrack_.Destroy();
  signals_->exportrecordeddata_.Destroy();
  signals_->getexportrecordeddatastate_.Destroy();
  signals_->getrecordingconfiguration_.Destroy();
  signals_->getrecordingjobconfiguration_.Destroy();
  signals_->getrecordingjobs_.Destroy();
  signals_->getrecordingjobstate_.Destroy();
  signals_->getrecordingoptions_.Destroy();
  signals_->getrecordings_.Destroy();
  signals_->getservicecapabilities_.Destroy();
  signals_->gettrackconfiguration_.Destroy();
  signals_->setrecordingconfiguration_.Destroy();
  signals_->setrecordingjobconfiguration_.Destroy();
  signals_->setrecordingjobmode_.Destroy();
  signals_->settrackconfiguration_.Destroy();
  signals_->stopexportrecordeddata_.Destroy();
}

// Requests
void RecordingClient::CreateRecording(const RecordingConfiguration& recordingconfiguration)
{
  signals_->createrecording_.Create(CreateRecordingBody(recordingconfiguration), recordingconfiguration);
}

void RecordingClient::CreateRecordingJob(const RecordingJobConfiguration& jobconfiguration)
{
  signals_->createrecordingjob_.Create(CreateRecordingJobBody(jobconfiguration), jobconfiguration);
}

void RecordingClient::CreateTrack(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration)
{
  signals_->createtrack_.Create(CreateTrackBody(recordingtoken, trackconfiguration), recordingtoken, trackconfiguration);
}

void RecordingClient::DeleteRecording(const std::string& recordingtoken)
{
  signals_->deleterecording_.Create(DeleteRecordingBody(recordingtoken), recordingtoken);
}

void RecordingClient::DeleteRecordingJob(const std::string& jobtoken)
{
  signals_->deleterecordingjob_.Create(DeleteRecordingJobBody(jobtoken), jobtoken);
}

void RecordingClient::DeleteTrack(const std::string& recordingtoken, const std::string& tracktoken)
{
  signals_->deletetrack_.Create(DeleteTrackBody(recordingtoken, tracktoken), recordingtoken, tracktoken);
}

void RecordingClient::ExportRecordedData(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination)
{
  signals_->exportrecordeddata_.Create(ExportRecordedDataBody(startpoint, endpoint, searchscope, fileformat, storagedestination), startpoint, endpoint, searchscope, fileformat, storagedestination);
}

void RecordingClient::GetExportRecordedDataState(const std::string& operationtoken)
{
  signals_->getexportrecordeddatastate_.Create(GetExportRecordedDataStateBody(operationtoken), operationtoken);
}

void RecordingClient::GetRecordingConfiguration(const std::string& recordingtoken)
{
  signals_->getrecordingconfiguration_.Create(GetRecordingConfigurationBody(recordingtoken), recordingtoken);
}

void RecordingClient::GetRecordingJobConfiguration(const std::string& jobtoken)
{
  signals_->getrecordingjobconfiguration_.Create(GetRecordingJobConfigurationBody(jobtoken), jobtoken);
}

void RecordingClient::GetRecordingJobs()
{
  signals_->getrecordingjobs_.Create(GetRecordingJobsBody());
}

void RecordingClient::GetRecordingJobState(const std::string& jobtoken)
{
  signals_->getrecordingjobstate_.Create(GetRecordingJobStateBody(jobtoken), jobtoken);
}

void RecordingClient::GetRecordingOptions(const std::string& recordingtoken)
{
  signals_->getrecordingoptions_.Create(GetRecordingOptionsBody(recordingtoken), recordingtoken);
}

void RecordingClient::GetRecordings()
{
  signals_->getrecordings_.Create(GetRecordingsBody());
}

void RecordingClient::GetServiceCapabilities()
{
  signals_->getservicecapabilities_.Create(GetServiceCapabilitiesBody());
}

void RecordingClient::GetTrackConfiguration(const std::string& recordingtoken, const std::string& tracktoken)
{
  signals_->gettrackconfiguration_.Create(GetTrackConfigurationBody(recordingtoken, tracktoken), recordingtoken, tracktoken);
}

void RecordingClient::SetRecordingConfiguration(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration)
{
  signals_->setrecordingconfiguration_.Create(SetRecordingConfigurationBody(recordingtoken, recordingconfiguration), recordingtoken, recordingconfiguration);
}

void RecordingClient::SetRecordingJobConfiguration(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration)
{
  signals_->setrecordingjobconfiguration_.Create(SetRecordingJobConfigurationBody(jobtoken, jobconfiguration), jobtoken, jobconfiguration);
}

void RecordingClient::SetRecordingJobMode(const std::string& jobtoken, const RECORDINGJOBMODE mode)
{
  signals_->setrecordingjobmode_.Create(SetRecordingJobModeBody(jobtoken, mode), jobtoken, mode);
}

void RecordingClient::SetTrackConfiguration(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration)
{
  signals_->settrackconfiguration_.Create(SetTrackConfigurationBody(recordingtoken, tracktoken, trackconfiguration), recordingtoken, tracktoken, trackconfiguration);
}

void RecordingClient::StopExportRecordedData(const std::string& operationtoken)
{
  signals_->stopexportrecordeddata_.Create(StopExportRecordedDataBody(operationtoken), operationtoken);
}

// Callbacks
Connection RecordingClient::CreateRecordingCallback(const RecordingConfiguration& recordingconfiguration, boost::function<void(const CreateRecordingResponse&)> callback)
{
  return signals_->createrecording_.CreateCallback(CreateRecordingBody(recordingconfiguration), callback, recordingconfiguration);
}

Connection RecordingClient::CreateRecordingJobCallback(const RecordingJobConfiguration& jobconfiguration, boost::function<void(const CreateRecordingJobResponse&)> callback)
{
  return signals_->createrecordingjob_.CreateCallback(CreateRecordingJobBody(jobconfiguration), callback, jobconfiguration);
}

Connection RecordingClient::CreateTrackCallback(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration, boost::function<void(const CreateTrackResponse&)> callback)
{
  return signals_->createtrack_.CreateCallback(CreateTrackBody(recordingtoken, trackconfiguration), callback, recordingtoken, trackconfiguration);
}

Connection RecordingClient::DeleteRecordingCallback(const std::string& recordingtoken, boost::function<void(const DeleteRecordingResponse&)> callback)
{
  return signals_->deleterecording_.CreateCallback(DeleteRecordingBody(recordingtoken), callback, recordingtoken);
}

Connection RecordingClient::DeleteRecordingJobCallback(const std::string& jobtoken, boost::function<void(const DeleteRecordingJobResponse&)> callback)
{
  return signals_->deleterecordingjob_.CreateCallback(DeleteRecordingJobBody(jobtoken), callback, jobtoken);
}

Connection RecordingClient::DeleteTrackCallback(const std::string& recordingtoken, const std::string& tracktoken, boost::function<void(const DeleteTrackResponse&)> callback)
{
  return signals_->deletetrack_.CreateCallback(DeleteTrackBody(recordingtoken, tracktoken), callback, recordingtoken, tracktoken);
}

Connection RecordingClient::ExportRecordedDataCallback(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination, boost::function<void(const ExportRecordedDataResponse&)> callback)
{
  return signals_->exportrecordeddata_.CreateCallback(ExportRecordedDataBody(startpoint, endpoint, searchscope, fileformat, storagedestination), callback, startpoint, endpoint, searchscope, fileformat, storagedestination);
}

Connection RecordingClient::GetExportRecordedDataStateCallback(const std::string& operationtoken, boost::function<void(const GetExportRecordedDataStateResponse&)> callback)
{
  return signals_->getexportrecordeddatastate_.CreateCallback(GetExportRecordedDataStateBody(operationtoken), callback, operationtoken);
}

Connection RecordingClient::GetRecordingConfigurationCallback(const std::string& recordingtoken, boost::function<void(const GetRecordingConfigurationResponse&)> callback)
{
  return signals_->getrecordingconfiguration_.CreateCallback(GetRecordingConfigurationBody(recordingtoken), callback, recordingtoken);
}

Connection RecordingClient::GetRecordingJobConfigurationCallback(const std::string& jobtoken, boost::function<void(const GetRecordingJobConfigurationResponse&)> callback)
{
  return signals_->getrecordingjobconfiguration_.CreateCallback(GetRecordingJobConfigurationBody(jobtoken), callback, jobtoken);
}

Connection RecordingClient::GetRecordingJobsCallback(boost::function<void(const GetRecordingJobsResponse&)> callback)
{
  return signals_->getrecordingjobs_.CreateCallback(GetRecordingJobsBody(), callback);
}

Connection RecordingClient::GetRecordingJobStateCallback(const std::string& jobtoken, boost::function<void(const GetRecordingJobStateResponse&)> callback)
{
  return signals_->getrecordingjobstate_.CreateCallback(GetRecordingJobStateBody(jobtoken), callback, jobtoken);
}

Connection RecordingClient::GetRecordingOptionsCallback(const std::string& recordingtoken, boost::function<void(const GetRecordingOptionsResponse&)> callback)
{
  return signals_->getrecordingoptions_.CreateCallback(GetRecordingOptionsBody(recordingtoken), callback, recordingtoken);
}

Connection RecordingClient::GetRecordingsCallback(boost::function<void(const GetRecordingsResponse&)> callback)
{
  return signals_->getrecordings_.CreateCallback(GetRecordingsBody(), callback);
}

Connection RecordingClient::GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback)
{
  return signals_->getservicecapabilities_.CreateCallback(GetServiceCapabilitiesBody(), callback);
}

Connection RecordingClient::GetTrackConfigurationCallback(const std::string& recordingtoken, const std::string& tracktoken, boost::function<void(const GetTrackConfigurationResponse&)> callback)
{
  return signals_->gettrackconfiguration_.CreateCallback(GetTrackConfigurationBody(recordingtoken, tracktoken), callback, recordingtoken, tracktoken);
}

Connection RecordingClient::SetRecordingConfigurationCallback(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration, boost::function<void(const SetRecordingConfigurationResponse&)> callback)
{
  return signals_->setrecordingconfiguration_.CreateCallback(SetRecordingConfigurationBody(recordingtoken, recordingconfiguration), callback, recordingtoken, recordingconfiguration);
}

Connection RecordingClient::SetRecordingJobConfigurationCallback(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration, boost::function<void(const SetRecordingJobConfigurationResponse&)> callback)
{
  return signals_->setrecordingjobconfiguration_.CreateCallback(SetRecordingJobConfigurationBody(jobtoken, jobconfiguration), callback, jobtoken, jobconfiguration);
}

Connection RecordingClient::SetRecordingJobModeCallback(const std::string& jobtoken, const RECORDINGJOBMODE mode, boost::function<void(const SetRecordingJobModeResponse&)> callback)
{
  return signals_->setrecordingjobmode_.CreateCallback(SetRecordingJobModeBody(jobtoken, mode), callback, jobtoken, mode);
}

Connection RecordingClient::SetTrackConfigurationCallback(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration, boost::function<void(const SetTrackConfigurationResponse&)> callback)
{
  return signals_->settrackconfiguration_.CreateCallback(SetTrackConfigurationBody(recordingtoken, tracktoken, trackconfiguration), callback, recordingtoken, tracktoken, trackconfiguration);
}

Connection RecordingClient::StopExportRecordedDataCallback(const std::string& operationtoken, boost::function<void(const StopExportRecordedDataResponse&)> callback)
{
  return signals_->stopexportrecordeddata_.CreateCallback(StopExportRecordedDataBody(operationtoken), callback, operationtoken);
}

// Futures
boost::unique_future<CreateRecordingResponse> RecordingClient::CreateRecordingFuture(const RecordingConfiguration& recordingconfiguration)
{
  return signals_->createrecording_.CreateFuture(CreateRecordingBody(recordingconfiguration), recordingconfiguration);
}

boost::unique_future<CreateRecordingJobResponse> RecordingClient::CreateRecordingJobFuture(const RecordingJobConfiguration& jobconfiguration)
{
  return signals_->createrecordingjob_.CreateFuture(CreateRecordingJobBody(jobconfiguration), jobconfiguration);
}

boost::unique_future<CreateTrackResponse> RecordingClient::CreateTrackFuture(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration)
{
  return signals_->createtrack_.CreateFuture(CreateTrackBody(recordingtoken, trackconfiguration), recordingtoken, trackconfiguration);
}

boost::unique_future<DeleteRecordingResponse> RecordingClient::DeleteRecordingFuture(const std::string& recordingtoken)
{
  return signals_->deleterecording_.CreateFuture(DeleteRecordingBody(recordingtoken), recordingtoken);
}

boost::unique_future<DeleteRecordingJobResponse> RecordingClient::DeleteRecordingJobFuture(const std::string& jobtoken)
{
  return signals_->deleterecordingjob_.CreateFuture(DeleteRecordingJobBody(jobtoken), jobtoken);
}

boost::unique_future<DeleteTrackResponse> RecordingClient::DeleteTrackFuture(const std::string& recordingtoken, const std::string& tracktoken)
{
  return signals_->deletetrack_.CreateFuture(DeleteTrackBody(recordingtoken, tracktoken), recordingtoken, tracktoken);
}

boost::unique_future<ExportRecordedDataResponse> RecordingClient::ExportRecordedDataFuture(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination)
{
  return signals_->exportrecordeddata_.CreateFuture(ExportRecordedDataBody(startpoint, endpoint, searchscope, fileformat, storagedestination), startpoint, endpoint, searchscope, fileformat, storagedestination);
}


boost::unique_future<GetExportRecordedDataStateResponse> RecordingClient::GetExportRecordedDataStateFuture(const std::string& operationtoken)
{
  return signals_->getexportrecordeddatastate_.CreateFuture(GetExportRecordedDataStateBody(operationtoken), operationtoken);
}

boost::unique_future<GetRecordingConfigurationResponse> RecordingClient::GetRecordingConfigurationFuture(const std::string& recordingtoken)
{
  return signals_->getrecordingconfiguration_.CreateFuture(GetRecordingConfigurationBody(recordingtoken), recordingtoken);
}

boost::unique_future<GetRecordingJobConfigurationResponse> RecordingClient::GetRecordingJobConfigurationFuture(const std::string& jobtoken)
{
  return signals_->getrecordingjobconfiguration_.CreateFuture(GetRecordingJobConfigurationBody(jobtoken), jobtoken);
}

boost::unique_future<GetRecordingJobsResponse> RecordingClient::GetRecordingJobsFuture()
{
  return signals_->getrecordingjobs_.CreateFuture(GetRecordingJobsBody());
}

boost::unique_future<GetRecordingJobStateResponse> RecordingClient::GetRecordingJobStateFuture(const std::string& jobtoken)
{
  return signals_->getrecordingjobstate_.CreateFuture(GetRecordingJobStateBody(jobtoken), jobtoken);
}

boost::unique_future<GetRecordingOptionsResponse> RecordingClient::GetRecordingOptionsFuture(const std::string& recordingtoken)
{
  return signals_->getrecordingoptions_.CreateFuture(GetRecordingOptionsBody(recordingtoken), recordingtoken);
}

boost::unique_future<GetRecordingsResponse> RecordingClient::GetRecordingsFuture()
{
  return signals_->getrecordings_.CreateFuture(GetRecordingsBody());
}

boost::unique_future<GetServiceCapabilitiesResponse> RecordingClient::GetServiceCapabilitiesFuture()
{
  return signals_->getservicecapabilities_.CreateFuture(GetServiceCapabilitiesBody());
}

boost::unique_future<GetTrackConfigurationResponse> RecordingClient::GetTrackConfigurationFuture(const std::string& recordingtoken, const std::string& tracktoken)
{
  return signals_->gettrackconfiguration_.CreateFuture(GetTrackConfigurationBody(recordingtoken, tracktoken), recordingtoken, tracktoken);
}

boost::unique_future<SetRecordingConfigurationResponse> RecordingClient::SetRecordingConfigurationFuture(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration)
{
  return signals_->setrecordingconfiguration_.CreateFuture(SetRecordingConfigurationBody(recordingtoken, recordingconfiguration), recordingtoken, recordingconfiguration);
}

boost::unique_future<SetRecordingJobConfigurationResponse> RecordingClient::SetRecordingJobConfigurationFuture(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration)
{
  return signals_->setrecordingjobconfiguration_.CreateFuture(SetRecordingJobConfigurationBody(jobtoken, jobconfiguration), jobtoken, jobconfiguration);
}

boost::unique_future<SetRecordingJobModeResponse> RecordingClient::SetRecordingJobModeFuture(const std::string& jobtoken, const RECORDINGJOBMODE mode)
{
  return signals_->setrecordingjobmode_.CreateFuture(SetRecordingJobModeBody(jobtoken, mode), jobtoken, mode);
}

boost::unique_future<SetTrackConfigurationResponse> RecordingClient::SetTrackConfigurationFuture(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration)
{
  return signals_->settrackconfiguration_.CreateFuture(SetTrackConfigurationBody(recordingtoken, tracktoken, trackconfiguration), recordingtoken, tracktoken, trackconfiguration);
}

boost::unique_future<StopExportRecordedDataResponse> RecordingClient::StopExportRecordedDataFuture(const std::string& operationtoken)
{
  return signals_->stopexportrecordeddata_.CreateFuture(StopExportRecordedDataBody(operationtoken), operationtoken);
}

void RecordingClient::Update(RECORDINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata)
{
  switch (operation)
  {
    case RECORDINGOPERATION_CREATERECORDING:
    {
      auto createrecordingresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateRecordingResponse']");
      if (!createrecordingresponse)
      {
        SignalError(RECORDINGOPERATION_CREATERECORDING, handle, localendpoint, latency, std::string("/Envelope/Body/CreateRecordingResponse element not found"));
        break;
      }
      
      signals_->createrecording_.Emit(handle, localendpoint, latency, std::string(), GetText(createrecordingresponse, "*[local-name()='RecordingToken']"));
      break;
    }
    case RECORDINGOPERATION_CREATERECORDINGJOB:
    {
      auto createrecordingjobresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateRecordingJobResponse']");
      if (!createrecordingjobresponse)
      {
        SignalError(RECORDINGOPERATION_CREATERECORDINGJOB, handle, localendpoint, latency, std::string("/Envelope/Body/CreateRecordingJobResponse element not found"));
        break;
      }
      
      signals_->createrecordingjob_.Emit(handle, localendpoint, latency, std::string(), GetText(createrecordingjobresponse, "*[local-name()='JobToken']"), GetClass<RecordingJobConfiguration>(createrecordingjobresponse, "*[local-name()='JobConfiguration']"));
      break;
    }
    case RECORDINGOPERATION_CREATETRACK:
    {
      auto createtrackresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='CreateTrackResponse']");
      if (!createtrackresponse)
      {
        SignalError(RECORDINGOPERATION_CREATETRACK, handle, localendpoint, latency, std::string("/Envelope/Body/CreateTrackResponse element not found"));
        break;
      }
      
      signals_->createtrack_.Emit(handle, localendpoint, latency, std::string(), GetText(createtrackresponse, "*[local-name()='TrackToken']"));
      break;
    }
    case RECORDINGOPERATION_DELETERECORDING:
    {
      auto deleterecordingresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteRecordingResponse']");
      if (!deleterecordingresponse)
      {
        SignalError(RECORDINGOPERATION_DELETERECORDING, handle, localendpoint, latency, std::string("/Envelope/Body/DeleteRecordingResponse element not found"));
        break;
      }
      
      signals_->deleterecording_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECORDINGOPERATION_DELETERECORDINGJOB:
    {
      auto deleterecordingjobresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteRecordingJobResponse']");
      if (!deleterecordingjobresponse)
      {
        SignalError(RECORDINGOPERATION_DELETERECORDINGJOB, handle, localendpoint, latency, std::string("/Envelope/Body/DeleteRecordingJobResponse element not found"));
        break;
      }
      
      signals_->deleterecordingjob_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECORDINGOPERATION_DELETETRACK:
    {
      auto deletetrackresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='DeleteTrackResponse']");
      if (!deletetrackresponse)
      {
        SignalError(RECORDINGOPERATION_DELETETRACK, handle, localendpoint, latency, std::string("/Envelope/Body/DeleteTrackResponse element not found"));
        break;
      }
      
      signals_->deletetrack_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECORDINGOPERATION_EXPORTRECORDEDDATA:
    {
      auto exportrecordeddataresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='ExportRecordedDataResponse']");
      if (!exportrecordeddataresponse)
      {
        SignalError(RECORDINGOPERATION_EXPORTRECORDEDDATA, handle, localendpoint, latency, std::string("/Envelope/Body/ExportRecordedDataResponse element not found"));
        break;
      }
      
      std::vector<std::string> filenames;
      for (const auto& filename : exportrecordeddataresponse.node().select_nodes("*[local-name()='FileNames']"))
      {
        filenames.push_back(std::string(filename.node().text().get()));

      }

      signals_->exportrecordeddata_.Emit(handle, localendpoint, latency, std::string(), GetText(exportrecordeddataresponse, "*[local-name()='OperationToken']"), filenames);
      break;
    }
    case RECORDINGOPERATION_GETEXPORTRECORDEDDATASTATE:
    {
      auto getexportrecordeddatastateresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetExportRecordedDataStateResponse']");
      if (!getexportrecordeddatastateresponse)
      {
        SignalError(RECORDINGOPERATION_GETEXPORTRECORDEDDATASTATE, handle, localendpoint, latency, std::string("/Envelope/Body/GetExportRecordedDataStateResponse element not found"));
        break;
      }
      
      signals_->getexportrecordeddatastate_.Emit(handle, localendpoint, latency, std::string(), GetFloat(getexportrecordeddatastateresponse, "*[local-name()='Progress']"), GetClass<ArrayOfFileProgress>(getexportrecordeddatastateresponse, "*[local-name()='FileProgressStatus']"));
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGCONFIGURATION:
    {
      auto getrecordingconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingConfigurationResponse']");
      if (!getrecordingconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_GETRECORDINGCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetRecordingConfigurationResponse element not found"));
        break;
      }
      
      signals_->getrecordingconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<RecordingConfiguration>(getrecordingconfigurationresponse, "*[local-name()='RecordingConfiguration']"));
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGJOBCONFIGURATION:
    {
      auto getrecordingjobconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingJobConfigurationResponse']");
      if (!getrecordingjobconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_GETRECORDINGJOBCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetRecordingJobConfigurationResponse element not found"));
        break;
      }
      
      signals_->getrecordingjobconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<RecordingJobConfiguration>(getrecordingjobconfigurationresponse, "*[local-name()='JobConfiguration']"));
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGJOBS:
    {
      auto getrecordingjobsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingJobsResponse']");
      if (!getrecordingjobsresponse)
      {
        SignalError(RECORDINGOPERATION_GETRECORDINGJOBS, handle, localendpoint, latency, std::string("/Envelope/Body/GetRecordingJobsResponse element not found"));
        break;
      }
      
      std::vector<GetRecordingJobsResponseItem> jobitems;
      for (const auto& jobitem : getrecordingjobsresponse.node().select_nodes("*[local-name()='JobItem']"))
      {
        jobitems.push_back(GetRecordingJobsResponseItem(jobitem.node()));

      }

      signals_->getrecordingjobs_.Emit(handle, localendpoint, latency, std::string(), jobitems);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGJOBSTATE:
    {
      auto getrecordingjobstateresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingJobStateResponse']");
      if (!getrecordingjobstateresponse)
      {
        SignalError(RECORDINGOPERATION_GETRECORDINGJOBSTATE, handle, localendpoint, latency, std::string("/Envelope/Body/GetRecordingJobStateResponse element not found"));
        break;
      }
      
      signals_->getrecordingjobstate_.Emit(handle, localendpoint, latency, std::string(), GetClass<RecordingJobStateInformation>(getrecordingjobstateresponse, "*[local-name()='State']"));
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGOPTIONS:
    {
      auto getrecordingoptionsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingOptionsResponse']");
      if (!getrecordingoptionsresponse)
      {
        SignalError(RECORDINGOPERATION_GETRECORDINGOPTIONS, handle, localendpoint, latency, std::string("/Envelope/Body/GetRecordingJobStateResponse element not found"));
        break;
      }
      
      signals_->getrecordingoptions_.Emit(handle, localendpoint, latency, std::string(), GetClass<RecordingOptions>(getrecordingoptionsresponse, "*[local-name()='Options']"));
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGS:
    {
      auto getrecordingsresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetRecordingsResponse']");
      if (!getrecordingsresponse)
      {
        SignalError(RECORDINGOPERATION_GETRECORDINGS, handle, localendpoint, latency, std::string("/Envelope/Body/GetRecordingsResponse element not found"));
        break;
      }
      
      std::vector<GetRecordingsResponseItem> recordingitems;
      for (const auto& recordingitem : getrecordingsresponse.node().select_nodes("*[local-name()='RecordingItem']"))
      {
        recordingitems.push_back(GetRecordingsResponseItem(recordingitem.node()));

      }

      signals_->getrecordings_.Emit(handle, localendpoint, latency, std::string(), recordingitems);
      break;
    }
    case RECORDINGOPERATION_GETSERVICECAPABILITIES:
    {
      auto getservicecapabilitiesresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetServiceCapabilitiesResponse']");
      if (!getservicecapabilitiesresponse)
      {
        SignalError(RECORDINGOPERATION_GETSERVICECAPABILITIES, handle, localendpoint, latency, std::string("/Envelope/Body/GetServiceCapabilitiesResponse element not found"));
        break;
      }
      
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, std::string(), GetClass<Capabilities>(getservicecapabilitiesresponse, "*[local-name()='Capabilities']"));
      break;
    }
    case RECORDINGOPERATION_GETTRACKCONFIGURATION:
    {
      auto gettrackconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='GetTrackConfigurationResponse']");
      if (!gettrackconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_GETTRACKCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/GetTrackConfigurationResponse element not found"));
        break;
      }
      
      signals_->gettrackconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<TrackConfiguration>(gettrackconfigurationresponse, "*[local-name()='TrackConfiguration']"));
      break;
    }
    case RECORDINGOPERATION_SETRECORDINGCONFIGURATION:
    {
      auto setrecordingconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRecordingConfigurationResponse']");
      if (!setrecordingconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_SETRECORDINGCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetRecordingConfigurationResponse element not found"));
        break;
      }
      
      signals_->setrecordingconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECORDINGOPERATION_SETRECORDINGJOBCONFIGURATION:
    {
      auto setrecordingjobconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRecordingJobConfigurationResponse']");
      if (!setrecordingjobconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_SETRECORDINGJOBCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetRecordingJobConfigurationResponse element not found"));
        break;
      }
      
      signals_->setrecordingjobconfiguration_.Emit(handle, localendpoint, latency, std::string(), GetClass<RecordingJobConfiguration>(setrecordingjobconfigurationresponse, "*[local-name()='JobConfiguration']"));
      break;
    }
    case RECORDINGOPERATION_SETRECORDINGJOBMODE:
    {
      auto setrecordingconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetRecordingJobModeResponse']");
      if (!setrecordingconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_SETRECORDINGJOBMODE, handle, localendpoint, latency, std::string("/Envelope/Body/SetRecordingJobModeResponse element not found"));
        break;
      }
      
      signals_->setrecordingjobmode_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECORDINGOPERATION_SETTRACKCONFIGURATION:
    {
      auto settrackconfigurationresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='SetTrackConfigurationResponse']");
      if (!settrackconfigurationresponse)
      {
        SignalError(RECORDINGOPERATION_SETTRACKCONFIGURATION, handle, localendpoint, latency, std::string("/Envelope/Body/SetTrackConfigurationResponse element not found"));
        break;
      }
      
      signals_->settrackconfiguration_.Emit(handle, localendpoint, latency, std::string());
      break;
    }
    case RECORDINGOPERATION_STOPEXPORTRECORDEDDATA:
    {
      auto stopexportrecordeddataresponse = document.select_node("/*[local-name()='Envelope']/*[local-name()='Body']/*[local-name()='StopExportRecordedDataResponse']");
      if (!stopexportrecordeddataresponse)
      {
        SignalError(RECORDINGOPERATION_STOPEXPORTRECORDEDDATA, handle, localendpoint, latency, std::string("/Envelope/Body/StopExportRecordedDataResponse element not found"));
        break;
      }
      
      signals_->stopexportrecordeddata_.Emit(handle, localendpoint, latency, std::string(), GetFloat(stopexportrecordeddataresponse, "*[local-name()='Progress']"), GetClass<ArrayOfFileProgress>(stopexportrecordeddataresponse, "*[local-name()='FileProgressStatus']"));
      break;
    }
    default:
    {

      break;
    }
  }
}

// Signals
boost::signals2::signal<void(const CreateRecordingResponse&)>& RecordingClient::CreateRecordingSignal()
{
  return signals_->createrecording_.GetSignal();
}

boost::signals2::signal<void(const CreateRecordingJobResponse&)>& RecordingClient::CreateRecordingJobSignal()
{
  return signals_->createrecordingjob_.GetSignal();
}

boost::signals2::signal<void(const CreateTrackResponse&)>& RecordingClient::CreateTrackSignal()
{
  return signals_->createtrack_.GetSignal();
}

boost::signals2::signal<void(const DeleteRecordingResponse&)>& RecordingClient::DeleteRecordingSignal()
{
  return signals_->deleterecording_.GetSignal();
}

boost::signals2::signal<void(const DeleteRecordingJobResponse&)>& RecordingClient::DeleteRecordingJobSignal()
{
  return signals_->deleterecordingjob_.GetSignal();
}

boost::signals2::signal<void(const DeleteTrackResponse&)>& RecordingClient::DeleteTrackSignal()
{
  return signals_->deletetrack_.GetSignal();
}

boost::signals2::signal<void(const ExportRecordedDataResponse&)>& RecordingClient::ExportRecordedDataSignal()
{
  return signals_->exportrecordeddata_.GetSignal();
}

boost::signals2::signal<void(const GetExportRecordedDataStateResponse&)>& RecordingClient::GetExportRecordedDataStateSignal()
{
  return signals_->getexportrecordeddatastate_.GetSignal();
}

boost::signals2::signal<void(const GetRecordingConfigurationResponse&)>& RecordingClient::GetRecordingConfigurationSignal()
{
  return signals_->getrecordingconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetRecordingJobConfigurationResponse&)>& RecordingClient::GetRecordingJobConfigurationSignal()
{
  return signals_->getrecordingjobconfiguration_.GetSignal();
}

boost::signals2::signal<void(const GetRecordingJobsResponse&)>& RecordingClient::GetRecordingJobsSignal()
{
  return signals_->getrecordingjobs_.GetSignal();
}

boost::signals2::signal<void(const GetRecordingJobStateResponse&)>& RecordingClient::GetRecordingJobStateSignal()
{
  return signals_->getrecordingjobstate_.GetSignal();
}

boost::signals2::signal<void(const GetRecordingOptionsResponse&)>& RecordingClient::GetRecordingOptionsSignal()
{
  return signals_->getrecordingoptions_.GetSignal();
}

boost::signals2::signal<void(const GetRecordingsResponse&)>& RecordingClient::GetRecordingsSignal()
{
  return signals_->getrecordings_.GetSignal();
}

boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& RecordingClient::GetServiceCapabilitiesSignal()
{
  return signals_->getservicecapabilities_.GetSignal();
}

boost::signals2::signal<void(const GetTrackConfigurationResponse&)>& RecordingClient::GetTrackConfigurationSignal()
{
  return signals_->gettrackconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SetRecordingConfigurationResponse&)>& RecordingClient::SetRecordingConfigurationSignal()
{
  return signals_->setrecordingconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SetRecordingJobConfigurationResponse&)>& RecordingClient::SetRecordingJobConfigurationSignal()
{
  return signals_->setrecordingjobconfiguration_.GetSignal();
}

boost::signals2::signal<void(const SetRecordingJobModeResponse&)>& RecordingClient::SetRecordingJobModeSignal()
{
  return signals_->setrecordingjobmode_.GetSignal();
}

boost::signals2::signal<void(const SetTrackConfigurationResponse&)>& RecordingClient::SetTrackConfigurationSignal()
{
  return signals_->settrackconfiguration_.GetSignal();
}

boost::signals2::signal<void(const StopExportRecordedDataResponse&)>& RecordingClient::StopExportRecordedDataSignal()
{
  return signals_->stopexportrecordeddata_.GetSignal();
}

void RecordingClient::SignalError(RECORDINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message)
{
  switch (operation)
  {
    case RECORDINGOPERATION_CREATERECORDING:
    {
      signals_->createrecording_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_CREATERECORDINGJOB:
    {
      signals_->createrecordingjob_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_CREATETRACK:
    {
      signals_->createtrack_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_DELETERECORDING:
    {
      signals_->deleterecording_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_DELETERECORDINGJOB:
    {
      signals_->deleterecordingjob_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_DELETETRACK:
    {
      signals_->deletetrack_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_EXPORTRECORDEDDATA:
    {
      signals_->exportrecordeddata_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETEXPORTRECORDEDDATASTATE:
    {
      signals_->getexportrecordeddatastate_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGCONFIGURATION:
    {
      signals_->getrecordingconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGJOBCONFIGURATION:
    {
      signals_->getrecordingjobconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGJOBS:
    {
      signals_->getrecordingjobs_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGJOBSTATE:
    {
      signals_->getrecordingjobstate_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGOPTIONS:
    {
      signals_->getrecordingoptions_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETRECORDINGS:
    {
      signals_->getrecordings_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETSERVICECAPABILITIES:
    {
      signals_->getservicecapabilities_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_GETTRACKCONFIGURATION:
    {
      signals_->gettrackconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_SETRECORDINGCONFIGURATION:
    {
      signals_->setrecordingconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_SETRECORDINGJOBCONFIGURATION:
    {
      signals_->setrecordingjobconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_SETRECORDINGJOBMODE:
    {
      signals_->setrecordingjobmode_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_SETTRACKCONFIGURATION:
    {
      signals_->settrackconfiguration_.Emit(handle, localendpoint, latency, message);
      break;
    }
    case RECORDINGOPERATION_STOPEXPORTRECORDEDDATA:
    {
      signals_->stopexportrecordeddata_.Emit(handle, localendpoint, latency, message);
      break;
    }
    default:
    {
       assert(false);
       break;
    }
  }
}

std::string RecordingClient::CreateRecordingBody(const RecordingConfiguration& recordingconfiguration)
{
  return std::string("<s:Body><trc:CreateRecording>") + recordingconfiguration.ToXml("trc:RecordingConfiguration") + std::string("</trc:CreateRecording></s:Body>");
}

std::string RecordingClient::CreateRecordingJobBody(const RecordingJobConfiguration& jobconfiguration)
{
  return std::string("<s:Body><trc:CreateRecordingJob>") + jobconfiguration.ToXml("trc:JobConfiguration") + std::string("</trc:CreateRecordingJob></s:Body>");
}

std::string RecordingClient::CreateTrackBody(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration)
{
  return std::string("<s:Body><trc:CreateTrack>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + trackconfiguration.ToXml("trc:TrackConfiguration") + std::string("</trc:CreateTrack></s:Body>");
}

std::string RecordingClient::DeleteRecordingBody(const std::string& recordingtoken)
{
  return std::string("<s:Body><trc:DeleteRecording>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + std::string("</trc:DeleteRecording></s:Body>");
}

std::string RecordingClient::DeleteRecordingJobBody(const std::string& jobtoken)
{
  return std::string("<s:Body><trc:DeleteRecordingJob>") + ToXml("trc:JobToken", boost::optional<std::string>(jobtoken)) + std::string("</trc:DeleteRecordingJob></s:Body>");
}

std::string RecordingClient::DeleteTrackBody(const std::string& recordingtoken, const std::string& tracktoken)
{
  return std::string("<s:Body><trc:DeleteTrack>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + ToXml("trc:TrackToken", boost::optional<std::string>(tracktoken)) + std::string("</trc:DeleteTrack></s:Body>");
}

std::string RecordingClient::ExportRecordedDataBody(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination)
{
  return std::string("<s:Body><trc:ExportRecordedData>") + startpoint.ToXml("trc:StartPoint") + endpoint.ToXml("trc:EndPoint") + searchscope.ToXml("trc:SearchScope")+ ToXml("trc:FileFormat", boost::optional<std::string>(fileformat)) + storagedestination.ToXml("trc:StorageDestination") + std::string("</trc:ExportRecordedData></s:Body>");
}

std::string RecordingClient::GetExportRecordedDataStateBody(const std::string& operationtoken)
{
  return std::string("<s:Body><trc:GetExportRecordedDataState>") + ToXml("trc:OperationToken", boost::optional<std::string>(operationtoken)) + std::string("</trc:GetExportRecordedDataState></s:Body>");
}

std::string RecordingClient::GetRecordingConfigurationBody(const std::string& recordingtoken)
{
  return std::string("<s:Body><trc:GetRecordingConfiguration>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken))+ std::string("</trc:GetRecordingConfiguration></s:Body>");
}

std::string RecordingClient::GetRecordingJobConfigurationBody(const std::string& jobtoken)
{
  return std::string("<s:Body><trc:GetRecordingJobConfiguration>") + ToXml("trc:JobToken", boost::optional<std::string>(jobtoken)) + std::string("</trc:GetRecordingJobConfiguration></s:Body>");
}

std::string RecordingClient::GetRecordingJobsBody()
{
  return std::string("<s:Body><trc:GetRecordingJobs/></s:Body>");
}

std::string RecordingClient::GetRecordingJobStateBody(const std::string& jobtoken)
{
  return std::string("<s:Body><trc:GetRecordingJobState>") + ToXml("trc:JobToken", boost::optional<std::string>(jobtoken)) + std::string("</trc:GetRecordingJobState></s:Body>");
}

std::string RecordingClient::GetRecordingOptionsBody(const std::string& recordingtoken)
{
  return std::string("<s:Body><trc:GetRecordingOptions>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + std::string("</trc:GetRecordingOptions></s:Body>");
}

std::string RecordingClient::GetRecordingsBody()
{
  return std::string("<s:Body><trc:GetRecordings/></s:Body>");
}

std::string RecordingClient::GetServiceCapabilitiesBody()
{
  return std::string("<s:Body><trc:GetServiceCapabilities/></s:Body>");
}

std::string RecordingClient::GetTrackConfigurationBody(const std::string& recordingtoken, const std::string& tracktoken)
{
  return std::string("<s:Body><trc:GetTrackConfiguration>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + ToXml("trc:TrackToken", boost::optional<std::string>(tracktoken)) + std::string("</trc:GetTrackConfiguration></s:Body>");
}

std::string RecordingClient::SetRecordingConfigurationBody(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration)
{
  return std::string("<s:Body><trc:SetRecordingConfiguration>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + recordingconfiguration.ToXml("trc:RecordingConfiguration") + std::string("</trc:SetRecordingConfiguration></s:Body>");
}

std::string RecordingClient::SetRecordingJobConfigurationBody(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration)
{
  return std::string("<s:Body><trc:SetRecordingJobConfiguration>") + ToXml("trc:JobToken", boost::optional<std::string>(jobtoken)) + jobconfiguration.ToXml("trc:JobConfiguration") + std::string("</trc:SetRecordingJobConfiguration></s:Body>");
}

std::string RecordingClient::SetRecordingJobModeBody(const std::string& jobtoken, const RECORDINGJOBMODE mode)
{
  return std::string("<s:Body><trc:SetRecordingJobMode>") + ToXml("trc:JobToken", boost::optional<std::string>(jobtoken)) + ToXmlEnum("trc:Mode", mode) + std::string("</trc:SetRecordingJobMode></s:Body>");
}

std::string RecordingClient::SetTrackConfigurationBody(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration)
{
  return std::string("<s:Body><trc:SetTrackConfiguration>") + ToXml("trc:RecordingToken", boost::optional<std::string>(recordingtoken)) + ToXml("trc:TrackToken", boost::optional<std::string>(tracktoken)) + trackconfiguration.ToXml("trc:TrackConfiguration") + std::string("</trc:SetTrackConfiguration></s:Body>");
}

std::string RecordingClient::StopExportRecordedDataBody(const std::string& operationtoken)
{
  return std::string("<s:Body><trc:StopExportRecordedData>") + ToXml("trc:OperationToken", boost::optional<std::string>(operationtoken)) + std::string("</trc:StopExportRecordedData></s:Body>");
}



}

}
