// recordingclient.hpp
//

#ifndef IDDM55F5DRDKJLC1DSNA4DA344JQ9W5ZCD
#define IDDM55F5DRDKJLC1DSNA4DA344JQ9W5ZCD

///// Includes /////

#include <curl/curl.h>
#include <onviftypes/onviftypes.hpp>

#include "client.hpp"
#include "response.hpp"
#include "signal.hpp"

///// Namespaces /////

namespace onvif
{

namespace recording
{

///// Enumerations /////

enum RECORDINGOPERATION
{
  RECORDINGOPERATION_CREATERECORDING,
  RECORDINGOPERATION_CREATERECORDINGJOB,
  RECORDINGOPERATION_CREATETRACK,
  RECORDINGOPERATION_DELETERECORDING,
  RECORDINGOPERATION_DELETERECORDINGJOB,
  RECORDINGOPERATION_DELETETRACK,
  RECORDINGOPERATION_EXPORTRECORDEDDATA,
  RECORDINGOPERATION_GETEXPORTRECORDEDDATASTATE,
  RECORDINGOPERATION_GETRECORDINGCONFIGURATION,
  RECORDINGOPERATION_GETRECORDINGJOBCONFIGURATION,
  RECORDINGOPERATION_GETRECORDINGJOBS,
  RECORDINGOPERATION_GETRECORDINGJOBSTATE,
  RECORDINGOPERATION_GETRECORDINGOPTIONS,
  RECORDINGOPERATION_GETRECORDINGS,
  RECORDINGOPERATION_GETSERVICECAPABILITIES,
  RECORDINGOPERATION_GETTRACKCONFIGURATION,
  RECORDINGOPERATION_SETRECORDINGCONFIGURATION,
  RECORDINGOPERATION_SETRECORDINGJOBCONFIGURATION,
  RECORDINGOPERATION_SETRECORDINGJOBMODE,
  RECORDINGOPERATION_SETTRACKCONFIGURATION,
  RECORDINGOPERATION_STOPEXPORTRECORDEDDATA
};

///// Declarations /////

class RecordingClient;
class RecordingSignals;

///// Classes /////

class CreateRecordingResponse : public Response<RecordingClient>
{
 public:

  CreateRecordingResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingConfiguration& recordingconfiguration);
  CreateRecordingResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingConfiguration& recordingconfiguration, const boost::optional<std::string>& recordingtoken);
  virtual ~CreateRecordingResponse();

  RecordingConfiguration recordingconfiguration_;
  boost::optional<std::string> recordingtoken_;

};

class CreateRecordingJobResponse : public Response<RecordingClient>
{
 public:

  CreateRecordingJobResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingJobConfiguration& jobconfiguration);
  CreateRecordingJobResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const RecordingJobConfiguration& jobconfiguration, const boost::optional<std::string>& jobtoken, const boost::optional<RecordingJobConfiguration>& jobconfigurationresponse);
  virtual ~CreateRecordingJobResponse();

  RecordingJobConfiguration jobconfiguration_;

  boost::optional<std::string> jobtoken_;
  boost::optional<RecordingJobConfiguration> jobconfigurationresponse_;

};

class CreateTrackResponse : public Response<RecordingClient>
{
 public:

  CreateTrackResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const TrackConfiguration& trackconfiguration);
  CreateTrackResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const TrackConfiguration& trackconfiguration, const boost::optional<std::string>& tracktoken);
  virtual ~CreateTrackResponse();

  std::string recordingtoken_;
  TrackConfiguration trackconfiguration_;

  boost::optional<std::string> tracktoken_;

};

class DeleteRecordingResponse : public Response<RecordingClient>
{
 public:

  DeleteRecordingResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken);
  virtual ~DeleteRecordingResponse();

  std::string recordingtoken_;

};

class DeleteRecordingJobResponse : public Response<RecordingClient>
{
 public:

  DeleteRecordingJobResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken);
  virtual ~DeleteRecordingJobResponse();

  std::string jobtoken_;

};

class DeleteTrackResponse : public Response<RecordingClient>
{
 public:

  DeleteTrackResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken);
  virtual ~DeleteTrackResponse();
  
  std::string recordingtoken_;
  std::string tracktoken_;

};

class ExportRecordedDataResponse : public Response<RecordingClient>
{
 public:

  ExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ws::DateTime startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination);
  ExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const ws::DateTime startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination, const boost::optional<std::string>& operationtoken, const std::vector<std::string>& filenames);
  virtual ~ExportRecordedDataResponse();

  ws::DateTime startpoint_;
  ws::DateTime endpoint_;
  SearchScope searchscope_;
  std::string fileformat_;
  StorageReferencePath storagedestination_; 

  boost::optional<std::string> operationtoken_;
  std::vector<std::string> filenames_;

};

class GetExportRecordedDataStateResponse : public Response<RecordingClient>
{
 public:

  GetExportRecordedDataStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken);
  GetExportRecordedDataStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken, const boost::optional<float>& progress, const boost::optional<ArrayOfFileProgress>& fileprogresstatus);
  virtual ~GetExportRecordedDataStateResponse();

  std::string operationtoken_;

  boost::optional<float> progress_;
  boost::optional<ArrayOfFileProgress> fileprogresstatus_;

};

class GetRecordingConfigurationResponse : public Response<RecordingClient>
{
 public:

  GetRecordingConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken);
  GetRecordingConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const boost::optional<RecordingConfiguration>& recordingconfiguration);
  virtual ~GetRecordingConfigurationResponse();

  std::string recordingtoken_;

  boost::optional<RecordingConfiguration> recordingconfiguration_;

};

class GetRecordingJobConfigurationResponse : public Response<RecordingClient>
{
 public:

  GetRecordingJobConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken);
  GetRecordingJobConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const boost::optional<RecordingJobConfiguration>& jobconfiguration);
  virtual ~GetRecordingJobConfigurationResponse();

  std::string jobtoken_;

  boost::optional<RecordingJobConfiguration> jobconfiguration_;

};

class GetRecordingJobsResponse : public Response<RecordingClient>
{
 public:

  GetRecordingJobsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetRecordingJobsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<recording::GetRecordingJobsResponseItem>& jobitem);
  virtual ~GetRecordingJobsResponse();

  std::vector<recording::GetRecordingJobsResponseItem> jobitem_;

};

class GetRecordingJobStateResponse : public Response<RecordingClient>
{
 public:

  GetRecordingJobStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken);
  GetRecordingJobStateResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const boost::optional<RecordingJobStateInformation>& state);
  virtual ~GetRecordingJobStateResponse();

  std::string jobtoken_;

  boost::optional<RecordingJobStateInformation> state_;

};

class GetRecordingOptionsResponse : public Response<RecordingClient>
{
 public:

  GetRecordingOptionsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken);
  GetRecordingOptionsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const boost::optional<RecordingOptions>& options);
  virtual ~GetRecordingOptionsResponse();

  std::string recordingtoken_;

  boost::optional<RecordingOptions> options_;

};

class GetRecordingsResponse : public Response<RecordingClient>
{
 public:

  GetRecordingsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetRecordingsResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::vector<recording::GetRecordingsResponseItem>& recordingitem);
  virtual ~GetRecordingsResponse();

  std::vector<recording::GetRecordingsResponseItem> recordingitem_;

};

class GetServiceCapabilitiesResponse : public Response<RecordingClient>
{
 public:

  GetServiceCapabilitiesResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message);
  GetServiceCapabilitiesResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const boost::optional<Capabilities>& capabilities);
  virtual ~GetServiceCapabilitiesResponse();

  boost::optional<Capabilities> capabilities_;

};

class GetTrackConfigurationResponse : public Response<RecordingClient>
{
 public:

  GetTrackConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken);
  GetTrackConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken, const boost::optional<TrackConfiguration>& trackconfiguration);
  virtual ~GetTrackConfigurationResponse();

  std::string recordingtoken_;
  std::string tracktoken_;

  boost::optional<TrackConfiguration> trackconfiguration_;

};

class SetRecordingConfigurationResponse : public Response<RecordingClient>
{
 public:

  SetRecordingConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration);
  virtual ~SetRecordingConfigurationResponse();
  
  std::string recordingtoken_;
  RecordingConfiguration recordingconfiguration_;

};

class SetRecordingJobConfigurationResponse : public Response<RecordingClient>
{
 public:
   
  SetRecordingJobConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration);
  SetRecordingJobConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration, const boost::optional<RecordingJobConfiguration>& jobconfigurationresponse);
  virtual ~SetRecordingJobConfigurationResponse();
  
  std::string jobtoken_;
  RecordingJobConfiguration jobconfiguration_;
  
  boost::optional<RecordingJobConfiguration> jobconfigurationresponse_;

};

class SetRecordingJobModeResponse : public Response<RecordingClient>
{
 public:

  SetRecordingJobModeResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& jobtoken, const RECORDINGJOBMODE& mode);
  virtual ~SetRecordingJobModeResponse();
  
  std::string jobtoken_;
  RECORDINGJOBMODE mode_;

};

class SetTrackConfigurationResponse : public Response<RecordingClient>
{
 public:

  SetTrackConfigurationResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration);
  virtual ~SetTrackConfigurationResponse();
  
  std::string recordingtoken_;
  std::string tracktoken_;
  TrackConfiguration trackconfiguration_;

};

class StopExportRecordedDataResponse : public Response<RecordingClient>
{
 public:

  StopExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken);
  StopExportRecordedDataResponse(boost::shared_ptr<RecordingClient> client, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message, const std::string& operationtoken, const boost::optional<float>& progress, const boost::optional<ArrayOfFileProgress>& fileprogressstatus);
  virtual ~StopExportRecordedDataResponse();

  std::string operationtoken_;

  boost::optional<float> progress_;
  boost::optional<ArrayOfFileProgress> fileprogressstatus_;

};

class RecordingClient : public Client<RECORDINGOPERATION>, public boost::enable_shared_from_this<RecordingClient>
{
 public:
  
  using Client::Update;

  RecordingClient();
  virtual ~RecordingClient();

  virtual void Destroy() override;

  // Requests
  void CreateRecording(const RecordingConfiguration& recordingconfiguration);
  void CreateRecordingJob(const RecordingJobConfiguration& jobconfiguration);
  void CreateTrack(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration);
  void DeleteRecording(const std::string& recordingtoken);
  void DeleteRecordingJob(const std::string& jobtoken);
  void DeleteTrack(const std::string& recordingtoken, const std::string& tracktoken);
  void ExportRecordedData(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination);
  void GetExportRecordedDataState(const std::string& operationtoken);
  void GetRecordingConfiguration(const std::string& recordingtoken);
  void GetRecordingJobConfiguration(const std::string& jobtoken);
  void GetRecordingJobs();
  void GetRecordingJobState(const std::string& jobtoken);
  void GetRecordingOptions(const std::string& recordingtoken);
  void GetRecordings();
  void GetServiceCapabilities();
  void GetTrackConfiguration(const std::string& recordingtoken, const std::string& tracktoken);
  void SetRecordingConfiguration(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration);
  void SetRecordingJobConfiguration(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration);
  void SetRecordingJobMode(const std::string& jobtoken, const RECORDINGJOBMODE mode);
  void SetTrackConfiguration(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration);
  void StopExportRecordedData(const std::string& operationtoken);
  
  // Callback requests
  Connection CreateRecordingCallback(const RecordingConfiguration& recordingconfiguration, boost::function<void(const CreateRecordingResponse&)> callback);
  Connection CreateRecordingJobCallback(const RecordingJobConfiguration& jobconfiguration, boost::function<void(const CreateRecordingJobResponse&)> callback);
  Connection CreateTrackCallback(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration, boost::function<void(const CreateTrackResponse&)> callback);
  Connection DeleteRecordingCallback(const std::string& recordingtoken, boost::function<void(const DeleteRecordingResponse&)> callback);
  Connection DeleteRecordingJobCallback(const std::string& jobtoken, boost::function<void(const DeleteRecordingJobResponse&)> callback);
  Connection DeleteTrackCallback(const std::string& recordingtoken, const std::string& tracktoken, boost::function<void(const DeleteTrackResponse&)> callback);
  Connection ExportRecordedDataCallback(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination, boost::function<void(const ExportRecordedDataResponse&)> callback);
  Connection GetExportRecordedDataStateCallback(const std::string& operationtoken, boost::function<void(const GetExportRecordedDataStateResponse&)> callback);
  Connection GetRecordingConfigurationCallback(const std::string& recordingtoken, boost::function<void(const GetRecordingConfigurationResponse&)> callback);
  Connection GetRecordingJobConfigurationCallback(const std::string& jobtoken, boost::function<void(const GetRecordingJobConfigurationResponse&)> callback);
  Connection GetRecordingJobsCallback(boost::function<void(const GetRecordingJobsResponse&)> callback);
  Connection GetRecordingJobStateCallback(const std::string& jobtoken, boost::function<void(const GetRecordingJobStateResponse&)> callback);
  Connection GetRecordingOptionsCallback(const std::string& recordingtoken, boost::function<void(const GetRecordingOptionsResponse&)> callback);
  Connection GetRecordingsCallback(boost::function<void(const GetRecordingsResponse&)> callback);
  Connection GetServiceCapabilitiesCallback(boost::function<void(const GetServiceCapabilitiesResponse&)> callback);
  Connection GetTrackConfigurationCallback(const std::string& recordingtoken, const std::string& tracktoken, boost::function<void(const GetTrackConfigurationResponse&)> callback);
  Connection SetRecordingConfigurationCallback(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration, boost::function<void(const SetRecordingConfigurationResponse&)> callback);
  Connection SetRecordingJobConfigurationCallback(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration, boost::function<void(const SetRecordingJobConfigurationResponse&)> callback);
  Connection SetRecordingJobModeCallback(const std::string& jobtoken, const RECORDINGJOBMODE mode, boost::function<void(const SetRecordingJobModeResponse&)> callback);
  Connection SetTrackConfigurationCallback(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration, boost::function<void(const SetTrackConfigurationResponse&)> callback);
  Connection StopExportRecordedDataCallback(const std::string& operationtoken, boost::function<void(const StopExportRecordedDataResponse&)> callback);

  // Future requests
  boost::unique_future<CreateRecordingResponse> CreateRecordingFuture(const RecordingConfiguration& recordingconfiguration);
  boost::unique_future<CreateRecordingJobResponse> CreateRecordingJobFuture(const RecordingJobConfiguration& jobconfiguration);
  boost::unique_future<CreateTrackResponse> CreateTrackFuture(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration);
  boost::unique_future<DeleteRecordingResponse> DeleteRecordingFuture(const std::string& recordingtoken);
  boost::unique_future<DeleteRecordingJobResponse> DeleteRecordingJobFuture(const std::string& jobtoken);
  boost::unique_future<DeleteTrackResponse> DeleteTrackFuture(const std::string& recordingtoken, const std::string& tracktoken);
  boost::unique_future<ExportRecordedDataResponse> ExportRecordedDataFuture(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination);
  boost::unique_future<GetExportRecordedDataStateResponse> GetExportRecordedDataStateFuture(const std::string& operationtoken);
  boost::unique_future<GetRecordingConfigurationResponse> GetRecordingConfigurationFuture(const std::string& recordingtoken);
  boost::unique_future<GetRecordingJobConfigurationResponse> GetRecordingJobConfigurationFuture(const std::string& jobtoken);
  boost::unique_future<GetRecordingJobsResponse> GetRecordingJobsFuture();
  boost::unique_future<GetRecordingJobStateResponse> GetRecordingJobStateFuture(const std::string& jobtoken);
  boost::unique_future<GetRecordingOptionsResponse> GetRecordingOptionsFuture(const std::string& recordingtoken);
  boost::unique_future<GetRecordingsResponse> GetRecordingsFuture();
  boost::unique_future<GetServiceCapabilitiesResponse> GetServiceCapabilitiesFuture();
  boost::unique_future<GetTrackConfigurationResponse> GetTrackConfigurationFuture(const std::string& recordingtoken, const std::string& tracktoken);
  boost::unique_future<SetRecordingConfigurationResponse> SetRecordingConfigurationFuture(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration);
  boost::unique_future<SetRecordingJobConfigurationResponse> SetRecordingJobConfigurationFuture(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration);
  boost::unique_future<SetRecordingJobModeResponse> SetRecordingJobModeFuture(const std::string& jobtoken, const RECORDINGJOBMODE mode);
  boost::unique_future<SetTrackConfigurationResponse> SetTrackConfigurationFuture(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration);
  boost::unique_future<StopExportRecordedDataResponse> StopExportRecordedDataFuture(const std::string& operationtoken);

  // Boost Signals2
  boost::signals2::signal<void(const CreateRecordingResponse&)>& CreateRecordingSignal();
  boost::signals2::signal<void(const CreateRecordingJobResponse&)>& CreateRecordingJobSignal();
  boost::signals2::signal<void(const CreateTrackResponse&)>& CreateTrackSignal();
  boost::signals2::signal<void(const DeleteRecordingResponse&)>& DeleteRecordingSignal();
  boost::signals2::signal<void(const DeleteRecordingJobResponse&)>& DeleteRecordingJobSignal();
  boost::signals2::signal<void(const DeleteTrackResponse&)>& DeleteTrackSignal();
  boost::signals2::signal<void(const ExportRecordedDataResponse&)>& ExportRecordedDataSignal();
  boost::signals2::signal<void(const GetExportRecordedDataStateResponse&)>& GetExportRecordedDataStateSignal();
  boost::signals2::signal<void(const GetRecordingConfigurationResponse&)>& GetRecordingConfigurationSignal();
  boost::signals2::signal<void(const GetRecordingJobConfigurationResponse&)>& GetRecordingJobConfigurationSignal();
  boost::signals2::signal<void(const GetRecordingJobsResponse&)>& GetRecordingJobsSignal();
  boost::signals2::signal<void(const GetRecordingJobStateResponse&)>& GetRecordingJobStateSignal();
  boost::signals2::signal<void(const GetRecordingOptionsResponse&)>& GetRecordingOptionsSignal();
  boost::signals2::signal<void(const GetRecordingsResponse&)>& GetRecordingsSignal();
  boost::signals2::signal<void(const GetServiceCapabilitiesResponse&)>& GetServiceCapabilitiesSignal();
  boost::signals2::signal<void(const GetTrackConfigurationResponse&)>& GetTrackConfigurationSignal();
  boost::signals2::signal<void(const SetRecordingConfigurationResponse&)>& SetRecordingConfigurationSignal();
  boost::signals2::signal<void(const SetRecordingJobConfigurationResponse&)>& SetRecordingJobConfigurationSignal();
  boost::signals2::signal<void(const SetRecordingJobModeResponse&)>& SetRecordingJobModeSignal();
  boost::signals2::signal<void(const SetTrackConfigurationResponse&)>& SetTrackConfigurationSignal();
  boost::signals2::signal<void(const StopExportRecordedDataResponse&)>& StopExportRecordedDataSignal();

 private:

  virtual void Update(RECORDINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const pugi::xml_document& document, const std::map< std::string, std::vector<char> >& mtomdata) override;
  virtual void SignalError(RECORDINGOPERATION operation, CURL* handle, const boost::asio::ip::address& localendpoint, int64_t latency, const std::string& message) override;

  // Get the body of the request string
  std::string CreateRecordingBody(const RecordingConfiguration& recordingconfiguration);
  std::string CreateRecordingJobBody(const RecordingJobConfiguration& jobconfiguration);
  std::string CreateTrackBody(const std::string& recordingtoken, const TrackConfiguration& trackconfiguration);
  std::string DeleteRecordingBody(const std::string& recordingtoken);
  std::string DeleteRecordingJobBody(const std::string& jobtoken);
  std::string DeleteTrackBody(const std::string& recordingtoken, const std::string& tracktoken);
  std::string ExportRecordedDataBody(const ws::DateTime& startpoint, const ws::DateTime& endpoint, const SearchScope& searchscope, const std::string& fileformat, const StorageReferencePath& storagedestination);
  std::string GetExportRecordedDataStateBody(const std::string& operationtoken);
  std::string GetRecordingConfigurationBody(const std::string& recordingtoken);
  std::string GetRecordingJobConfigurationBody(const std::string& jobtoken);
  std::string GetRecordingJobsBody();
  std::string GetRecordingJobStateBody(const std::string& jobtoken);
  std::string GetRecordingOptionsBody(const std::string& recordingtoken);
  std::string GetRecordingsBody();
  std::string GetServiceCapabilitiesBody();
  std::string GetTrackConfigurationBody(const std::string& recordingtoken, const std::string& tracktoken);
  std::string SetRecordingConfigurationBody(const std::string& recordingtoken, const RecordingConfiguration& recordingconfiguration);
  std::string SetRecordingJobConfigurationBody(const std::string& jobtoken, const RecordingJobConfiguration& jobconfiguration);
  std::string SetRecordingJobModeBody(const std::string& jobtoken, const RECORDINGJOBMODE mode);
  std::string SetTrackConfigurationBody(const std::string& recordingtoken, const std::string& tracktoken, const TrackConfiguration& trackconfiguration);
  std::string StopExportRecordedDataBody(const std::string& operationtoken);

  RecordingSignals* signals_;

};

}

}

#endif
