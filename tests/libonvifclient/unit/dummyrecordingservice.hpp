// dummyrecordingservice.hpp
//

#ifndef IDOQJ1NCGGM7GVYCUFD2UEG88E5QD28OFK
#define IDOQJ1NCGGM7GVYCUFD2UEG88E5QD28OFK

///// Includes /////

#include <onvifserver/onvifserver.hpp>
#include <onviftypes/onviftypes.hpp>
#include <vector>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class DummyRecordingService : public server::Service
{
 public:

  DummyRecordingService();
  virtual ~DummyRecordingService();

  virtual bool IsRestricted(evhttp_cmd_type requesttype, const std::string& path, const std::string& content) const override { return false; }
  virtual bool DigestAuthenticate(evhttp_cmd_type requesttype, const std::vector<char>& content, const std::string& path, const std::string& clientusername, const std::string& clientrealm, const std::string& clientnonce, const std::string& clienturi, const std::string& clientcnonce, const std::string& clientresponse, const std::string& clientqop) const override { return false; }

  virtual boost::shared_future<server::ServerResponse> Request(bool authenticated, evhttp_cmd_type requesttype, const std::string& localaddress, const uint16_t port, const std::string& remoteaddress, const std::string& path, const std::string& parameters, const std::vector<char>& content, const std::map< std::string, std::vector<char> >& mtomdata) override;
  
  inline void SetCreateRecording(const std::string& createrecording) { createrecording_ = createrecording; }
  inline void SetCreateRecordingJob(const std::string& createrecordingjobjobtoken, const recording::RecordingJobConfiguration& createrecordingjobjobconfiguration) { createrecordingjobjobtoken_ = createrecordingjobjobtoken; createrecordingjobjobconfiguration_ = createrecordingjobjobconfiguration; }
  inline void SetCreateTrack(const std::string& createtrack) { createtrack_ = createtrack; }
  inline void SetExportRecordedData(const std::string& exportrecordeddataoperationtoken, const std::vector<std::string>& exportrecordeddatafilenames) { exportrecordeddataoperationtoken_ = exportrecordeddataoperationtoken; exportrecordeddatafilenames_ = exportrecordeddatafilenames;}
  inline void SetGetExportRecordedDataState(const float getexportrecordeddatastateprogress, const ArrayOfFileProgress& getexportrecordeddatastatefileprogressstatus) { getexportrecordeddatastateprogress_ = getexportrecordeddatastateprogress; getexportrecordeddatastatefileprogressstatus_ = getexportrecordeddatastatefileprogressstatus; }
  inline void SetGetRecordingConfiguration(const recording::RecordingConfiguration& getrecordingconfiguration) { getrecordingconfiguration_ = getrecordingconfiguration; }
  inline void SetGetRecordingJobConfiguration(const recording::RecordingJobConfiguration& getrecordingjobconfiguration) { getrecordingjobconfiguration_ = getrecordingjobconfiguration; }
  inline void SetGetRecordingJobs(const std::vector<recording::GetRecordingJobsResponseItem>& getrecordingjobs) { getrecordingjobs_ = getrecordingjobs; }
  inline void SetGetRecordingJobState(const recording::RecordingJobStateInformation& getrecordingjobstate) { getrecordingjobstate_ = getrecordingjobstate; }
  inline void SetGetRecordingOptions(const recording::RecordingOptions& getrecordingoptions) { getrecordingoptions_ = getrecordingoptions; }
  inline void SetGetRecordings(const std::vector<recording::GetRecordingsResponseItem>& getrecordings) { getrecordings_ = getrecordings; }
  inline void SetGetServiceCapabilities(const recording::Capabilities& getservicecapabilities) { getservicecapabilities_ = getservicecapabilities; }
  inline void SetGetTrackConfiguration(const TrackConfiguration& gettrackconfiguration) { gettrackconfiguration_ = gettrackconfiguration; }
  inline void SetSetRecordingJobConfiguration(const recording::RecordingJobConfiguration& setrecordingjobconfiguration) { setrecordingjobconfiguration_ = setrecordingjobconfiguration; }
  inline void SetStopExportRecordedData(const float stopexportrecordeddataprogress, const ArrayOfFileProgress& stopexportrecordeddatafileprogressstatus) { stopexportrecordeddataprogress_ = stopexportrecordeddataprogress; stopexportrecordeddatafileprogressstatus_ = stopexportrecordeddatafileprogressstatus; }

 private:

  std::string createrecording_;
  std::string createrecordingjobjobtoken_;
  recording::RecordingJobConfiguration createrecordingjobjobconfiguration_;
  std::string createtrack_;
  std::string exportrecordeddataoperationtoken_;
  std::vector<std::string> exportrecordeddatafilenames_;
  float getexportrecordeddatastateprogress_;
  ArrayOfFileProgress getexportrecordeddatastatefileprogressstatus_;
  recording::RecordingConfiguration getrecordingconfiguration_;
  recording::RecordingJobConfiguration getrecordingjobconfiguration_;
  std::vector<recording::GetRecordingJobsResponseItem> getrecordingjobs_;
  recording::RecordingJobStateInformation getrecordingjobstate_;
  recording::RecordingOptions getrecordingoptions_;
  std::vector<recording::GetRecordingsResponseItem> getrecordings_;
  recording::Capabilities getservicecapabilities_;
  TrackConfiguration gettrackconfiguration_;
  recording::RecordingJobConfiguration setrecordingjobconfiguration_;
  float stopexportrecordeddataprogress_;
  ArrayOfFileProgress stopexportrecordeddatafileprogressstatus_;

};

}

}

#endif
