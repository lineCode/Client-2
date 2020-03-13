// connection.hpp
//

#ifndef IDF69P3OAV5USY5O123456VTGJT0STS5UA
#define IDF69P3OAV5USY5O123456VTGJT0STS5UA

///// Includes /////

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <flatbuffers/flatbuffers.h>
#include <mutex>
#include <vector>
#include <zlib.h>

#include "monocleprotocol/monocleprotocol.hpp"
#include "monocleprotocol/objects_generated.h"

///// Namespaces /////

namespace monocle
{

///// Declarations /////

enum class MetadataFrameType : uint16_t;
enum class ObjectDetectorFrameType : uint16_t;

///// Namespaces /////

namespace server
{

///// Declarations /////

class Server;

///// Structures /////

struct SNAPSHOT
{
  SNAPSHOT();
  SNAPSHOT(const std::vector<uint8_t>& data, const uint64_t time);

  std::vector<uint8_t> data_;
  uint64_t time_;

};

struct STREAM
{
  STREAM();
  STREAM(const uint64_t token, const std::vector<CODECINDEX>& codecindices);

  uint64_t token_;
  std::vector<CODECINDEX> codecindices_;

};

///// Classes /////

class Connection : public boost::enable_shared_from_this<Connection>
{
 public:

  Connection(boost::asio::ip::tcp::socket&& socket, const boost::shared_ptr<Server>& server);
  Connection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server);
  virtual ~Connection();

  void Destroy();

  virtual boost::system::error_code Connected();
  
  virtual void Disconnected() = 0;
  virtual Error AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount) = 0;
  virtual Error AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) = 0;
  virtual Error AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image) = 0;
  virtual Error AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel) = 0;
  virtual Error AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) = 0;
  virtual std::pair<Error, uint64_t> AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob) = 0;
  virtual std::pair<Error, uint64_t> AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources) = 0;
  virtual std::pair<Error, uint32_t> AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files) = 0;
  virtual Error AddTrack2(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters) = 0;
  virtual Error AddUser(const std::string& username, const std::string& digest, const uint64_t group) = 0;
  virtual std::pair<Error, AUTHENTICATERESPONSE> Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest) = 0;
  virtual Error ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) = 0;
  virtual Error ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image) = 0;
  virtual Error ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const ONVIFUserlevel onvifuserlevel) = 0;
  virtual Error ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) = 0;
  virtual Error ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime) = 0;
  virtual Error ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources) = 0;
  virtual Error ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files) = 0;
  virtual Error ChangeTrack2(const uint64_t recordingtoken, const uint32_t trackid, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint32_t objectdetectortrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters) = 0;
  virtual Error ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group) = 0;
  virtual Error ControlStream(const uint64_t streamtoken, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const boost::optional<bool>& forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes) = 0;
  virtual Error ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum) = 0;
  virtual Error ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequestindex) = 0;
  virtual Error ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time) = 0;
  virtual std::pair<Error, uint64_t> CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast) = 0;
  virtual std::pair<Error, uint64_t> CreateFindObject(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height) = 0;
  virtual std::pair<Error, STREAM> CreateStream(const uint64_t recordingtoken, const uint64_t tracktoken) = 0;
  virtual Error DestroyFindMotion(const uint64_t token) = 0;
  virtual Error DestroyFindObject(const uint64_t token) = 0;
  virtual Error DestroyStream(const uint64_t streamtoken) = 0;
  virtual Error DiscoveryBroadcast() = 0;
  virtual std::string GetAuthenticationNonce() = 0;
  virtual std::pair< Error, std::vector<std::string> > GetChildFolders(const std::string& path, const bool parentpaths) = 0;
  virtual std::pair< Error, std::vector<FILE> > GetFiles() = 0;
  virtual std::pair< Error, std::vector<RECEIVER> > GetReceivers() = 0;
  virtual std::pair<Error, RECORDING> GetRecording(const uint64_t token) = 0;
  virtual std::pair< Error, std::vector<RECORDING> > GetRecordings() = 0;
  virtual std::pair<Error, SNAPSHOT> GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, const float x, const float y, const float width, const float height) = 0;
  virtual std::pair<Error, GETSTATE> GetState() = 0;
  virtual uint64_t GetTime() = 0;
  virtual Error MountFile(const uint64_t token) = 0;
  virtual Error RemoveFile(const uint64_t token) = 0;
  virtual Error RemoveGroup(const uint64_t token) = 0;
  virtual Error RemoveMap(const uint64_t token) = 0;
  virtual Error RemoveONVIFUser(const uint64_t token) = 0;
  virtual Error RemoveReceiver(const uint64_t token) = 0;
  virtual Error RemoveRecording(const uint64_t token) = 0;
  virtual Error RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token) = 0;
  virtual Error RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token) = 0;
  virtual Error RemoveTrack(const uint64_t recordingtoken, const uint32_t id) = 0;
  virtual Error RemoveTracks(const uint64_t recordingtoken, const std::vector<uint32_t>& ids) = 0;
  virtual Error RemoveUser(const uint64_t token) = 0;
  virtual Error SetLocation(const std::string& latitude, const std::string& longitude) = 0;
  virtual Error SetName(const std::string& name) = 0;
  virtual std::pair<Error, SUBSCRIBE> Subscribe() = 0;
  virtual Error SubscribeDiscovery(const bool broadcast) = 0;
  virtual std::pair<Error, monocle::HARDWARESTATS> SubscribeHardwareStats() = 0;
  virtual std::pair<monocle::Error, SUBSCRIBERECORDINGJOBLOG> SubscribeRecordingJobLog(const uint64_t recordingtoken) = 0;
  virtual std::pair<monocle::Error, SUBSCRIBERECORDINGJOBSOURCETRACKLOG> SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken) = 0;
  virtual std::pair<monocle::Error, monocle::SUBSCRIBERECORDINGLOG> SubscribeRecordingLog(const uint64_t token) = 0;
  virtual std::pair<monocle::Error, SUBSCRIBERECORDINGTRACKLOG> SubscribeRecordingTrackLog(const uint64_t recordingtoken) = 0;
  virtual Error UnmountFile(const uint64_t token) = 0;
  virtual Error UnsubscribeDiscovery() = 0;
  virtual Error UnsubscribeHardwareStats() = 0;
  virtual void UnsubscribeRecordingJobLog(const uint64_t recordingtoken) = 0;
  virtual void UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken) = 0;
  virtual void UnsubscribeRecordingLog(const uint64_t token) = 0;
  virtual void UnsubscribeRecordingTrackLog(const uint64_t recordingtoken) = 0;

  virtual bool Permission(const Message message) = 0;

  boost::system::error_code SendControlStreamEnd(const uint64_t stream, const uint64_t playrequest, const monocle::ErrorCode error);
  boost::system::error_code SendDiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes);
  boost::system::error_code SendFileAdded(const uint64_t token, const boost::filesystem::path& path, const boost::filesystem::path& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount);
  boost::system::error_code SendFileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate);
  boost::system::error_code SendFileRemoved(const uint64_t token);
  boost::system::error_code SendFileStateChanged(const uint64_t token, const monocle::FileState state);
  boost::system::error_code SendFindMotionEnd(const uint64_t token, const uint64_t ret);
  boost::system::error_code SendFindMotionProgress(const uint64_t token, const float progress);
  boost::system::error_code SendFindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end);
  boost::system::error_code SendFindObjectEnd(const uint64_t token, const uint64_t ret);
  boost::system::error_code SendFindObjectProgress(const uint64_t token, const float progress);
  boost::system::error_code SendFindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight);
  boost::system::error_code SendGoodbye();
  boost::system::error_code SendGroupAdded(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  boost::system::error_code SendGroupChanged(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  boost::system::error_code SendGroupRemoved(const uint64_t token);
  boost::system::error_code SendH265Frame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size);
  boost::system::error_code SendH264Frame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size);
  boost::system::error_code SendHardwareStats(const uint64_t timestamp, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats);
  boost::system::error_code SendJPEGFrame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size);
  boost::system::error_code SendLocationChanged(const std::string& latitude, const std::string& location);
  boost::system::error_code SendMapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5);
  boost::system::error_code SendMapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5);
  boost::system::error_code SendMapRemoved(const uint64_t token);
  boost::system::error_code SendMetadataFrame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size);
  boost::system::error_code SendMountAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source);
  boost::system::error_code SendMountRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source);
  boost::system::error_code SendMPEG4Frame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size);
  boost::system::error_code SendNameChanged(const std::string& name);
  boost::system::error_code SendNewCodecIndex(const uint64_t stream, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp);
  boost::system::error_code SendObjectDetectorFrame(const uint64_t stream, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const boost::optional<uint64_t>& sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::ObjectDetectorFrameType objectdetectorframetype, const char* data, const size_t size);
  boost::system::error_code SendONVIFUserAdded(const uint64_t token, const std::string& username, const ONVIFUserlevel onvifuserlevel);
  boost::system::error_code SendONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const ONVIFUserlevel onvifuserlevel);
  boost::system::error_code SendONVIFUserRemoved(const uint64_t token);
  boost::system::error_code SendReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state);
  boost::system::error_code SendReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters);
  boost::system::error_code SendReceiverRemoved(const uint64_t token);
  boost::system::error_code SendRecordingActiveJobChanged(const uint64_t token, const boost::optional<uint64_t>& activejob);
  boost::system::error_code SendRecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  boost::system::error_code SendRecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  boost::system::error_code SendRecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message);
  boost::system::error_code SendRecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state);
  boost::system::error_code SendRecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority);
  boost::system::error_code SendRecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token);
  boost::system::error_code SendRecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken);
  boost::system::error_code SendRecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token);
  boost::system::error_code SendRecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<std::string>& activeparameters);
  boost::system::error_code SendRecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters);
  boost::system::error_code SendRecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message);
  boost::system::error_code SendRecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token);
  boost::system::error_code SendRecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const RecordingJobState state, const std::string& error);
  boost::system::error_code SendRecordingLogMessage(const uint64_t token, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message);
  boost::system::error_code SendRecordingRemoved(const uint64_t token);
  boost::system::error_code SendRecordingStatistics(const uint64_t time, const std::vector<monocle::RECORDINGSTATISTICS>& recordingsstatistics);
  boost::system::error_code SendRecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp);
  boost::system::error_code SendRecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id);
  boost::system::error_code SendRecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message);
  boost::system::error_code SendServerLogMessage(const std::chrono::system_clock::time_point time, const monocle::Severity severity, const std::string& message);
  boost::system::error_code SendTrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::pair<uint64_t, uint64_t>& totaltrackdata);
  boost::system::error_code SendTrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::pair<uint64_t, uint64_t>& totaltrackdata);
  boost::system::error_code SendTrackRemoved(const uint64_t recordingtoken, const uint32_t id);
  boost::system::error_code SendTrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices);
  boost::system::error_code SendTrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);
  boost::system::error_code SendUserAdded(const uint64_t token, const std::string& username, const std::string& digest, const uint64_t group);
  boost::system::error_code SendUserChanged(const uint64_t token, const uint64_t group);
  boost::system::error_code SendUserRemoved(const uint64_t token);

  inline boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
  inline uint16_t GetNextSequence() { return ++sequence_; }

 protected:

  void HandleReadHeader(const boost::system::error_code& err, std::size_t size);
  void HandleReadMessage(const boost::system::error_code& err, const uint32_t size, const bool error, const bool compressed, const Message message, const uint16_t sequence);
  boost::system::error_code HandleMessage(const bool error, const bool compressed, const Message message, const uint16_t sequence, const char* data, size_t datasize);
  boost::system::error_code SendHeaderResponse(const HEADER& header);
  boost::system::error_code SendErrorResponse(const Message message, const uint16_t sequence, const Error& error);
  boost::system::error_code SendResponse(const bool compress, const Message message, const uint16_t sequence);
  void DestroyConnection();

  boost::shared_ptr<Server> server_;

  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf buffer_;

  mutable std::mutex writemutex_;
  flatbuffers::FlatBufferBuilder fbb_;

  uint16_t sequence_;

  z_stream deflate_;
  z_stream inflate_;
  std::vector<char> zbuffer_; // Buffer for inflating and deflating

 private:

  void InitCompression();

};

}

}

#endif
