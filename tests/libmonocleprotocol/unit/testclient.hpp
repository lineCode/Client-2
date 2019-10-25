// testclient.hpp
//

#ifndef IDHDWTLPP684734234XCOXZ4YWZOJ6O51N
#define IDHDWTLPP684734234XCOXZ4YWZOJ6O51N

///// Includes /////

#include <functional>
#include <monocleprotocol/client/client.hpp>

#include "testserver.hpp"

///// Namespaces /////

namespace monocle
{

namespace client
{

namespace tests
{

///// Classes /////

class TestClient : public client::Client
{
 public:

  TestClient(boost::asio::io_service& io);
  virtual ~TestClient();

  virtual sock::Connection Init(const sock::ProxyParams& proxyparams, const std::string& hostname, const uint16_t port, const bool httpupgrade, const boost::function<void(const boost::system::error_code&)> connectcallback) override;
  virtual void Destroy() override;

  virtual void ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error) override;
  virtual void Disconnected() override;
  virtual void DiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes) override;
  virtual void FileAdded(const uint64_t token, const std::string& mountpoint, const std::string& path, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate) override;
  virtual void FileRemoved(const uint64_t token) override;
  virtual void FileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate) override;
  virtual void FileStateChanged(const uint64_t token, const monocle::FileState state) override;
  virtual void FindMotionEnd(const uint64_t token, const uint64_t ret) override;
  virtual void FindMotionProgress(const uint64_t token, const float progress) override;
  virtual void FindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end) override;
  virtual void FindObjectEnd(const uint64_t token, const uint64_t ret) override;
  virtual void FindObjectProgress(const uint64_t token, const float progress) override;
  virtual void FindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight) override;
  virtual void Goodbye() override;
  virtual void GroupAdded(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) override;
  virtual void GroupChanged(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) override;
  virtual void GroupRemoved(const uint64_t token) override;
  virtual void H265Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) override;
  virtual void H264Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) override;
  virtual void HardwareStatsMessage(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory) override;
  virtual void JPEGFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size) override;
  virtual void LocationChanged(const std::string& latitude, const std::string& longitude) override;
  virtual void MapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) override;
  virtual void MapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) override;
  virtual void MapRemoved(const uint64_t token) override;
  virtual void MetadataFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size) override;
  virtual void MountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) override;
  virtual void MountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) override;
  virtual void MPEG4Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size) override;
  virtual void NewCodecIndex(const uint64_t stream, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp) override;
  virtual void ONVIFUserAdded(const uint64_t token, const std::string& username, const ONVIFUserlevel onvifuserlevel) override;
  virtual void ONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const monocle::ONVIFUserlevel onvifuserlevel) override;
  virtual void ONVIFUserRemoved(const uint64_t token) override;
  virtual void ReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauril, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state) override;
  virtual void ReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) override;
  virtual void ReceiverRemoved(const uint64_t token) override;
  virtual void RecordingActiveJobChanged(const uint64_t recordingtoken, const boost::optional<uint64_t>& activejob) override;
  virtual void RecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob) override;
  virtual void RecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob) override;
  virtual void RecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state) override;
  virtual void RecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority) override;
  virtual void RecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token) override;
  virtual void RecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken) override;
  virtual void RecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token) override;
  virtual void RecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<std::string>& activeparameters) override;
  virtual void RecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters) override;
  virtual void RecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token) override;
  virtual void RecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const RecordingJobState state, const std::string& error) override;
  virtual void RecordingRemoved(const uint64_t token) override;
  virtual void ServerLogMessage(const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp) override;
  virtual void RecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id) override;
  virtual void RecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void NameChanged(const std::string& name) override;
  virtual void TrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<CODECINDEX>& codecindices) override;
  virtual void TrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<CODECINDEX>& codecindices) override;
  virtual void TrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end) override;
  virtual void TrackRemoved(const uint64_t recordingtoken, const uint32_t token) override;
  virtual void TrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices) override;
  virtual void UserAdded(const uint64_t token, const std::string& username, const uint64_t group) override;
  virtual void UserChanged(const uint64_t token, const uint64_t group) override;
  virtual void UserRemoved(const uint64_t token) override;

  inline void SetDisconnectedCallback(const std::function<void()>& disconnectedcallback) { std::lock_guard<std::recursive_mutex> lock(mutex_); disconnectedcallback_ = disconnectedcallback; }
  inline void SetMJPEGFrameCallback(const std::function<void(const server::tests::TESTFRAME&)>& mjpegframecallback) { std::lock_guard<std::recursive_mutex> lock(mutex_); mjpegframecallback_ = mjpegframecallback; }
  inline void SetReceiverChangedCallback(const std::function<void(const uint64_t, const monocle::ReceiverMode, const std::string&, const bool, const std::string&, const std::string&, const std::vector<std::string>&)>& receiverchangedcallback) { std::lock_guard<std::recursive_mutex> lock(mutex_); receiverchangedcallback_ = receiverchangedcallback; }
  inline void SetRecordingTrackStateChangedCallback(const std::function<void(const uint64_t, const uint64_t, const uint64_t, const uint64_t, const uint64_t, const RecordingJobState)>& recordingtrackstatechangedcallback) { std::lock_guard<std::recursive_mutex> lock(mutex_); recordingtrackstatechangedcallback_ = recordingtrackstatechangedcallback; }
  inline void SetTrackDeleteDataCallback(const std::function<void(const uint64_t, const uint32_t, const boost::optional<uint64_t>&, const boost::optional<uint64_t>&)>& trackdeletedatacallback) { std::lock_guard<std::recursive_mutex> lock(mutex_); trackdeletedatacallback_ = trackdeletedatacallback; }
  inline void SetRecordingLogMessageCallback(const std::function<void(const uint64_t, const uint64_t, const monocle::Severity, const std::string&)>& recordinglogmessagecallback) { std::lock_guard<std::recursive_mutex> lock(mutex_); recordinglogmessagecallback_ = recordinglogmessagecallback; }

 protected:


 private:

  std::function<void()> disconnectedcallback_;
  std::function<void(const server::tests::TESTFRAME&)> mjpegframecallback_;
  std::function<void(const uint64_t, const monocle::ReceiverMode, const std::string&, const bool, const std::string&, const std::string&, const std::vector<std::string>&)> receiverchangedcallback_;
  std::function<void(const uint64_t, const uint64_t, const uint64_t, const uint64_t, const uint64_t, const RecordingJobState)> recordingtrackstatechangedcallback_;
  std::function<void(const uint64_t, const uint32_t, const boost::optional<uint64_t>&, const boost::optional<uint64_t>&)> trackdeletedatacallback_;
  std::function<void(const uint64_t, const uint64_t, const monocle::Severity, const std::string&)> recordinglogmessagecallback_;

};

}

}

}

#endif
