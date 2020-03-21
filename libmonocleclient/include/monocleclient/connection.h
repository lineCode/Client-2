// connection.h+
//

#ifndef IDFMKGWV9SDFNDSFJS2SCCDCDC366BC609
#define IDFMKGWV9SDFNDSFJS2SCCDCDC366BC609

///// Includes /////

#include <monocleprotocol/client/client.hpp>
#include <socket/proxyparams.hpp>
#include <QObject>
#include <QString>
#include <vector>

#include "stream.h"

///// Namespaces /////

namespace client
{

///// Declarations /////

enum class MetadataFrameType : uint16_t;
enum class ObjectDetectorFrameType : uint16_t;

///// Classes /////

class Connection : public QObject, public monocle::client::Client
{
 Q_OBJECT

 public:

  Connection(boost::asio::io_service& io, const sock::ProxyParams& proxyparams, const QString& address, const uint16_t port);
  virtual ~Connection();

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
  virtual void GuiOrderChanged(const std::vector< std::pair<uint64_t, uint64_t> >& recordingsorder, const std::vector< std::pair<uint64_t, uint64_t> >& mapsorder) override;
  virtual void H265Frame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const bool donlfield, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) override;
  virtual void H264Frame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint32_t* offsets, const size_t numoffsets, const char* data, const size_t size) override;
  virtual void HardwareStatsMessage(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats) override;
  virtual void JPEGFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const char* data, const size_t size) override;
  virtual void LayoutAdded(const monocle::LAYOUT& layout) override;
  virtual void LayoutChanged(const monocle::LAYOUT& layout) override;
  virtual void LayoutNameChanged(const uint64_t token, const std::string& name) override;
  virtual void LayoutRemoved(const uint64_t token) override;
  virtual void MapAdded(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) override;
  virtual void MapChanged(const uint64_t token, const std::string& name, const std::string& location, const std::string& imagemd5) override;
  virtual void MapRemoved(const uint64_t token) override;
  virtual void MetadataFrame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::MetadataFrameType metadataframetype, const char* data, const size_t size) override;
  virtual void MountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) override;
  virtual void MountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const std::string& path, const std::string& type, const std::string& source) override;
  virtual void MPEG4Frame(const uint64_t token, const uint64_t playrequestindex, const uint64_t codecindex, const bool marker, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const char* data, const size_t size) override;
  virtual void LocationChanged(const std::string& latitude, const std::string& longitude) override;
  virtual void NameChanged(const std::string& namechanged) override;
  virtual void NewCodecIndex(const uint64_t token, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp) override;
  virtual void ObjectDetectorFrame(const uint64_t token, const uint64_t playrequest, const uint64_t codecindex, const uint64_t timestamp, const int64_t sequencenum, const float progress, const uint8_t* signature, const size_t signaturesize, const monocle::ObjectDetectorFrameType objectdetectorframetype, const char* data, const size_t size) override;
  virtual void ONVIFUserAdded(const uint64_t token, const std::string& username, const monocle::ONVIFUserlevel onvifuserlevel) override;
  virtual void ONVIFUserChanged(const uint64_t token, const boost::optional<std::string>& username, const monocle::ONVIFUserlevel onvifuserlevel) override;
  virtual void ONVIFUserRemoved(const uint64_t token) override;
  virtual void ReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters, const monocle::ReceiverState state) override;
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
  virtual void RecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<std::string>& parameters, const monocle::RecordingJobState state, const std::string& error, const std::vector<std::string>& activeparameters) override;
  virtual void RecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token) override;
  virtual void RecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const monocle::RecordingJobState state, const std::string& error) override;
  virtual void RecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp) override;
  virtual void RecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id) override;
  virtual void RecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void RecordingRemoved(const uint64_t token) override;
  virtual void RecordingsStatistics(const uint64_t time, const std::vector<monocle::RECORDINGSTATISTICS>& recordingsstatistics) override;
  virtual void ServerLogMessage(const uint64_t time, const monocle::Severity severity, const std::string& message) override;
  virtual void TrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::pair<uint32_t, uint64_t>& totaltrackdata) override;
  virtual void TrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::pair<uint32_t, uint64_t>& totaltrackdata) override;
  virtual void TrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end) override;
  virtual void TrackRemoved(const uint64_t recordingtoken, const uint32_t token) override;
  virtual void TrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& times) override;
  virtual void UserAdded(const uint64_t token, const std::string& username, const uint64_t group) override;
  virtual void UserChanged(const uint64_t token, const uint64_t group) override;
  virtual void UserRemoved(const uint64_t token) override;

  sock::Connection Connect(const boost::function<void(const boost::system::error_code&)> callback);

  monocle::client::Connection Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::AUTHENTICATERESPONSE&)> callback);
  monocle::client::Connection CreateStream(const uint64_t recordingtoken, const uint64_t tracktoken, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::CREATESTREAMRESPONSE&)> callback, const CONTROLSTREAMEND controlstreamendcallback, const H265CALLBACK h265callback, const H264CALLBACK h264callback, const METADATACALLBACK metadatacallback, const JPEGCALLBACK jpegcallback, const MPEG4CALLBACK mpeg4callback, const OBJECTDETECTORCALLBACK objectdetectorcallback, const NEWCODECINDEX newcodecindexcallback, void* callbackdata);
  monocle::client::Connection DestroyStream(const uint64_t streamtoken, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::DESTROYSTREAMRESPONSE&)> callback);
  monocle::client::Connection GetAuthenticationNonce(boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::GETAUTHENTICATIONNONCERESPONSE&)> callback);
  monocle::client::Connection RemoveFile(const uint64_t token, boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::REMOVEFILERESPONSE&)> callback);
  monocle::client::Connection Subscribe(boost::function<void(const std::chrono::steady_clock::duration, const monocle::client::SUBSCRIBERESPONSE&)> callback);

  inline const sock::ProxyParams& GetProxyParams() const { return proxyparams_; }
  inline const QString& GetAddress() const { return address_; }
  inline uint16_t GetPort() const { return port_; }
  
 protected:

   std::vector<QString> ConvertStrings(const std::vector<std::string>& strings) const;

   sock::ProxyParams proxyparams_;
   QString address_;
   uint16_t port_;

 private:

  std::vector<Stream> streams_;

 signals:

  void SignalDiscoveryHello(const std::vector<std::string>& addresses, const std::vector<std::string>& scopes);
  void SignalFileAdded(const uint64_t token, const std::string& path, const std::string& mountpoint, const uint64_t numchunks, const uint64_t chunksize, const bool automount, const monocle::FileState state, const monocle::FileMonitorState monitorstate);
  void SignalFileRemoved(const uint64_t token);
  void SignalFileMonitorStateChanged(const uint64_t token, const monocle::FileMonitorState monitorstate);
  void SignalFileStateChanged(const uint64_t token, const monocle::FileState state);
  void SignalFindMotionEnd(const uint64_t token, const uint64_t ret);
  void SignalFindMotionProgress(const uint64_t token, const float progress);
  void SignalFindMotionResult(const uint64_t token, const uint64_t start, const uint64_t end);
  void SignalFindObjectEnd(const uint64_t token, const uint64_t ret);
  void SignalFindObjectProgress(const uint64_t token, const float progress);
  void SignalFindObjectResult(const uint64_t token, const uint64_t start, const uint64_t end, const monocle::ObjectClass objectclass, const uint64_t id, const uint64_t largesttime, const float largestx, const float largesty, const float largestwidth, const float largestheight);
  void SignalGroupAdded(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  void SignalGroupChanged(const uint64_t token, const QString& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings);
  void SignalGroupRemoved(const uint64_t token);
  void SignalGuiOrderChanged(const std::vector< std::pair<uint64_t, uint64_t> >& recordingsorder, const std::vector< std::pair<uint64_t, uint64_t> >& mapsorder);
  void SignalHardwareStats(const uint64_t time, const std::vector<monocle::DISKSTAT>& diskstats, const double cpuusage, const uint64_t totalmemory, const uint64_t availablememory, const std::vector<monocle::GPUSTAT>& gpustats);
  void SignalLayoutAdded(const monocle::LAYOUT& layout);
  void SignalLayoutChanged(const monocle::LAYOUT& layout);
  void SignalLayoutNameChanged(const uint64_t token, const QString& name);
  void SignalLayoutRemoved(const uint64_t token);
  void SignalLocationChanged(const QString& latitude, const QString& longitude);
  void SignalMapAdded(const uint64_t token, const QString& name, const QString& location, const QString& imagemd5);
  void SignalMapChanged(const uint64_t token, const QString& name, const QString& location, const QString& imagemd5);
  void SignalMapRemoved(const uint64_t token);
  void SignalMountPointAdded(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source);
  void SignalMountPointRemoved(const uint64_t id, const uint64_t parentid, const uint64_t majorstdev, const uint64_t minorstdev, const QString& path, const QString& type, const QString& source);
  void SignalNameChanged(const QString& name);
  void SignalONVIFUserAdded(const uint64_t token, const QString& username, const monocle::ONVIFUserlevel onvifuserlevel);
  void SignalONVIFUserChanged(const uint64_t token, const boost::optional<QString>& username, const monocle::ONVIFUserlevel onvifuserlevel);
  void SignalONVIFUserRemoved(const uint64_t token);
  void SignalReceiverAdded(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters, const monocle::ReceiverState state);
  void SignalReceiverChanged(const uint64_t token, const monocle::ReceiverMode mode, const QString& mediauri, const bool autocreated, const QString& username, const QString& password, const std::vector<QString>& parameters);
  void SignalReceiverRemoved(const uint64_t token);
  void SignalRecordingActiveJobChanged(const uint64_t token, const boost::optional<uint64_t>& activejob);
  void SignalRecordingAdded(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  void SignalRecordingChanged(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const boost::optional<uint64_t>& activejob);
  void SignalRecordingJobLogMessage(const uint64_t recordingtoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingJobAdded(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const monocle::RecordingJobState state);
  void SignalRecordingJobChanged(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority);
  void SignalRecordingJobRemoved(const uint64_t recordingtoken, const uint64_t token);
  void SignalRecordingJobSourceAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token, const uint64_t receivertoken);
  void SignalRecordingJobSourceRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token);
  void SignalRecordingJobSourceTrackActiveParametersChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const std::vector<QString>& activeparameters);
  void SignalRecordingJobSourceTrackAdded(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint32_t recordingtrackid, const std::vector<QString>& parameters, const monocle::RecordingJobState state, const QString& error, const std::vector<QString>& activeparameters);
  void SignalRecordingJobSourceTrackLogMessage(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingJobSourceTrackRemoved(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t token);
  void SignalRecordingJobSourceTrackStateChanged(const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const monocle::RecordingJobState state, const QString& error);
  void SignalRecordingRemoved(const uint64_t token);
  void SignalRecordingLogMessage(const uint64_t token, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalRecordingStatistics(const uint64_t time, const std::vector<monocle::RECORDINGSTATISTICS>& recordingsstatistics);
  void SignalRecordingTrackCodecAdded(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id, const monocle::Codec codec, const std::string& parameters, const uint64_t timestamp);
  void SignalRecordingTrackCodecRemoved(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t id);
  void SignalRecordingTrackLogMessage(const uint64_t recordingtoken, const uint32_t id, const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalServerLogMessage(const uint64_t time, const monocle::Severity severity, const QString& message);
  void SignalTrackAdded(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::pair<uint64_t, uint64_t>& totaltrackdata);
  void SignalTrackChanged(const uint64_t recordingtoken, const uint32_t id, const std::string& token, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::vector<monocle::CODECINDEX>& codecindices, const std::pair<uint64_t, uint64_t>& totaltrackdata);
  void SignalTrackDeleteData(const uint64_t recording, const uint32_t trackid, const boost::optional<uint64_t>& start, const boost::optional<uint64_t>& end);
  void SignalTrackRemoved(const uint64_t recordingtoken, const uint32_t id);
  void SignalTrackSetData(const uint64_t recording, const uint32_t trackid, const std::vector<monocle::INDEX>& indices);
  void SignalUserAdded(const uint64_t token, const QString& username, const uint64_t group);
  void SignalUserChanged(const uint64_t token, const uint64_t group);
  void SignalUserRemoved(const uint64_t token);

};

}

#endif
