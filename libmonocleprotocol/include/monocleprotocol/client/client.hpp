// client.hpp
//

#ifndef IDF69P3OAV5ULY5O7254Q6ETGJT0STS5UA
#define IDF69P3OAV5ULY5O7254Q6ETGJT0STS5UA

///// Includes /////

#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/function.hpp>
#include <boost/thread/future.hpp>
#include <flatbuffers/flatbuffers.h>
#include <mutex>
#include <socket/tcpsocket.hpp>
#include <stdint.h>
#include <thread>
#include <zlib.h>

#include "monocleprotocol/client/connection.hpp"
#include "monocleprotocol/client/response.hpp"
#include "monocleprotocol/client/signal.hpp"
#include "monocleprotocol/errorresponse_generated.h"
#include "monocleprotocol/monocleprotocol.hpp"

///// Namespaces /////

namespace monocle
{

///// Declarations /////

enum class MetadataFrameType : uint16_t;

///// Namespaces /////

namespace client
{

///// Classes /////

class Client : public boost::enable_shared_from_this<Client>
{
 friend class Signal<Client, ADDFILERESPONSE>;
 friend class Signal<Client, ADDGROUPRESPONSE>;
 friend class Signal<Client, ADDMAPRESPONSE>;
 friend class Signal<Client, ADDONVIFUSERRESPONSE>;
 friend class Signal<Client, ADDRECEIVERRESPONSE>;
 friend class Signal<Client, ADDRECORDINGRESPONSE>;
 friend class Signal<Client, ADDRECORDINGJOBRESPONSE>;
 friend class Signal<Client, ADDTRACKRESPONSE>;
 friend class Signal<Client, ADDUSERRESPONSE>;
 friend class Signal<Client, AUTHENTICATERESPONSE>;
 friend class Signal<Client, CHANGEGROUPRESPONSE>;
 friend class Signal<Client, CHANGEMAPRESPONSE>;
 friend class Signal<Client, CHANGEONVIFUSERRESPONSE>;
 friend class Signal<Client, CHANGERECEIVERRESPONSE>;
 friend class Signal<Client, CHANGERECORDINGJOBRESPONSE>;
 friend class Signal<Client, CHANGERECORDINGRESPONSE>;
 friend class Signal<Client, CHANGETRACKRESPONSE>;
 friend class Signal<Client, CHANGEUSERRESPONSE>;
 friend class Signal<Client, CONTROLSTREAMRESPONSE>;
 friend class Signal<Client, CREATEFINDMOTIONRESPONSE>;
 friend class Signal<Client, CREATESTREAMRESPONSE>;
 friend class Signal<Client, DESTROYFINDMOTIONRESPONSE>;
 friend class Signal<Client, DESTROYSTREAMRESPONSE>;
 friend class Signal<Client, DISCOVERYBROADCASTRESPONSE>;
 friend class Signal<Client, GETAUTHENTICATIONNONCERESPONSE>;
 friend class Signal<Client, GETCHILDFOLDERSRESPONSE>;
 friend class Signal<Client, GETFILESRESPONSE>;
 friend class Signal<Client, GETRECEIVERSRESPONSE>;
 friend class Signal<Client, GETRECORDINGSRESPONSE>;
 friend class Signal<Client, GETSNAPSHOTRESPONSE>;
 friend class Signal<Client, GETSTATERESPONSE>;
 friend class Signal<Client, GETTIMERESPONSE>;
 friend class Signal<Client, KEEPALIVERESPONSE>;
 friend class Signal<Client, MOUNTFILERESPONSE>;
 friend class Signal<Client, REMOVEFILERESPONSE>;
 friend class Signal<Client, REMOVEGROUPRESPONSE>;
 friend class Signal<Client, REMOVEMAPRESPONSE>;
 friend class Signal<Client, REMOVEONVIFUSERRESPONSE>;
 friend class Signal<Client, REMOVERECEIVERRESPONSE>;
 friend class Signal<Client, REMOVERECORDINGJOBRESPONSE>;
 friend class Signal<Client, REMOVERECORDINGJOBSOURCERESPONSE>;
 friend class Signal<Client, REMOVERECORDINGRESPONSE>;
 friend class Signal<Client, REMOVETRACKRESPONSE>;
 friend class Signal<Client, REMOVEUSERRESPONSE>;
 friend class Signal<Client, SETLOCATIONRESPONSE>;
 friend class Signal<Client, SETNAMERESPONSE>;
 friend class Signal<Client, SUBSCRIBERESPONSE>;
 friend class Signal<Client, SUBSCRIBEDISCOVERYRESPONSE>;
 friend class Signal<Client, SUBSCRIBEHARDWARESTATSRESPONSE>;
 friend class Signal<Client, SUBSCRIBERECORDINGJOBLOGRESPONSE>;
 friend class Signal<Client, SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE>;
 friend class Signal<Client, SUBSCRIBERECORDINGLOGRESPONSE>;
 friend class Signal<Client, SUBSCRIBERECORDINGTRACKLOGRESPONSE>;
 friend class Signal<Client, UNMOUNTFILERESPONSE>;
 friend class Signal<Client, UNSUBSCRIBEDISCOVERYRESPONSE>;
 friend class Signal<Client, UNSUBSCRIBEHARDWARESTATSRESPONSE>;
 friend class Signal<Client, UNSUBSCRIBERECORDINGJOBLOGRESPONSE>;
 friend class Signal<Client, UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE>;
 friend class Signal<Client, UNSUBSCRIBERECORDINGLOGRESPONSE>;
 friend class Signal<Client, UNSUBSCRIBERECORDINGTRACKLOGRESPONSE>;

 public:

  static int32_t GetDefaultTimeout();

  Client(boost::asio::io_service& io);
  virtual ~Client();

  virtual sock::Connection Init(const sock::ProxyParams& proxyparams, const std::string& hostname, const uint16_t port, const bool httpupgrade, const boost::function<void(const boost::system::error_code&)> connectcallback);
  virtual void Destroy();

  virtual void ControlStreamEnd(const uint64_t streamtoken, const uint64_t playrequestindex, const monocle::ErrorCode error) = 0;
  virtual void Disconnected() = 0;
  virtual void DiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes) = 0;
  virtual void FileAdded(const uint64_t token, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate) = 0;
  virtual void FileRemoved(const uint64_t token) = 0;
  virtual void FileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate) = 0;
  virtual void FileStateChanged(const uint64_t token, const monocle::FileState state) = 0;
  virtual void FindMotionEnd(const uint64_t token, const uint64_t ret) = 0;
  virtual void FindMotionProgress(const uint64_t token, const float progress) = 0;
  virtual void FindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end) = 0;
  virtual void Goodbye() = 0;
  virtual void GroupAdded(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) = 0;
  virtual void GroupChanged(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) = 0;
  virtual void GroupRemoved(const uint64_t token) = 0;
  virtual void H265Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) = 0;
  virtual void H264Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) = 0;
  virtual void HardwareStatsMessage(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory) = 0;
  virtual void JPEGFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size) = 0;
  virtual void MapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) = 0;
  virtual void MapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) = 0;
  virtual void MapRemoved(const uint64_t token) = 0;
  virtual void MetadataFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size) = 0;
  virtual void MountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) = 0;
  virtual void MountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) = 0;
  virtual void MPEG4Frame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size) = 0;
  virtual void LocationChanged(const std::string& latitude, const std::string& longitude) = 0;
  virtual void NameChanged(const std::string& namechanged) = 0;
  virtual void NewCodecIndex(const uint64_t stream, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp) = 0;
  virtual void ONVIFUserAdded(const uint64_t token, const std::string& username, const ONVIFUserlevel onvifuserlevel) = 0;
  virtual void ONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const monocle::ONVIFUserlevel onvifuserlevel) = 0;
  virtual void ONVIFUserRemoved(const uint64_t token) = 0;
  virtual void ReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state) = 0;
  virtual void ReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) = 0;
  virtual void ReceiverRemoved(const uint64_t token) = 0;
  virtual void RecordingActiveJobChanged(const uint64_t recordingtoken, const boost::optional<uint64_t>& activejob) = 0;
  virtual void RecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob) = 0;
  virtual void RecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob) = 0;
  virtual void RecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state) = 0;
  virtual void RecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority) = 0;
  virtual void RecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token) = 0;
  virtual void RecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken) = 0;
  virtual void RecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token) = 0;
  virtual void RecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters) = 0;
  virtual void RecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) = 0;
  virtual void RecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token) = 0;
  virtual void RecordingRemoved(const uint64_t token) = 0;
  virtual void RecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) = 0;
  virtual void RecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<std::string>& activeparameters) = 0;
  virtual void RecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const RecordingJobState state, const std::string& error) = 0;
  virtual void RecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) = 0;
  virtual void RecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const std::string& message) = 0;
  virtual void ServerLogMessage(const uint64_t time, const monocle::Severity severity, const std::string& message) = 0;
  virtual void TrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files) = 0;
  virtual void TrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files) = 0;
  virtual void TrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end) = 0;
  virtual void TrackRemoved(const uint64_t recordingtoken, const uint32_t id) = 0;
  virtual void TrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices) = 0;
  virtual void UserAdded(const uint64_t token, const std::string& username, const uint64_t group) = 0;
  virtual void UserChanged(const uint64_t token, const uint64_t group) = 0;
  virtual void UserRemoved(const uint64_t token) = 0;

  boost::unique_future<ADDFILERESPONSE> AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount);
  boost::unique_future<ADDGROUPRESPONSE> AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  boost::unique_future<ADDMAPRESPONSE> AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image);
  boost::unique_future<ADDONVIFUSERRESPONSE> AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel);
  boost::unique_future<ADDRECEIVERRESPONSE> AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters);
  boost::unique_future<ADDRECORDINGRESPONSE> AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob);
  boost::unique_future<ADDRECORDINGJOBRESPONSE> AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources);
  boost::unique_future<ADDTRACKRESPONSE> AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files);
  boost::unique_future<ADDUSERRESPONSE> AddUser(const std::string& username, const std::string& password, const uint64_t group);
  boost::unique_future<AUTHENTICATERESPONSE> Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest);
  boost::unique_future<CHANGEGROUPRESPONSE> ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  boost::unique_future<CHANGEMAPRESPONSE> ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image);
  boost::unique_future<CHANGERECORDINGJOBRESPONSE> ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources);
  boost::unique_future<CHANGEONVIFUSERRESPONSE> ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const monocle::ONVIFUserlevel onvifuserlevel);
  boost::unique_future<CHANGERECEIVERRESPONSE> ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters);
  boost::unique_future<CHANGERECORDINGRESPONSE> ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime);
  boost::unique_future<CHANGETRACKRESPONSE> ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files);
  boost::unique_future<CHANGEUSERRESPONSE> ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group);
  boost::unique_future<CONTROLSTREAMRESPONSE> ControlStream(const uint64_t streamtoken, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes);
  boost::unique_future<CONTROLSTREAMRESPONSE> ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum);
  boost::unique_future<CONTROLSTREAMRESPONSE> ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequestindex);
  boost::unique_future<CONTROLSTREAMRESPONSE> ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time);
  boost::unique_future<CREATEFINDMOTIONRESPONSE> CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast);
  boost::unique_future<CREATESTREAMRESPONSE> CreateStream(const uint64_t recordingtoken, const uint32_t tracktoken);
  boost::unique_future<DESTROYFINDMOTIONRESPONSE> DestroyFindMotion(const uint64_t token);
  boost::unique_future<DESTROYSTREAMRESPONSE> DestroyStream(const uint64_t streamtoken);
  boost::unique_future<DISCOVERYBROADCASTRESPONSE> DiscoveryBroadcast();
  boost::unique_future<GETAUTHENTICATIONNONCERESPONSE> GetAuthenticationNonce();
  boost::unique_future<GETCHILDFOLDERSRESPONSE> GetChildFolders(const std::string& path);
  boost::unique_future<GETFILESRESPONSE> GetFiles();
  boost::unique_future<GETRECEIVERSRESPONSE> GetReceivers();
  boost::unique_future<GETRECORDINGSRESPONSE> GetRecordings();
  boost::unique_future<GETSNAPSHOTRESPONSE> GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time);
  boost::unique_future<GETSTATERESPONSE> GetState();
  boost::unique_future<GETTIMERESPONSE> GetTime();
  boost::unique_future<KEEPALIVERESPONSE> Keepalive();
  boost::unique_future<MOUNTFILERESPONSE> MountFile(const uint64_t token);
  boost::unique_future<REMOVEFILERESPONSE> RemoveFile(const uint64_t token);
  boost::unique_future<REMOVEGROUPRESPONSE> RemoveGroup(const uint64_t token);
  boost::unique_future<REMOVEMAPRESPONSE> RemoveMap(const uint64_t token);
  boost::unique_future<REMOVEONVIFUSERRESPONSE> RemoveONVIFUser(const uint64_t token);
  boost::unique_future<REMOVERECEIVERRESPONSE> RemoveReceiver(const uint64_t token);
  boost::unique_future<REMOVERECORDINGRESPONSE> RemoveRecording(const uint64_t token);
  boost::unique_future<REMOVERECORDINGJOBRESPONSE> RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token);
  boost::unique_future<REMOVERECORDINGJOBSOURCERESPONSE> RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token);
  boost::unique_future<REMOVETRACKRESPONSE> RemoveTrack(const uint64_t recordingtoken, const uint32_t id);
  boost::unique_future<REMOVEUSERRESPONSE> RemoveUser(const uint64_t token);
  boost::unique_future<SETLOCATIONRESPONSE> SetLocation(const std::string& latitude, const std::string& longitude);
  boost::unique_future<SETNAMERESPONSE> SetName(const std::string& name);
  boost::unique_future<SUBSCRIBERESPONSE> Subscribe();
  boost::unique_future<SUBSCRIBEDISCOVERYRESPONSE> SubscribeDiscovery(const bool broadcast);
  boost::unique_future<SUBSCRIBEHARDWARESTATSRESPONSE> SubscribeHardwareStats();
  boost::unique_future<SUBSCRIBERECORDINGJOBLOGRESPONSE> SubscribeRecordingJobLog(const uint64_t recordingtoken);
  boost::unique_future<SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE> SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken);
  boost::unique_future<SUBSCRIBERECORDINGLOGRESPONSE> SubscribeRecordingLog(const uint64_t recordingtoken);
  boost::unique_future<SUBSCRIBERECORDINGTRACKLOGRESPONSE> SubscribeRecordingTrackLog(const uint64_t recordingtoken);
  boost::unique_future<UNMOUNTFILERESPONSE> UnmountFile(const uint64_t token);
  boost::unique_future<UNSUBSCRIBEDISCOVERYRESPONSE> UnsubscribeDiscovery();
  boost::unique_future<UNSUBSCRIBEHARDWARESTATSRESPONSE> UnsubscribeHardwareStats();
  boost::unique_future<UNSUBSCRIBERECORDINGJOBLOGRESPONSE> UnsubscribeRecordingJobLog(const uint64_t recordingtoken);
  boost::unique_future<UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE> UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken);
  boost::unique_future<UNSUBSCRIBERECORDINGLOGRESPONSE> UnsubscribeRecordingLog(const uint64_t recordingtoken);
  boost::unique_future<UNSUBSCRIBERECORDINGTRACKLOGRESPONSE> UnsubscribeRecordingTrackLog(const uint64_t recordingtoken);

  Connection AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount, boost::function<void(const std::chrono::steady_clock::duration, const ADDFILERESPONSE&)> callback);
  Connection AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool allrecordings, const bool managemaps, const bool managedevice, const std::vector<uint64_t>& recordings, boost::function<void(const std::chrono::steady_clock::duration, const ADDGROUPRESPONSE&)> callback);
  Connection AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image, boost::function<void(const std::chrono::steady_clock::duration, const ADDMAPRESPONSE&)> callback);
  Connection AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel, boost::function<void(const std::chrono::steady_clock::duration, const ADDONVIFUSERRESPONSE&)> callback);
  Connection AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, boost::function<void(const std::chrono::steady_clock::duration, const ADDRECEIVERRESPONSE&)> callback);
  Connection AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob, boost::function<void(const std::chrono::steady_clock::duration, const ADDRECORDINGRESPONSE&)> callback);
  Connection AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources, boost::function<void(const std::chrono::steady_clock::duration, const ADDRECORDINGJOBRESPONSE&)> callback);
  Connection AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, boost::function<void(const std::chrono::steady_clock::duration, const ADDTRACKRESPONSE&)> callback);
  Connection AddUser(const std::string& username, const std::string& password, const uint64_t group, boost::function<void(const std::chrono::steady_clock::duration, const ADDUSERRESPONSE&)> callback);
  Connection ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEGROUPRESPONSE&)> callback);
  Connection ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEMAPRESPONSE&)> callback);
  Connection ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const monocle::ONVIFUserlevel onvifuserlevel, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEONVIFUSERRESPONSE&)> callback);
  Connection ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, boost::function<void(const std::chrono::steady_clock::duration, const CHANGERECEIVERRESPONSE&)> callback);
  Connection ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, boost::function<void(const std::chrono::steady_clock::duration, const CHANGERECORDINGRESPONSE&)> callback);
  Connection ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources, boost::function<void(const std::chrono::steady_clock::duration, const CHANGERECORDINGJOBRESPONSE&)> callback);
  Connection ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, boost::function<void(const std::chrono::steady_clock::duration, const CHANGETRACKRESPONSE&)> callback);
  Connection ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group, boost::function<void(const std::chrono::steady_clock::duration, const CHANGEUSERRESPONSE&)> callback);
  Connection Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest, boost::function<void(const std::chrono::steady_clock::duration, const AUTHENTICATERESPONSE&)> callback);
  Connection ControlStream(const uint64_t streamtoken, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback);
  Connection ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback);
  Connection ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequestindex, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback);
  Connection ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time, boost::function<void(const std::chrono::steady_clock::duration, const CONTROLSTREAMRESPONSE&)> callback);
  Connection CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast, boost::function<void(const std::chrono::steady_clock::duration, const CREATEFINDMOTIONRESPONSE&)> callback);
  Connection CreateStream(const uint64_t recordingtoken, const uint32_t tracktoken, boost::function<void(const std::chrono::steady_clock::duration, const CREATESTREAMRESPONSE&)> callback);
  Connection DestroyFindMotion(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const DESTROYFINDMOTIONRESPONSE&)> callback);
  Connection DestroyStream(const uint64_t streamtoken, boost::function<void(const std::chrono::steady_clock::duration, const DESTROYSTREAMRESPONSE&)> callback);
  Connection DiscoveryBroadcast(boost::function<void(const std::chrono::steady_clock::duration, const DISCOVERYBROADCASTRESPONSE&)> callback);
  Connection GetAuthenticationNonce(boost::function<void(const std::chrono::steady_clock::duration, const GETAUTHENTICATIONNONCERESPONSE&)> callback);
  Connection GetChildFolders(const std::string& path, boost::function<void(const std::chrono::steady_clock::duration, const GETCHILDFOLDERSRESPONSE&)> callback);
  Connection GetFiles(boost::function<void(const std::chrono::steady_clock::duration, const GETFILESRESPONSE&)> callback);
  Connection GetReceivers(boost::function<void(const std::chrono::steady_clock::duration, const GETRECEIVERSRESPONSE&)> callback);
  Connection GetRecordings(boost::function<void(const std::chrono::steady_clock::duration, const GETRECORDINGSRESPONSE&)> callback);
  Connection GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, boost::function<void(const std::chrono::steady_clock::duration, const GETSNAPSHOTRESPONSE&)> callback);
  Connection GetState(boost::function<void(const std::chrono::steady_clock::duration, const GETSTATERESPONSE&)> callback);
  Connection GetTime(boost::function<void(const std::chrono::steady_clock::duration, const GETTIMERESPONSE&)> callback);
  Connection Keepalive(boost::function<void(const std::chrono::steady_clock::duration, const KEEPALIVERESPONSE&)> callback);
  Connection MountFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const MOUNTFILERESPONSE&)> callback);
  Connection RemoveFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVEFILERESPONSE&)> callback);
  Connection RemoveGroup(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVEGROUPRESPONSE&)> callback);
  Connection RemoveMap(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVEMAPRESPONSE&)> callback);
  Connection RemoveONVIFUser(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVEONVIFUSERRESPONSE&)> callback);
  Connection RemoveReceiver(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVERECEIVERRESPONSE&)> callback);
  Connection RemoveRecording(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVERECORDINGRESPONSE&)> callback);
  Connection RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVERECORDINGJOBRESPONSE&)> callback);
  Connection RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVERECORDINGJOBSOURCERESPONSE&)> callback);
  Connection RemoveTrack(const uint64_t recordingtoken, const uint32_t id, boost::function<void(const std::chrono::steady_clock::duration, const REMOVETRACKRESPONSE&)> callback);
  Connection RemoveUser(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const REMOVEUSERRESPONSE&)> callback);
  Connection SetLocation(const std::string& latitude, const std::string& longitude, boost::function<void(const std::chrono::steady_clock::duration, const SETLOCATIONRESPONSE&)> callback);
  Connection SetName(const std::string& name, boost::function<void(const std::chrono::steady_clock::duration, const SETNAMERESPONSE&)> callback);
  Connection Subscribe(boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBERESPONSE&)> callback);
  Connection SubscribeDiscovery(const bool broadcast, boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBEDISCOVERYRESPONSE&)> callback);
  Connection SubscribeHardwareStats(boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBEHARDWARESTATSRESPONSE&)> callback);
  Connection SubscribeRecordingJobLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBERECORDINGJOBLOGRESPONSE&)> callback);
  Connection SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE&)> callback);
  Connection SubscribeRecordingLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBERECORDINGLOGRESPONSE&)> callback);
  Connection SubscribeRecordingTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const SUBSCRIBERECORDINGTRACKLOGRESPONSE&)> callback);
  Connection UnmountFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const UNMOUNTFILERESPONSE&)> callback);
  Connection UnsubscribeDiscovery(boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBEDISCOVERYRESPONSE&)> callback);
  Connection UnsubscribeHardwareStats(boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBEHARDWARESTATSRESPONSE&)> callback);
  Connection UnsubscribeRecordingJobLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBERECORDINGJOBLOGRESPONSE&)> callback);
  Connection UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE&)> callback);
  Connection UnsubscribeRecordingLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBERECORDINGLOGRESPONSE&)> callback);
  Connection UnsubscribeRecordingTrackLog(const uint64_t recordingtoken, boost::function<void(const std::chrono::steady_clock::duration, const UNSUBSCRIBERECORDINGTRACKLOGRESPONSE&)> callback);

 protected:

  void HandleConnect(const boost::system::error_code& err);
  void HandleHTTPUpgradeResponse(const boost::system::error_code& err, const size_t size, const boost::function<void(const boost::system::error_code&)> connectcallback, boost::shared_ptr<sock::ConnectionBlock> connectionblock);
  void HandleReadHeader(const boost::system::error_code& err);
  void HandleReadMessage(const boost::system::error_code& err, const bool error, const bool compressed, const Message message, const uint16_t sequence, const size_t size);
  void HandleMessage(const bool error, const bool compressed, const Message message, const uint16_t sequence, const char* data, size_t datasize);

  boost::system::error_code AddFileSend(const std::string& mountpoint, const std::string& path, const bool fillfisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount);
  boost::system::error_code AddGroupSend(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  boost::system::error_code AddMapSend(const std::string& name, const std::string& location, const std::vector<int8_t>& image);
  boost::system::error_code AddONVIFUserSend(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel);
  boost::system::error_code AddReceiverSend(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters);
  boost::system::error_code AddRecordingSend(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjob);
  boost::system::error_code AddRecordingJobSend(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<ADDRECORDINGJOBSOURCE>& sources);
  boost::system::error_code AddTrackSend(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files);
  boost::system::error_code AddUserSend(const std::string& username, const std::string& password, const uint64_t group);
  boost::system::error_code AuthenticateSend(const std::string& username, const std::string& clientnonce, const std::string& authdigest);
  boost::system::error_code ChangeGroupSend(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  boost::system::error_code ChangeMapSend(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image);
  boost::system::error_code ChangeONVIFUserSend(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const monocle::ONVIFUserlevel onvifuserlevel);
  boost::system::error_code ChangeReceiverSend(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters);
  boost::system::error_code ChangeRecordingSend(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime);
  boost::system::error_code ChangeRecordingJobSend(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources);
  boost::system::error_code ChangeTrackSend(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files);
  boost::system::error_code ChangeUserSend(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group);
  boost::system::error_code ControlStreamSend(const uint64_t streamtoken, const uint64_t playrequest, const bool fetchmarker, const bool ratecontrol, const bool forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes);
  boost::system::error_code ControlStreamFrameStepSend(const uint64_t streamtoken, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum);
  boost::system::error_code ControlStreamLiveSend(const uint64_t streamtoken, const uint64_t playrequest);
  boost::system::error_code ControlStreamPauseSend(const uint64_t streamtoken, const boost::optional<uint64_t>& time);
  boost::system::error_code CreateFindMotionSend(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast);
  boost::system::error_code CreateStreamSend(const uint64_t recordingtoken, const uint32_t tracktoken);
  boost::system::error_code DestroyFindMotionSend(const uint64_t token);
  boost::system::error_code DestroyStreamSend(const uint64_t streamtoken);
  boost::system::error_code DiscoveryBroadcastSend();
  boost::system::error_code GetAuthenticationNonceSend();
  boost::system::error_code GetChildFoldersSend(const std::string& path);
  boost::system::error_code GetFilesSend();
  boost::system::error_code GetReceiversSend();
  boost::system::error_code GetRecordingsSend();
  boost::system::error_code GetSnapshotSend(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time);
  boost::system::error_code GetStateSend();
  boost::system::error_code GetTimeSend();
  boost::system::error_code MountFileSend(const uint64_t token);
  boost::system::error_code RemoveFileSend(const uint64_t token);
  boost::system::error_code RemoveGroupSend(const uint64_t token);
  boost::system::error_code RemoveMapSend(const uint64_t token);
  boost::system::error_code RemoveONVIFUserSend(const uint64_t token);
  boost::system::error_code RemoveReceiverSend(const uint64_t token);
  boost::system::error_code RemoveRecordingJobSend(const uint64_t recordingtoken, const uint64_t token);
  boost::system::error_code RemoveRecordingJobSourceSend(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token);
  boost::system::error_code RemoveRecordingSend(const uint64_t token);
  boost::system::error_code RemoveTrackSend(const uint64_t recordingtoken, const uint32_t id);
  boost::system::error_code RemoveUserSend(const uint64_t token);
  boost::system::error_code SetLocationSend(const std::string& latitude, const std::string& longitude);
  boost::system::error_code SetNameSend(const std::string& name);
  boost::system::error_code SubscribeSend();
  boost::system::error_code SubscribeDiscoverySend(const bool broadcast);
  boost::system::error_code SubscribeHardwareStatsSend();
  boost::system::error_code SubscribeRecordingJobLogSend(const uint64_t recordingtoken);
  boost::system::error_code SubscribeRecordingJobSourceTrackLogSend(const uint64_t recordingtoken);
  boost::system::error_code SubscribeRecordingLogSend(const uint64_t recordingtoken);
  boost::system::error_code SubscribeRecordingTrackLogSend(const uint64_t recordingtoken);
  boost::system::error_code UnmountFileSend(const uint64_t token);
  boost::system::error_code UnsubscribeDiscoverySend();
  boost::system::error_code UnsubscribeHardwareStatsSend();
  boost::system::error_code UnsubscribeRecordingJobLogSend(const uint64_t recordingtoken);
  boost::system::error_code UnsubscribeRecordingJobSourceTrackLogSend(const uint64_t recordingtoken);
  boost::system::error_code UnsubscribeRecordingLogSend(const uint64_t recordingtoken);
  boost::system::error_code UnsubscribeRecordingTrackLogSend(const uint64_t recordingtoken);

  std::pair< Error, std::vector<ONVIFUSER> > GetONVIFUsersBuffer(const flatbuffers::Vector< flatbuffers::Offset<ONVIFUser> >& onvifuserbuffers) const;
  std::pair< Error, std::vector<GROUP> > GetGroupsBuffer(const flatbuffers::Vector< flatbuffers::Offset<Group> >& groupbuffers) const;
  std::pair< Error, std::vector<USER> > GetUsersBuffer(const flatbuffers::Vector< flatbuffers::Offset<User> >& userbuffers) const;
  std::pair< Error, std::vector<FILE> > GetFilesBuffer(const flatbuffers::Vector< flatbuffers::Offset<File> >& filebuffers) const;
  std::pair< Error, std::vector<RECEIVER> > GetReceiversBuffer(const flatbuffers::Vector< flatbuffers::Offset<Receiver> >& receiverbuffers) const;
  std::pair< Error, std::vector<RECORDING> > GetRecordingsBuffer(const flatbuffers::Vector< flatbuffers::Offset<Recording> >& recordingbuffers) const;

  template<class T>
  void HandleError(Signal<Client, T>& signal, const uint16_t sequence, const char* data, const size_t datasize)
  {
    if (!flatbuffers::Verifier(reinterpret_cast<const uint8_t*>(data), datasize).VerifyBuffer<ErrorResponse>(nullptr))
    {
      signal.Response(sequence, T(Error(ErrorCode::InvalidMessage, std::string(typeid(T).name()) + " Verification failed")));
      return;
    }

    const ErrorResponse* errorresponse = flatbuffers::GetRoot<ErrorResponse>(data);
    std::string text;
    if (errorresponse->text())
    {
      text = errorresponse->text()->str();

    }
    signal.Response(sequence, T(Error(errorresponse->code(), text)));
  }

  template<class T>
  std::vector<T> ToVector(const flatbuffers::Vector<T>& vec) const
  {
    std::vector<T> result;
    for (const T& element : vec)
    {
      result.push_back(element);

    }
    return result;
  }

  std::vector<INDEX> ToVector(const flatbuffers::Vector<const INDEX*>& indices) const;

  boost::asio::io_service& io_;
  boost::asio::io_service::strand strand_;
  boost::shared_ptr<sock::TcpSocket> socket_;
  boost::asio::streambuf buffer_;

  mutable std::recursive_mutex mutex_;

  uint16_t sequence_;

  Signal<Client, ADDFILERESPONSE> addfile_;
  Signal<Client, ADDGROUPRESPONSE> addgroup_;
  Signal<Client, ADDMAPRESPONSE> addmap_;
  Signal<Client, ADDONVIFUSERRESPONSE> addonvifuser_;
  Signal<Client, ADDRECEIVERRESPONSE> addreceiver_;
  Signal<Client, ADDRECORDINGRESPONSE> addrecording_;
  Signal<Client, ADDRECORDINGJOBRESPONSE> addrecordingjob_;
  Signal<Client, ADDTRACKRESPONSE> addtrack_;
  Signal<Client, ADDUSERRESPONSE> adduser_;
  Signal<Client, AUTHENTICATERESPONSE> authenticate_;
  Signal<Client, CHANGEGROUPRESPONSE> changegroup_;
  Signal<Client, CHANGEMAPRESPONSE> changemap_;
  Signal<Client, CHANGEONVIFUSERRESPONSE> changeonvifuser_;
  Signal<Client, CHANGERECEIVERRESPONSE> changereceiver_;
  Signal<Client, CHANGERECORDINGRESPONSE> changerecording_;
  Signal<Client, CHANGERECORDINGJOBRESPONSE> changerecordingjob_;
  Signal<Client, CHANGETRACKRESPONSE> changetrack_;
  Signal<Client, CHANGEUSERRESPONSE> changeuser_;
  Signal<Client, CONTROLSTREAMRESPONSE> controlstream_;
  Signal<Client, CONTROLSTREAMRESPONSE> controlstreamframestep_;
  Signal<Client, CONTROLSTREAMRESPONSE> controlstreamlive_;
  Signal<Client, CONTROLSTREAMRESPONSE> controlstreampause_;
  Signal<Client, CREATEFINDMOTIONRESPONSE> createfindmotion_;
  Signal<Client, CREATESTREAMRESPONSE> createstream_;
  Signal<Client, DESTROYFINDMOTIONRESPONSE> destroyfindmotion_;
  Signal<Client, DESTROYSTREAMRESPONSE> destroystream_;
  Signal<Client, DISCOVERYBROADCASTRESPONSE> discoverybroadcast_;
  Signal<Client, GETAUTHENTICATIONNONCERESPONSE> getauthenticationnonce_;
  Signal<Client, GETCHILDFOLDERSRESPONSE> getchildfolders_;
  Signal<Client, GETFILESRESPONSE> getfiles_;
  Signal<Client, GETRECEIVERSRESPONSE> getreceivers_;
  Signal<Client, GETRECORDINGSRESPONSE> getrecordings_;
  Signal<Client, GETSNAPSHOTRESPONSE> getsnapshot_;
  Signal<Client, GETSTATERESPONSE> getstate_;
  Signal<Client, GETTIMERESPONSE> gettime_;
  Signal<Client, KEEPALIVERESPONSE> keepalive_;
  Signal<Client, MOUNTFILERESPONSE> mountfile_;
  Signal<Client, REMOVEFILERESPONSE> removefile_;
  Signal<Client, REMOVEGROUPRESPONSE> removegroup_;
  Signal<Client, REMOVEMAPRESPONSE> removemap_;
  Signal<Client, REMOVEONVIFUSERRESPONSE> removeonvifuser_;
  Signal<Client, REMOVERECEIVERRESPONSE> removereceiver_;
  Signal<Client, REMOVERECORDINGJOBRESPONSE> removerecordingjob_;
  Signal<Client, REMOVERECORDINGJOBSOURCERESPONSE> removerecordingjobsource_;
  Signal<Client, REMOVERECORDINGRESPONSE> removerecording_;
  Signal<Client, REMOVETRACKRESPONSE> removetrack_;
  Signal<Client, REMOVEUSERRESPONSE> removeuser_;
  Signal<Client, SETLOCATIONRESPONSE> setlocation_;
  Signal<Client, SETNAMERESPONSE> setname_;
  Signal<Client, SUBSCRIBERESPONSE> subscribe_;
  Signal<Client, SUBSCRIBEDISCOVERYRESPONSE> subscribediscovery_;
  Signal<Client, SUBSCRIBEHARDWARESTATSRESPONSE> subscribehardwarestats_;
  Signal<Client, SUBSCRIBERECORDINGJOBLOGRESPONSE> subscriberecordingjoblog_;
  Signal<Client, SUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE> subscriberecordingjobsourcetracklog_;
  Signal<Client, SUBSCRIBERECORDINGLOGRESPONSE> subscriberecordinglog_;
  Signal<Client, SUBSCRIBERECORDINGTRACKLOGRESPONSE> subscriberecordingtracklog_;
  Signal<Client, UNMOUNTFILERESPONSE> unmountfile_;
  Signal<Client, UNSUBSCRIBEDISCOVERYRESPONSE> unsubscribediscovery_;
  Signal<Client, UNSUBSCRIBEHARDWARESTATSRESPONSE> unsubscribehardwarestats_;
  Signal<Client, UNSUBSCRIBERECORDINGJOBLOGRESPONSE> unsubscriberecordingjoblog_;
  Signal<Client, UNSUBSCRIBERECORDINGJOBSOURCETRACKLOGRESPONSE> unsubscriberecordingjobsourcetracklog_;
  Signal<Client, UNSUBSCRIBERECORDINGLOGRESPONSE> unsubscriberecordinglog_;
  Signal<Client, UNSUBSCRIBERECORDINGTRACKLOGRESPONSE> unsubscriberecordingtracklog_;

  flatbuffers::FlatBufferBuilder fbb_;

  z_stream inflate_;
  std::vector<char> inflatebuffer_;

};

}

}

#endif