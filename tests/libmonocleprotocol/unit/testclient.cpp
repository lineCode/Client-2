// testclient.cpp
//

///// Includes /////

#include "testclient.hpp"

///// Namespaces /////

namespace monocle
{

namespace client
{

namespace tests
{

///// Methods /////

TestClient::TestClient(boost::asio::io_service& io) :
  Client(io)
{

}

TestClient::~TestClient()
{

}

sock::Connection TestClient::Init(const sock::ProxyParams& proxyparams, const std::string& hostname, const uint16_t port, const bool httpupgrade, const boost::function<void(const boost::system::error_code&)> connectcallback)
{
  disconnectedcallback_ = std::function<void()>();
  mjpegframecallback_ = std::function<void(const server::tests::TESTFRAME&)>();
  receiverchangedcallback_ = std::function<void(const uint64_t, const monocle::ReceiverMode, const std::string&, const bool, const std::string&, const std::string&, const std::vector<std::string>&)>();
  return Client::Init(proxyparams, hostname, port, httpupgrade, connectcallback);
}

void TestClient::Destroy()
{
  Client::Destroy();
}

void TestClient::ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error)
{

}

void TestClient::Disconnected()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (!disconnectedcallback_)
  {

    return;
  }
  disconnectedcallback_();
}

void TestClient::DiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes)
{

}

void TestClient::FileAdded(const uint64_t token, const std::string& mountpoint, const std::string& path, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate)
{

}

void TestClient::FileRemoved(const uint64_t token)
{

}

void TestClient::FileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate)
{

}

void TestClient::FileStateChanged(const uint64_t token, const monocle::FileState state)
{

}

void TestClient::FindMotionEnd(const uint64_t token, const uint64_t ret)
{

}

void TestClient::FindMotionProgress(const uint64_t token, const float progress)
{

}

void TestClient::FindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end)
{

}

void TestClient::FindObjectEnd(const uint64_t token, const uint64_t ret)
{

}

void TestClient::FindObjectProgress(const uint64_t token, const float progress)
{

}

void TestClient::FindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight)
{

}

void TestClient::Goodbye()
{

}

void TestClient::GroupAdded(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{

}

void TestClient::GroupChanged(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{

}

void TestClient::GroupRemoved(const uint64_t token)
{

}

void TestClient::GuiOrderChanged(const std::vector< std::pair<uint64_t, uint64_t> >& recordingsorder, const std::vector< std::pair<uint64_t, uint64_t> >& mapsorder)
{

}

void TestClient::H265Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size)
{

}

void TestClient::H264Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size)
{

}

void TestClient::HardwareStatsMessage(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats)
{

}

void TestClient::JPEGFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (!mjpegframecallback_)
  {

    return;
  }
  mjpegframecallback_(server::tests::TESTFRAME(token, playrequest, codecindex, timestamp, sequencenum, std::vector<uint8_t>(signature, signature + signaturesize), restartinterval, typespecificfragmentoffset, type, q, width, height, lqt, cqt, std::vector<char>(data, data + size)));
}

void TestClient::LayoutAdded(const monocle::LAYOUT& layout)
{

}

void TestClient::LayoutChanged(const monocle::LAYOUT& layout)
{

}

void TestClient::LayoutNameChanged(const uint64_t token, const std::string& name)
{

}

void TestClient::LayoutRemoved(const uint64_t token)
{

}

void TestClient::LocationChanged(const std::string& latitude, const std::string& longitude)
{

}

void TestClient::MapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5)
{

}

void TestClient::MapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5)
{

}

void TestClient::MapRemoved(const uint64_t token)
{

}

void TestClient::MetadataFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size)
{

}

void TestClient::MountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source)
{

}

void TestClient::MountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source)
{

}

void TestClient::MPEG4Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size)
{

}

void TestClient::NewCodecIndex(const uint64_t stream, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{

}

void TestClient::ObjectDetectorFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::ObjectDetectorFrameType objectdetectorframetype, const char* data, const size_t size)
{

}

void TestClient::ONVIFUserAdded(const uint64_t token, const std::string& username, const ONVIFUserlevel onvifuserlevel)
{

}

void TestClient::ONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const monocle::ONVIFUserlevel onvifuserlevel)
{

}

void TestClient::ONVIFUserRemoved(const uint64_t token)
{

}

void TestClient::ReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state)
{

}

void TestClient::ReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (!receiverchangedcallback_)
  {

    return;
  }
  receiverchangedcallback_(token, mode, mediauri, autocreated, username, password, parameters);
}

void TestClient::ReceiverRemoved(const uint64_t token)
{

}

void TestClient::RecordingActiveJobChanged(const uint64_t recordingtoken, const boost::optional<uint64_t>& activejob)
{

}

void TestClient::RecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool adaptivestreaming, const boost::optional<uint64_t>& activejob)
{

}

void TestClient::RecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool adaptivestreaming, const boost::optional<uint64_t>& activejob)
{

}

void TestClient::RecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state)
{

}

void TestClient::RecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority)
{

}

void TestClient::RecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token)
{

}

void TestClient::RecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken)
{

}

void TestClient::RecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{

}

void TestClient::RecordingRemoved(const uint64_t token)
{

}

void TestClient::RecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message)
{

}

void TestClient::ServerLogMessage(const uint64_t time, const monocle::Severity severity, const std::string& message)
{

}

void TestClient::RecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<std::string>& activeparameters)
{

}

void TestClient::RecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters)
{

}

void TestClient::RecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp)
{

}

void TestClient::RecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id)
{

}

void TestClient::RecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message)
{

}

void TestClient::RecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token)
{

}

void TestClient::RecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const RecordingJobState state, const std::string& error)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (!recordingtrackstatechangedcallback_)
  {

    return;
  }
  recordingtrackstatechangedcallback_(recording, recordingjob, recordingjobsource, recordingjobsourcetrack, time, state);
}

void TestClient::RecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (!recordinglogmessagecallback_)
  {

    return;
  }
  recordinglogmessagecallback_(token, time, severity, message);
}

void TestClient::RecordingsStatistics(const uint64_t time, const std::vector<monocle::RECORDINGSTATISTICS>& recordingsstatistics)
{

}

void TestClient::RecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const std::string& message)
{

}

void TestClient::NameChanged(const std::string& name)
{

}

void TestClient::TrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<CODECINDEX>& codecindices, const std::pair<uint32_t, uint64_t>& totaltrackdata)
{

}

void TestClient::TrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<CODECINDEX>& codecindices, const std::pair<uint32_t, uint64_t>& totaltrackdata)
{

}

void TestClient::TrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end)
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (!trackdeletedatacallback_)
  {

    return;
  }
  trackdeletedatacallback_(recording, trackid, start, end);
}

void TestClient::TrackRemoved(const uint64_t recordingtoken, const uint32_t token)
{

}

void TestClient::TrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices)
{

}

void TestClient::UserAdded(const uint64_t token, const std::string& username, const uint64_t group)
{

}

void TestClient::UserChanged(const uint64_t token, const uint64_t group)
{

}

void TestClient::UserRemoved(const uint64_t token)
{

}

}

}

}
