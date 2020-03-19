// testconnection.cpp
//

///// Includes /////

#include <boost/asio.hpp>

#include <boost/bind.hpp>
#include <chrono>
#include <gtest/gtest.h>
#include <monocleprotocol/monocleprotocol.hpp>
#include <utility/utility.hpp>

#include "testconnection.hpp"

///// Namespaces /////

namespace monocle
{

namespace server
{

namespace tests
{

///// Methods /////

TESTFRAME::TESTFRAME(const uint64_t stream, const uint64_t playrequest, const uint64_t codec, const uint64_t timestamp, const int64_t sequencenum, const std::vector<uint8_t>& signature, const uint16_t restartinterval, const uint32_t typespecificfragmentoffset, const uint8_t type, const uint8_t q, const uint8_t width, const uint8_t height, const uint8_t* lqt, const uint8_t* cqt, const std::vector<char>& data) :
  stream_(stream),
  playrequest_(playrequest),
  codec_(codec),
  timestamp_(timestamp),
  sequencenum_(sequencenum),
  signature_(signature),
  restartinterval_(restartinterval),
  typespecificfragmentoffset_(typespecificfragmentoffset),
  type_(type),
  q_(q),
  width_(width),
  height_(height),
  data_(data)
{
  if (lqt && cqt)
  {
    memcpy(lqt_, lqt, sizeof(lqt_));
    memcpy(cqt_, cqt, sizeof(cqt_));
  }
  else
  {
    memset(lqt_, 0, sizeof(lqt_));
    memset(cqt_, 0, sizeof(cqt_));
  }
}

bool TESTFRAME::operator==(const TESTFRAME& rhs) const
{
  return ((stream_ == rhs.stream_) && (playrequest_ == rhs.playrequest_) && (codec_ == rhs.codec_) && (timestamp_ == rhs.timestamp_) && (sequencenum_ == rhs.sequencenum_) && (signature_ == rhs.signature_) && (data_ == rhs.data_));
}

TestConnection::TestConnection(boost::asio::io_service& io, const boost::shared_ptr<Server>& server, const std::string& testusername, const std::string& testpassword, const std::string& testnonce, const STREAM& teststream, const std::string& testname, const std::string& testpublickey, const std::string& testarchitecture, const int testoperatingsystem, const std::string& testcompiler, const std::string& testdatabasepath, const utility::Version& testversion, const uint64_t testidentifier, const std::vector<std::string>& testenvironmentvariables, const std::vector<std::string>& testcommandlinevariables, const std::vector<ONVIFUSER>& testonvifusers, const std::vector<GROUP>& testgroups, const std::vector<USER>& testusers, const std::vector<FILE>& testfiles, const std::vector<RECEIVER>& testreceivers, const std::vector<RECORDING>& testrecordings, const std::vector<monocle::LOGMESSAGE>& testrecordinglogmessages, const uint32_t testmaxrecordings, const std::vector<LAYOUT>& testlayouts, const std::vector<MAP>& testmaps, const std::vector<MOUNTPOINT>& testmountpoints, const std::string& testlatitude, const std::string& testlongitude, const int testnumcudadevices, const int testnumcldevices, const int testmaxobjectdetectors, const TESTFRAME& testframe) :
  Connection(io, server),
  testusername_(testusername),
  testpassword_(testpassword),
  testnonce_(testnonce),
  teststream_(teststream),
  testname_(testname),
  testpublickey_(testpublickey),
  testarchitecture_(testarchitecture),
  testoperatingsystem_(testoperatingsystem),
  testcompiler_(testcompiler),
  testdatabasepath_(testdatabasepath),
  testversion_(testversion),
  testidentifier_(testidentifier),
  testenvironmentvariables_(testenvironmentvariables),
  testcommandlinevariables_(testcommandlinevariables),
  testonvifusers_(testonvifusers),
  testgroups_(testgroups),
  testusers_(testusers),
  testfiles_(testfiles),
  testreceivers_(testreceivers),
  testrecordings_(testrecordings),
  testrecordinglogmessages_(testrecordinglogmessages),
  testmaxrecordings_(testmaxrecordings),
  testlayouts_(testlayouts),
  testmaps_(testmaps),
  testmountpoints_(testmountpoints),
  testlatitude_(testlatitude),
  testlongitude_(testlongitude),
  testframe_(testframe),
  testnumcudadevices_(testnumcudadevices),
  testnumcldevices_(testnumcldevices),
  testmaxobjectdetectors_(testmaxobjectdetectors),
  frames_(socket_.get_io_service())
{

}

TestConnection::~TestConnection()
{

}

boost::system::error_code TestConnection::Connected()
{
  // Send frames continuously
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  boost::system::error_code err;
  frames_.expires_from_now(boost::posix_time::milliseconds(100), err);
  if (err)
  {

    return err;
  }
  frames_.async_wait(boost::bind(&TestConnection::FrameCallback, boost::static_pointer_cast<TestConnection>(shared_from_this()), boost::asio::placeholders::error));
  return Connection::Connected();
}

void TestConnection::Disconnected()
{
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  boost::system::error_code err;
  frames_.cancel(err);
}

Error TestConnection::AddFile(const std::string& mountpoint, const std::string& path, const bool filldisk, const uint64_t numchunks, const uint64_t chunksize, const bool automount)
{
  return Error();
}

Error TestConnection::AddGroup(const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  return Error();
}

Error TestConnection::AddLayout(const LAYOUT& layout)
{
  return Error();
}

Error TestConnection::AddMap(const std::string& name, const std::string& location, const std::vector<int8_t>& image)
{
  return Error();
}

Error TestConnection::AddONVIFUser(const std::string& username, const std::string& password, const ONVIFUserlevel onvifuserlevel)
{
  return Error();
}

Error TestConnection::AddReceiver(const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  return Error();
}

std::pair<Error, uint64_t> TestConnection::AddRecording(const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime, const bool createdefaulttracks, const bool createdefaultjobs)
{
  return std::make_pair(Error(), 0);
}

std::pair<Error, uint64_t> TestConnection::AddRecordingJob(const uint64_t recordingtoken, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<monocle::ADDRECORDINGJOBSOURCE>& sources)
{
  return std::make_pair(Error(), 0);
}

std::pair<Error, uint32_t> TestConnection::AddTrack(const uint64_t recordingtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files)
{
  return std::make_pair(Error(), 0);
}

Error TestConnection::AddTrack2(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters)
{
  return Error();
}

Error TestConnection::AddUser(const std::string& username, const std::string& digest, const uint64_t group)
{
  return Error();
}

std::pair<Error, AUTHENTICATERESPONSE> TestConnection::Authenticate(const std::string& username, const std::string& clientnonce, const std::string& authdigest)
{
  if ((testusername_.size() != username.size()) || !std::equal(testusername_.begin(), testusername_.end(), username.begin(), username.end()))
  {

    return std::make_pair(Error(ErrorCode::BadAuth, std::string()), AUTHENTICATERESPONSE());
  }

  if (AuthenticateDigest(testusername_, testpassword_, clientnonce) != authdigest)
  {

    return std::make_pair(Error(ErrorCode::BadAuth, std::string()), AUTHENTICATERESPONSE());
  }

  return std::make_pair(Error(ErrorCode::Success, std::string()), AUTHENTICATERESPONSE());
}

Error TestConnection::ChangeGroup(const uint64_t token, const std::string& name, const bool manageusers, const bool managerecordings, const bool managemaps, const bool managedevice, const bool allrecordings, const std::vector<uint64_t>& recordings)
{
  return Error();
}

Error TestConnection::ChangeLayout(const LAYOUT& layout)
{
  return Error();
}

Error TestConnection::ChangeLayoutName(const uint64_t token, const std::string& name)
{
  return Error();
}

Error TestConnection::ChangeMap(const uint64_t token, const std::string& name, const std::string& location, const std::vector<int8_t>& image)
{
  return Error();
}

Error TestConnection::ChangeONVIFUser(const uint64_t token, const boost::optional<std::string>& username, const boost::optional<std::string>& password, const ONVIFUserlevel onvifuserlevel)
{
  return Error();
}

Error TestConnection::ChangeReceiver(const uint64_t token, const monocle::ReceiverMode mode, const std::string& uri, const std::string& username, const std::string& password, const std::vector<std::string>& parameters)
{
  return Error();
}

Error TestConnection::ChangeRecording(const uint64_t token, const std::string& sourceid, const std::string& name, const std::string& location, const std::string& description, const std::string& address, const std::string& content, const uint64_t retentiontime)
{
  return Error();
}

Error TestConnection::ChangeRecordingJob(const uint64_t recordingtoken, const uint64_t token, const std::string& name, const bool enabled, const uint64_t priority, const std::vector<CHANGERECORDINGJOBSOURCE>& sources)
{
  return Error();
}

Error TestConnection::ChangeTrack(const uint64_t recordingtoken, const uint32_t id, const monocle::TrackType tracktype, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files)
{
  return Error();
}

Error TestConnection::ChangeTrack2(const uint64_t recordingtoken, const uint32_t trackid, const uint64_t recordingjobtoken, const uint64_t recordingjobsourcetoken, const uint64_t recordingjobsourcetracktoken, const uint32_t objectdetectortrackid, const uint64_t objectdetectorrecordingjobsourcetoken, const uint64_t objectdetectorrecordingjobsourcetracktoken, const std::string& description, const bool fixedfiles, const bool digitalsigning, const bool encrypt, const uint32_t flushfrequency, const std::vector<uint64_t>& files, const std::string& mediauri, const std::string& username, const std::string& password, const std::vector<std::string>& receiverparameters, const std::vector<std::string>& sourceparameters, const std::vector<std::string>& objectdetectorsourceparameters)
{
  return Error();
}

Error TestConnection::ChangeUser(const uint64_t token, const boost::optional<std::string>& digest, const uint64_t group)
{
  return Error();
}

Error TestConnection::ControlStream(const uint64_t streamtoken, const uint64_t playrequestindex, const bool fetchmarker, const bool ratecontrol, const boost::optional<bool>& forwards, const boost::optional<uint64_t>& starttime, const boost::optional<uint64_t>& endtime, const boost::optional<uint64_t>& numframes, const bool iframes)
{
  return Error();
}

Error TestConnection::ControlStreamFrameStep(const uint64_t streamtoken, const uint64_t playrequestindex, const bool forwards, const uint64_t sequencenum)
{
  return Error();
}

Error TestConnection::ControlStreamLive(const uint64_t streamtoken, const uint64_t playrequestindex)
{
  return Error();
}

Error TestConnection::ControlStreamPause(const uint64_t streamtoken, const boost::optional<uint64_t>& time)
{
  return Error();
}

std::string TestConnection::GetAuthenticationNonce()
{
  return testnonce_;
}

std::pair< Error, std::vector<std::string> > GetChildFolders(const std::string& path)
{
  return std::make_pair(Error(), std::vector<std::string>());
}

std::pair<Error, uint64_t> TestConnection::CreateFindMotion(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height, const float sensitivity, const bool fast)
{
  return std::make_pair(Error(), 0);
}

std::pair<Error, uint64_t> TestConnection::CreateFindObject(const uint64_t recordingtoken, const uint32_t tracktoken, const uint64_t starttime, const uint64_t endtime, const float x, const float y, const float width, const float height)
{
  return std::make_pair(Error(), 0);
}

std::pair<Error, STREAM> TestConnection::CreateStream(const uint64_t recordingtoken, const uint64_t tracktoken)
{
  return std::make_pair(Error(), teststream_);
}

Error TestConnection::DestroyFindMotion(const uint64_t token)
{
  return Error();
}

Error TestConnection::DestroyFindObject(const uint64_t token)
{
  return Error();
}

Error TestConnection::DestroyStream(const uint64_t streamtoken)
{
  return Error();
}

Error TestConnection::DiscoveryBroadcast()
{
  return Error();
}

std::pair< Error, std::vector<std::string> > TestConnection::GetChildFolders(const std::string& path, const bool parentpaths)
{
  return std::make_pair(Error(), std::vector<std::string>());
}

std::pair< Error, std::vector<FILE> > TestConnection::GetFiles()
{
  return std::make_pair(Error(), testfiles_);
}

std::pair< Error, std::vector<RECEIVER> > TestConnection::GetReceivers()
{
  return std::make_pair(Error(), testreceivers_);
}

std::pair<Error, monocle::RECORDING> TestConnection::GetRecording(const uint64_t token)
{
  return std::make_pair(Error(), monocle::RECORDING());
}

std::pair< Error, std::vector<RECORDING> > TestConnection::GetRecordings()
{
  return std::make_pair(Error(), testrecordings_);
}

std::pair<Error, SNAPSHOT> TestConnection::GetSnapshot(const uint64_t recordingtoken, const uint32_t recordingtrackid, const uint64_t time, const float x, const float y, const float width, const float height)
{
  return std::make_pair(Error(), SNAPSHOT());
}

std::pair<Error, GETSTATE> TestConnection::GetState()
{
  return std::make_pair(Error(), GETSTATE());
}

uint64_t TestConnection::GetTime()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Error TestConnection::SetLocation(const std::string& latitude, const std::string& longitude)
{
  return Error();
}

Error TestConnection::MountFile(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveFile(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveLayout(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveGroup(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveMap(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveONVIFUser(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveReceiver(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveRecording(const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveRecordingJob(const uint64_t recordingtoken, const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveRecordingJobSource(const uint64_t recordingtoken, const uint64_t recordingjobtoken, const uint64_t token)
{
  return Error();
}

Error TestConnection::RemoveTrack(const uint64_t recordingtoken, const uint32_t id)
{
  return Error();
}

Error TestConnection::RemoveTracks(const uint64_t recordingtoken, const std::vector<uint32_t>& ids)
{
  return Error();
}

Error TestConnection::RemoveUser(const uint64_t token)
{
  return Error();
}

Error TestConnection::SetName(const std::string& name)
{
  return Error();
}

std::pair<Error, SUBSCRIBE> TestConnection::Subscribe()
{
  return std::make_pair(Error(), SUBSCRIBE(testname_, testpublickey_, testarchitecture_, testoperatingsystem_, testcompiler_, testdatabasepath_, testversion_, testidentifier_, testenvironmentvariables_, testcommandlinevariables_, testonvifusers_, testgroups_, testusers_, testfiles_, testreceivers_, testrecordings_, {}, testmaxrecordings_, testlayouts_, testmaps_, testmountpoints_, testlatitude_, testlongitude_, testnumcudadevices_, testnumcldevices_, testmaxobjectdetectors_));
}

Error TestConnection::SubscribeDiscovery(const bool broadcast)
{
  return Error();
}

std::pair<Error, HARDWARESTATS> TestConnection::SubscribeHardwareStats()
{
  return std::make_pair(Error(), HARDWARESTATS());
}

std::pair<Error, SUBSCRIBERECORDINGJOBLOG> TestConnection::SubscribeRecordingJobLog(const uint64_t recordingtoken)
{
  return std::make_pair(Error(), SUBSCRIBERECORDINGJOBLOG());
}

std::pair<Error, SUBSCRIBERECORDINGJOBSOURCETRACKLOG> TestConnection::SubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken)
{
  return std::make_pair(Error(), SUBSCRIBERECORDINGJOBSOURCETRACKLOG());
}

std::pair<Error, SUBSCRIBERECORDINGLOG> TestConnection::SubscribeRecordingLog(const uint64_t token)
{
  return std::make_pair(Error(), SUBSCRIBERECORDINGLOG(testrecordinglogmessages_));
}

std::pair<Error, SUBSCRIBERECORDINGTRACKLOG> TestConnection::SubscribeRecordingTrackLog(const uint64_t recordingtoken)
{
  return std::make_pair(Error(), SUBSCRIBERECORDINGTRACKLOG());
}

Error TestConnection::UnmountFile(const uint64_t token)
{
  return Error();
}

Error TestConnection::UnsubscribeDiscovery()
{
  return Error();
}

Error TestConnection::UnsubscribeHardwareStats()
{
  return Error();
}

void TestConnection::UnsubscribeRecordingJobLog(const uint64_t recordingtoken)
{

}

void TestConnection::UnsubscribeRecordingJobSourceTrackLog(const uint64_t recordingtoken)
{

}

void TestConnection::UnsubscribeRecordingLog(const uint64_t token)
{

}

void TestConnection::UnsubscribeRecordingTrackLog(const uint64_t recordingtoken)
{

}

bool TestConnection::Permission(const Message message)
{

  return true;
}

void TestConnection::FrameCallback(const boost::system::error_code& e)
{
  if (e)
  {

    return;
  }

  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (SendJPEGFrame(testframe_.stream_, testframe_.playrequest_, testframe_.codec_, testframe_.timestamp_, testframe_.sequencenum_, 1.0f,  testframe_.signature_.data(), testframe_.signature_.size(), testframe_.restartinterval_, testframe_.typespecificfragmentoffset_, testframe_.type_, testframe_.q_, testframe_.width_, testframe_.height_, testframe_.lqt_, testframe_.cqt_, testframe_.data_.data(), testframe_.data_.size()))
  {

    return;
  }
  boost::system::error_code err;
  frames_.expires_from_now(boost::posix_time::milliseconds(100), err);
  if (err)
  {

    return;
  }
  frames_.async_wait(boost::bind(&TestConnection::FrameCallback, this, boost::asio::placeholders::error));
}

}

}

}
