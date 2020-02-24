// testserver.hpp
//

#ifndef IDHDWTLPP6M05FF72MXCOXZ4YWZOJ6O51N
#define IDHDWTLPP6M05FF72MXCOXZ4YWZOJ6O51N

///// Includes /////

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <monocleprotocol/server/connection.hpp>
#include <stdint.h>
#include <vector>

///// Namespaces /////

namespace monocle
{

namespace server
{

namespace tests
{

///// Structures /////

struct TESTFRAME
{
  TESTFRAME(const uint64_t stream, const uint64_t playrequest, const uint64_t codec, const uint64_t timestamp, const int64_t sequencenum, const std::vector<uint8_t>& signature, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const std::vector<char>& data);

  bool operator==(const TESTFRAME& rhs) const;

  uint64_t stream_;
  uint64_t playrequest_;
  uint64_t codec_;
  uint64_t timestamp_;
  uint64_t sequencenum_;
  std::vector<uint8_t> signature_;
  uint16_t restartinterval_;
  uint32_t typespecificfragmentoffset_;
  uint8_t type_;
  uint8_t q_;
  uint8_t width_;
  uint8_t height_;
  uint8_t lqt_[64];
  uint8_t cqt_[64];
  std::vector<char> data_;

};

///// Classes /////

class TestConnection : public server::Connection
{
 public:

  TestConnection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const std::string& testusername, const std::string& testpassword, const std::string& testnonce, const STREAM& teststream, const std::string& testname, const std::string& testpublickey, const std::string& testarchitecture, const int testoperatingsystem, const std::string& testcompiler, const std::string& testdatabasepath, const utility::Version& testversion, const uint64_t testidentifier, const std::vector<std::string>& testenvironmentvariables, const std::vector<std::string>& testcommandlinevariables, const std::vector<ONVIFUSER>& testonvifusers, const std::vector<GROUP>& testgroups, const std::vector<USER>& testusers, const std::vector<FILE>& testfiles, const std::vector<RECEIVER>& testreceivers, const std::vector<RECORDING>& testrecordings, const std::vector<monocle::LOGMESSAGE>& testrecordinglogmessages, const uint32_t testmaxrecordings, const std::vector<MAP>& testmaps, const std::vector<MOUNTPOINT>& testmountpoints, const std::string& testlatitude, const std::string& testlongitude, const int testnumcudadevices, const int testnumcldevices, const int testmaxobjectdetectors, const TESTFRAME& testframe);
  virtual ~TestConnection();

  virtual boost::system::error_code Connected() override;
  virtual void Disconnected() override;
  virtual Error AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount) override;
  virtual Error AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) override;
  virtual Error AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image) override;
  virtual Error AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel) override;
  virtual Error AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) override;
  virtual std::pair<Error, uint64_t> AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjobs) override;
  virtual std::pair<Error, uint64_t> AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<monocle::ADDRECORDINGJOBSOURCE>& sources) override;
  virtual std::pair<Error, uint32_t> AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files) override;
  virtual Error AddTrack2(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters) override;
  virtual Error AddUser(const std::string& username, const std::string& digest, const uint64_t group) override;
  virtual std::pair<Error, AUTHENTICATERESPONSE> Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest) override;
  virtual Error ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings) override;
  virtual Error ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image) override;
  virtual Error ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const ONVIFUserlevel onvifuserlevel) override;
  virtual Error ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) override;
  virtual Error ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime) override;
  virtual Error ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources) override;
  virtual Error ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files) override;
  virtual Error ChangeTrack2(const uint64_t recordingtoken, const uint32_t trackid, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint32_t objectdetectortrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters) override;
  virtual Error ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group) override;
  virtual Error ControlStream(const uint64_t streamtoken, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const boost::optional<bool>& forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes) override;
  virtual Error ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum) override;
  virtual Error ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequestindex) override;
  virtual Error ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time) override;
  virtual std::pair<Error, uint64_t> CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast) override;
  virtual std::pair<Error, uint64_t> CreateFindObject(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height) override;
  virtual std::pair<Error, STREAM> CreateStream(const uint64_t recordingtoken, const uint64_t tracktoken) override;
  virtual Error DestroyFindMotion(const uint64_t token) override;
  virtual Error DestroyFindObject(const uint64_t token) override;
  virtual Error DestroyStream(const uint64_t streamtoken) override;
  virtual Error DiscoveryBroadcast() override;
  virtual std::string GetAuthenticationNonce() override;
  virtual std::pair< Error, std::vector<std::string> > GetChildFolders(const std::string& path, const bool parentpaths) override;
  virtual std::pair< Error, std::vector<FILE> > GetFiles() override;
  virtual std::pair< Error, std::vector<RECEIVER> > GetReceivers() override;
  virtual std::pair<Error, monocle::RECORDING> GetRecording(const uint64_t token) override;
  virtual std::pair< Error, std::vector<RECORDING> > GetRecordings() override;
  virtual std::pair<Error, SNAPSHOT> GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, const float x, const float y, const float width, const float height) override;
  virtual std::pair<Error, GETSTATE> GetState() override;
  virtual uint64_t GetTime() override;
  virtual Error SetLocation(const std::string& latitude, const std::string& longitude) override;
  virtual Error MountFile(const uint64_t token) override;
  virtual Error RemoveFile(const uint64_t token) override;
  virtual Error RemoveGroup(const uint64_t token) override;
  virtual Error RemoveMap(const uint64_t token) override;
  virtual Error RemoveONVIFUser(const uint64_t token) override;
  virtual Error RemoveReceiver(const uint64_t token) override;
  virtual Error RemoveRecording(const uint64_t token) override;
  virtual Error RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token) override;
  virtual Error RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token) override;
  virtual Error RemoveTrack(const uint64_t recordingtoken, const uint32_t id) override;
  virtual Error RemoveTracks(const uint64_t recordingtoken, const std::vector<uint32_t>& ids) override;
  virtual Error RemoveUser(const uint64_t token) override;
  virtual Error SetName(const std::string& name) override;
  virtual std::pair<Error, SUBSCRIBE> Subscribe() override;
  virtual Error SubscribeDiscovery(const bool broadcast) override;
  virtual std::pair<Error, HARDWARESTATS> SubscribeHardwareStats() override;
  virtual std::pair<Error, SUBSCRIBERECORDINGJOBLOG> SubscribeRecordingJobLog(const uint64_t recordingtoken) override;
  virtual std::pair<Error, SUBSCRIBERECORDINGJOBSOURCETRACKLOG> SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken) override;
  virtual std::pair<Error, SUBSCRIBERECORDINGLOG> SubscribeRecordingLog(const uint64_t token) override;
  virtual std::pair<Error, SUBSCRIBERECORDINGTRACKLOG> SubscribeRecordingTrackLog(const uint64_t recordingtoken) override;
  virtual Error UnmountFile(const uint64_t token) override;
  virtual Error UnsubscribeDiscovery() override;
  virtual Error UnsubscribeHardwareStats() override;
  virtual void UnsubscribeRecordingJobLog(const uint64_t recordingtoken) override;
  virtual void UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken) override;
  virtual void UnsubscribeRecordingLog(const uint64_t token) override;
  virtual void UnsubscribeRecordingTrackLog(const uint64_t recordingtoken) override;

  virtual bool Permission(const Message message) override;

 protected:


 private:

  void FrameCallback(const boost::system::error_code& e);

  const std::string testusername_;
  const std::string testpassword_;
  const std::string testnonce_;
  const STREAM teststream_;
  const std::string testname_;
  const std::string testpublickey_;
  const std::string testarchitecture_;
  const int testoperatingsystem_;
  const std::string testcompiler_;
  const std::string testdatabasepath_;
  const utility::Version testversion_;
  const uint64_t testidentifier_;
  const std::vector<std::string> testenvironmentvariables_;
  const std::vector<std::string> testcommandlinevariables_;
  const std::vector<ONVIFUSER> testonvifusers_;
  const std::vector<GROUP> testgroups_;
  const std::vector<USER> testusers_;
  const std::vector<FILE> testfiles_;
  const std::vector<RECEIVER> testreceivers_;
  const std::vector<RECORDING> testrecordings_;
  const std::vector<monocle::LOGMESSAGE> testrecordinglogmessages_;
  const uint32_t testmaxrecordings_;
  const std::vector<MAP> testmaps_;
  const std::vector<MOUNTPOINT> testmountpoints_;
  const std::string testlatitude_;
  const std::string testlongitude_;
  const int testnumcudadevices_;
  const int testnumcldevices_;
  const int testmaxobjectdetectors_;
  const TESTFRAME testframe_;

  mutable std::recursive_mutex mutex_;
  boost::asio::deadline_timer frames_;


};

}

}

}

#endif
