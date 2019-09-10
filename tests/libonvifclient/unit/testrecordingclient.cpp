// testrecordingclient.cpp
//

///// Includes /////

#include "testrecordingclient.hpp"

#include <boost/optional.hpp>
#include <onviftypes/onviftypes.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 8000;
const std::string RECORDINGURI("/onvif/recording");

///// Methods /////

TestRecordingClient::TestRecordingClient()
{
  
}

TestRecordingClient::~TestRecordingClient()
{
  
}

void TestRecordingClient::SetUp()
{
  Init(PORT, RECORDINGURI);

  // Setup dummy server
  dummyrecordingservice_ = std::make_unique<DummyRecordingService>();
  ASSERT_FALSE(httpserver_.AddServer(RECORDINGURI, dummyrecordingservice_.get()));
}

void TestRecordingClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(RECORDINGURI));
  dummyrecordingservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestRecordingClient, CreateRecording)
{
  const recording::RecordingConfiguration recordingconfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5));
  const std::string recordingtoken("RecordingToken");
  dummyrecordingservice_->SetCreateRecording(recordingtoken);

  recording::CreateRecordingResponse createrecordingresponse = client_->CreateRecordingFuture(recordingconfiguration).get();
  ASSERT_GE(createrecordingresponse.latency_, 0);
  ASSERT_FALSE(createrecordingresponse.Error());
  ASSERT_TRUE(createrecordingresponse.recordingtoken_.is_initialized());
  ASSERT_EQ(recordingtoken, *createrecordingresponse.recordingtoken_);
}

TEST_F(TestRecordingClient, CreateRecordingJob)
{
  const std::string jobtoken("JobToken");
  const recording::RecordingJobConfiguration jobconfiguration(std::string("RecordingToken"), recording::RECORDINGJOBMODE_ACTIVE, 5, { recording::RecordingJobSource(SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }), recording::RecordingJobSource(SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }) });
  dummyrecordingservice_->SetCreateRecordingJob(jobtoken, jobconfiguration);

  recording::CreateRecordingJobResponse createrecordingjobresponse = client_->CreateRecordingJobFuture(jobconfiguration).get();
  ASSERT_GE(createrecordingjobresponse.latency_, 0);
  ASSERT_FALSE(createrecordingjobresponse.Error());
  ASSERT_EQ(jobconfiguration, createrecordingjobresponse.jobconfiguration_);
  ASSERT_EQ(jobtoken, *createrecordingjobresponse.jobtoken_);
  ASSERT_EQ(jobconfiguration, *createrecordingjobresponse.jobconfigurationresponse_);
}

TEST_F(TestRecordingClient, CreateTrack)
{
  const std::string recordingtoken("RecordingToken");
  const TrackConfiguration trackconfiguration(TRACKTYPE_AUDIO, std::string("Description"));
  const std::string tracktoken("TrackToken");
  dummyrecordingservice_->SetCreateTrack(tracktoken);

  recording::CreateTrackResponse createtrackresponse = client_->CreateTrackFuture(recordingtoken, trackconfiguration).get();
  ASSERT_GE(createtrackresponse.latency_, 0);
  ASSERT_FALSE(createtrackresponse.Error());
  ASSERT_EQ(recordingtoken, createtrackresponse.recordingtoken_);
  ASSERT_EQ(trackconfiguration, createtrackresponse.trackconfiguration_);
  ASSERT_EQ(tracktoken, *createtrackresponse.tracktoken_);
}

TEST_F(TestRecordingClient, DeleteRecording)
{
  const std::string recordingtoken("RecordingToken");

  recording::DeleteRecordingResponse deleterecordingresponse = client_->DeleteRecordingFuture(recordingtoken).get();
  ASSERT_GE(deleterecordingresponse.latency_, 0);
  ASSERT_FALSE(deleterecordingresponse.Error());
  ASSERT_EQ(recordingtoken, deleterecordingresponse.recordingtoken_);
}

TEST_F(TestRecordingClient, DeleteRecordingJob)
{
  const std::string jobtoken("JobToken");

  recording::DeleteRecordingJobResponse deleterecordingjobresponse = client_->DeleteRecordingJobFuture(jobtoken).get();
  ASSERT_GE(deleterecordingjobresponse.latency_, 0);
  ASSERT_FALSE(deleterecordingjobresponse.Error());
  ASSERT_EQ(jobtoken, deleterecordingjobresponse.jobtoken_);
}

TEST_F(TestRecordingClient, DeleteTrack)
{
  const std::string recordingtoken("RecordingToken");
  const std::string tracktoken("TrackToken");

  recording::DeleteTrackResponse deletetrackresponse = client_->DeleteTrackFuture(recordingtoken, tracktoken).get();
  ASSERT_GE(deletetrackresponse.latency_, 0);
  ASSERT_FALSE(deletetrackresponse.Error());
  ASSERT_EQ(recordingtoken, deletetrackresponse.recordingtoken_);
  ASSERT_EQ(tracktoken, deletetrackresponse.tracktoken_);
}

TEST_F(TestRecordingClient, ExportRecordedData)
{
  const ws::DateTime startpoint("startpoint");
  const ws::DateTime endpoint("endpoint");
  const SearchScope searchscope({ SourceReference(std::string("type1"), std::string("token1")), SourceReference(std::string("type2"), std::string("token2")) }, { std::string("IncludedRecordings1"), std::string("IncludedRecordings2") }, std::string("RecordingInformationFilter"));
  const std::string fileformat("FileFormat");
  const StorageReferencePath storagedestination(std::string("storagetoken"), std::string("relativepath"));
  const std::string operationtoken("OperationToken");
  std::vector<std::string> filenames = { "FilenNme1", "FileName2" };
  dummyrecordingservice_->SetExportRecordedData(operationtoken, filenames);

  recording::ExportRecordedDataResponse exportrecordeddataresponse = client_->ExportRecordedDataFuture(startpoint, endpoint, searchscope, fileformat, storagedestination).get();
  ASSERT_GE(exportrecordeddataresponse.latency_, 0);
  ASSERT_FALSE(exportrecordeddataresponse.Error());
  ASSERT_EQ(startpoint, exportrecordeddataresponse.startpoint_);
  ASSERT_EQ(endpoint, exportrecordeddataresponse.endpoint_);
  ASSERT_EQ(searchscope, exportrecordeddataresponse.searchscope_);
  ASSERT_EQ(fileformat, exportrecordeddataresponse.fileformat_);
  ASSERT_EQ(storagedestination, exportrecordeddataresponse.storagedestination_);
  ASSERT_EQ(operationtoken, *exportrecordeddataresponse.operationtoken_);
  ASSERT_EQ(filenames, exportrecordeddataresponse.filenames_);
}

TEST_F(TestRecordingClient, GetExportRecordedDataState)
{
  const std::string operationtoken("OperationToken");
  const float progress = 5.0f;
  const ArrayOfFileProgress fileprogressstatus({ FileProgress(std::string("filename1"), 1.0f), FileProgress(std::string("filename2"), 1.0f) });
  dummyrecordingservice_->SetGetExportRecordedDataState(progress, fileprogressstatus);

  recording::GetExportRecordedDataStateResponse getexportrecordeddatastateresponse = client_->GetExportRecordedDataStateFuture(operationtoken).get();
  ASSERT_GE(getexportrecordeddatastateresponse.latency_, 0);
  ASSERT_FALSE(getexportrecordeddatastateresponse.Error());
  ASSERT_EQ(operationtoken, getexportrecordeddatastateresponse.operationtoken_);
  ASSERT_EQ(progress, *getexportrecordeddatastateresponse.progress_);
  ASSERT_EQ(fileprogressstatus, *getexportrecordeddatastateresponse.fileprogresstatus_);
}

TEST_F(TestRecordingClient, GetRecordingConfiguration)
{
  const std::string recordingtoken("RecordingToken");
  const recording::RecordingConfiguration recordingconfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5));
  dummyrecordingservice_->SetGetRecordingConfiguration(recordingconfiguration);

  recording::GetRecordingConfigurationResponse getrecordingconfigurationresponse = client_->GetRecordingConfigurationFuture(recordingtoken).get();
  ASSERT_GE(getrecordingconfigurationresponse.latency_, 0);
  ASSERT_FALSE(getrecordingconfigurationresponse.Error());
  ASSERT_EQ(recordingtoken, getrecordingconfigurationresponse.recordingtoken_);
  ASSERT_EQ(recordingconfiguration, *getrecordingconfigurationresponse.recordingconfiguration_);
}

TEST_F(TestRecordingClient, GetRecordingConfigurationJob)
{
  const std::string jobtoken("JobToken");
  const recording::RecordingJobConfiguration jobconfiguration(std::string("RecordingToken"), recording::RECORDINGJOBMODE_ACTIVE, 5, { recording::RecordingJobSource(SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }), recording::RecordingJobSource(SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }) });
  dummyrecordingservice_->SetGetRecordingJobConfiguration(jobconfiguration);

  recording::GetRecordingJobConfigurationResponse recordingjobconfigurationresponse = client_->GetRecordingJobConfigurationFuture(jobtoken).get();
  ASSERT_GE(recordingjobconfigurationresponse.latency_, 0);
  ASSERT_FALSE(recordingjobconfigurationresponse.Error());
  ASSERT_EQ(jobtoken, recordingjobconfigurationresponse.jobtoken_);
  ASSERT_EQ(jobconfiguration, *recordingjobconfigurationresponse.jobconfiguration_);
}

TEST_F(TestRecordingClient, GetRecordingJobs)
{
  const std::vector<recording::GetRecordingJobsResponseItem> jobitem(
  {
    recording::GetRecordingJobsResponseItem(std::string("JobToken1"), recording::RecordingJobConfiguration(std::string("RecordingToken"), recording::RECORDINGJOBMODE_ACTIVE, 5, { recording::RecordingJobSource(SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }), recording::RecordingJobSource(SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }) })),
    recording::GetRecordingJobsResponseItem(std::string("JobToken2"), recording::RecordingJobConfiguration(std::string("RecordingToken"), recording::RECORDINGJOBMODE_ACTIVE, 5, { recording::RecordingJobSource(SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }), recording::RecordingJobSource(SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }) }))
  });
  dummyrecordingservice_->SetGetRecordingJobs(jobitem);

  recording::GetRecordingJobsResponse getrecordingjobsresponse = client_->GetRecordingJobsFuture().get();
  ASSERT_GE(getrecordingjobsresponse.latency_, 0);
  ASSERT_FALSE(getrecordingjobsresponse.Error());
  ASSERT_EQ(jobitem, getrecordingjobsresponse.jobitem_);
}

TEST_F(TestRecordingClient, GetRecordingJobState)
{
  const std::string jobtoken("JobToken");
  const recording::RecordingJobStateInformation state(std::string("RecordingToken"), recording::RECORDINGJOBSTATE_ACTIVE, std::vector<recording::RecordingJobStateSource>({ recording::RecordingJobStateSource(SourceReference(std::string("type"), std::string("token")), recording::RECORDINGJOBSTATE_ACTIVE, recording::RecordingJobStateTracks({ recording::RecordingJobStateTrack(std::string("sourcetag1"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE), recording::RecordingJobStateTrack(std::string("sourcetag2"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE) })), recording::RecordingJobStateSource(SourceReference(std::string("type"), std::string("token")), recording::RECORDINGJOBSTATE_ACTIVE, recording::RecordingJobStateTracks({ recording::RecordingJobStateTrack(std::string("sourcetag1"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE), recording::RecordingJobStateTrack(std::string("sourcetag2"), std::string("destination"), std::string("error"), recording::RECORDINGJOBSTATE_ACTIVE) })) }));
  dummyrecordingservice_->SetGetRecordingJobState(state);

  recording::GetRecordingJobStateResponse getrecordingjobstateresponse = client_->GetRecordingJobStateFuture(jobtoken).get();
  ASSERT_GE(getrecordingjobstateresponse.latency_, 0);
  ASSERT_FALSE(getrecordingjobstateresponse.Error());
  ASSERT_EQ(jobtoken, getrecordingjobstateresponse.jobtoken_);
  ASSERT_EQ(state, *getrecordingjobstateresponse.state_);
}

TEST_F(TestRecordingClient, GetRecordingOptions)
{
  const std::string recordingtoken("RecordingToken");
  const recording::RecordingOptions options(recording::JobOptions(5, StringAttrList(std::vector<std::string>({ "test1", "test2" }))), recording::TrackOptions(5, 5, 5, 5));
  dummyrecordingservice_->SetGetRecordingOptions(options);

  recording::GetRecordingOptionsResponse getrecordingoptionsresponse = client_->GetRecordingOptionsFuture(recordingtoken).get();
  ASSERT_GE(getrecordingoptionsresponse.latency_, 0);
  ASSERT_FALSE(getrecordingoptionsresponse.Error());
  ASSERT_EQ(recordingtoken, getrecordingoptionsresponse.recordingtoken_);
  ASSERT_EQ(options, *getrecordingoptionsresponse.options_);
}

TEST_F(TestRecordingClient, GetRecordings)
{
  const std::vector<recording::GetRecordingsResponseItem> recordingitems(
  {
    recording::GetRecordingsResponseItem(std::string("RecordingToken1"), recording::RecordingConfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5)), recording::GetTracksResponseList({ recording::GetTracksResponseItem(std::string("TrackToken1"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description1"))), recording::GetTracksResponseItem(std::string("TrackToken2"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description2"))) })),
    recording::GetRecordingsResponseItem(std::string("RecordingToken2"), recording::RecordingConfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5)), recording::GetTracksResponseList({ recording::GetTracksResponseItem(std::string("TrackToken1"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description1"))), recording::GetTracksResponseItem(std::string("TrackToken2"), TrackConfiguration(TRACKTYPE_AUDIO, std::string("description2"))) }))
  });
  dummyrecordingservice_->SetGetRecordings(recordingitems);

  recording::GetRecordingsResponse getrecordingsresponse = client_->GetRecordingsFuture().get();
  ASSERT_GE(getrecordingsresponse.latency_, 0);
  ASSERT_FALSE(getrecordingsresponse.Error());
  ASSERT_EQ(recordingitems, getrecordingsresponse.recordingitem_);
}

TEST_F(TestRecordingClient, GetServiceCapabilities)
{
  const recording::Capabilities capabilities(true, true, recording::EncodingTypes({ std::string("encoding1"), std::string("encoding2") }), 5.0f, 6.0f, 7, 5, true, true, StringAttrList(std::vector<std::string>({ "test1", "test" })));
  dummyrecordingservice_->SetGetServiceCapabilities(capabilities);

  recording::GetServiceCapabilitiesResponse getrecordingservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getrecordingservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getrecordingservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getrecordingservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getrecordingservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestRecordingClient, GetTrackConfiguration)
{
  const std::string recordingtoken("RecordingToken");
  const std::string tracktoken("TrackToken");
  const TrackConfiguration trackconfiguration(TRACKTYPE_AUDIO, std::string("description"));
  dummyrecordingservice_->SetGetTrackConfiguration(trackconfiguration);

  recording::GetTrackConfigurationResponse gettrackconfigurationresponse = client_->GetTrackConfigurationFuture(recordingtoken, tracktoken).get();
  ASSERT_GE(gettrackconfigurationresponse.latency_, 0);
  ASSERT_FALSE(gettrackconfigurationresponse.Error());
  ASSERT_EQ(recordingtoken, gettrackconfigurationresponse.recordingtoken_);
  ASSERT_EQ(tracktoken, gettrackconfigurationresponse.tracktoken_);
  ASSERT_EQ(trackconfiguration, *gettrackconfigurationresponse.trackconfiguration_);
}

TEST_F(TestRecordingClient, SetRecordingConfiguration)
{
  const std::string recordingtoken("RecordingToken");
  const recording::RecordingConfiguration recordingconfiguration(RecordingSourceInformation(std::string("sourceid"), std::string("name"), std::string("location"), std::string("description"), std::string("address")), std::string("content"), Duration(5, 5, 5, 5, 5, 5));

  recording::SetRecordingConfigurationResponse setrecordingconfigurationresponse = client_->SetRecordingConfigurationFuture(recordingtoken, recordingconfiguration).get();
  ASSERT_GE(setrecordingconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setrecordingconfigurationresponse.Error());
  ASSERT_EQ(recordingtoken, setrecordingconfigurationresponse.recordingtoken_);
  ASSERT_EQ(recordingconfiguration, setrecordingconfigurationresponse.recordingconfiguration_);
}

TEST_F(TestRecordingClient, SetRecordingJobConfiguration)
{
  const std::string jobtoken("JobToken");
  const recording::RecordingJobConfiguration jobconfiguration(std::string("RecordingToken"), recording::RECORDINGJOBMODE_ACTIVE, 5, { recording::RecordingJobSource(SourceReference(std::string("type1"), std::string("token1")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }), recording::RecordingJobSource(SourceReference(std::string("type2"), std::string("token2")), true, { recording::RecordingJobTrack(std::string("sourcetag1"), std::string("destination")), recording::RecordingJobTrack(std::string("sourcetag2"), std::string("destination")) }) });
  dummyrecordingservice_->SetSetRecordingJobConfiguration(jobconfiguration);

  recording::SetRecordingJobConfigurationResponse setrecordingjobconfigurationresponse = client_->SetRecordingJobConfigurationFuture(jobtoken, jobconfiguration).get();
  ASSERT_GE(setrecordingjobconfigurationresponse.latency_, 0);
  ASSERT_FALSE(setrecordingjobconfigurationresponse.Error());
  ASSERT_EQ(jobtoken, setrecordingjobconfigurationresponse.jobtoken_);
  ASSERT_EQ(jobconfiguration, setrecordingjobconfigurationresponse.jobconfiguration_);
  ASSERT_EQ(jobconfiguration, setrecordingjobconfigurationresponse.jobconfigurationresponse_);
}

TEST_F(TestRecordingClient, SetRecordingJobMode)
{
  const std::string jobtoken("JobToken");
  const recording::RECORDINGJOBMODE mode = recording::RECORDINGJOBMODE_ACTIVE;

  recording::SetRecordingJobModeResponse setrecordingjobmoderesponse = client_->SetRecordingJobModeFuture(jobtoken, mode).get();
  ASSERT_GE(setrecordingjobmoderesponse.latency_, 0);
  ASSERT_FALSE(setrecordingjobmoderesponse.Error());
  ASSERT_EQ(jobtoken, setrecordingjobmoderesponse.jobtoken_);
  ASSERT_EQ(mode, setrecordingjobmoderesponse.mode_);
}

TEST_F(TestRecordingClient, SetTrackConfiguration)
{
  const std::string recordingtoken("RecordingToken");
  const std::string tracktoken("TrackToken");
  const TrackConfiguration trackconfiguration(TRACKTYPE_AUDIO, std::string("description"));
  const recording::RECORDINGJOBMODE mode = recording::RECORDINGJOBMODE_ACTIVE;

  recording::SetTrackConfigurationResponse settrackconfigurationresponse = client_->SetTrackConfigurationFuture(recordingtoken, tracktoken, trackconfiguration).get();
  ASSERT_GE(settrackconfigurationresponse.latency_, 0);
  ASSERT_FALSE(settrackconfigurationresponse.Error());
  ASSERT_EQ(recordingtoken, settrackconfigurationresponse.recordingtoken_);
  ASSERT_EQ(tracktoken, settrackconfigurationresponse.tracktoken_);
  ASSERT_EQ(trackconfiguration, settrackconfigurationresponse.trackconfiguration_);
}

TEST_F(TestRecordingClient, StopExportRecordedData)
{
  const std::string operationtoken("OperationToken");
  const float progress = 5.0f;
  const ArrayOfFileProgress fileprogressstatus({ FileProgress(std::string("filename1"), 1.0f), FileProgress(std::string("filename2"), 1.0f) });
  dummyrecordingservice_->SetStopExportRecordedData(progress, fileprogressstatus);

  recording::StopExportRecordedDataResponse stopexportrecordeddataresponse = client_->StopExportRecordedDataFuture(operationtoken).get();
  ASSERT_GE(stopexportrecordeddataresponse.latency_, 0);
  ASSERT_FALSE(stopexportrecordeddataresponse.Error());
  ASSERT_EQ(operationtoken, stopexportrecordeddataresponse.operationtoken_);
  ASSERT_EQ(progress, *stopexportrecordeddataresponse.progress_);
  ASSERT_EQ(fileprogressstatus, *stopexportrecordeddataresponse.fileprogressstatus_);
}

}

}
