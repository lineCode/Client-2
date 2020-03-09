// testprotocol.cpp
//

///// Includes /////

#include "testprotocol.hpp"

#include <boost/asio.hpp>
#include <future>
#include <gtest/gtest.h>
#include <socket/proxyparams.hpp>
#include <utility/ioservicepool.hpp>
#include <utility/utility.hpp>

#include "main.hpp"

///// Namespaces /////

namespace monocle
{

namespace tests
{

///// Globals /////

const uint16_t PORT = 9856;
const std::string TESTUSERNAME("testusername");
const std::string TESTPASSWORD("testpassword");
const std::string TESTNONCE = utility::GenerateRandomString(32);
const int WAITMS = 2000;

///// Methods /////

TestProtocol::TestProtocol() :
  teststream_(5, std::vector<monocle::CODECINDEX>({ monocle::CODECINDEX(1, monocle::Codec::MJPEG, std::string("parameters"), 5) })),
  testname_("testname"),
  testpublickey_("testpublickey"),
  testarchitecture_("testarchitecture"),
  testoperatingsystem_(5),
  testcompiler_("testcompiler"),
  testdatabasepath_("testdatabasepath"),
  testversion_(1, 2, 3),
  testidentifier_(123456),
  testenvironmentvariables_({ std::string("env1"), std::string("env2") }),
  testcommandlinevariables_({ std::string("cmd1"), std::string("cmd2") }),
  testonvifusers_({ ONVIFUSER(1, "username", monocle::ONVIFUserlevel::Operator) }),
  testgroups_({ GROUP(1, "name", true, false, false, true, true, { 1, 2 }) }),
  testusers_({ USER(1, "username", 2) }),
  testfiles_({ FILE(0, "path", "mountpoint", 1, 2, true, monocle::FileState::Mounted, monocle::FileMonitorState::Available), FILE(1, "path2", "mountpoint2", 3, 4, false, monocle::FileState::Mounting, monocle::FileMonitorState::Unavailable) }),
  testreceivers_(std::vector<RECEIVER>({ RECEIVER(0, ReceiverMode::AlwaysConnect, "mediauri", true, std::string("username1"), std::string("password1"), { std::string("parameters1") }, ReceiverState::Connecting), RECEIVER(1, ReceiverMode::AlwaysConnect, "mediauri2", false, std::string("username2"), std::string("password2"), { std::string("parameters2") }, ReceiverState::NotConnected) })),
  testrecordings_({ RECORDING(0,
                              "sourceid",
                              "name",
                              "location",
                              "description",
                              "address",
                              "content",
                              5,
                              {
                                RECORDINGJOB(0,
                                             "name",
                                             false,
                                             0,
                                             {
                                               RECORDINGJOBSOURCE(0,
                                                                  RecordingJobSourceType::Media,
                                                                  1,
                                                                  {
                                                                    RECORDINGJOBSOURCETRACK(0,
                                                                                            1,
                                                                                            { "parameters" },
                                                                                            RecordingJobState::Error,
                                                                                            "Error",
                                                                                            { "activeparameters" })
                                                                  })
                                             })
                              },
                              {
                                RECORDINGTRACK(0, "token", TrackType::Video, "description", true, true, true, 5, { 0 }, { monocle::INDEX(1, 2) }, { monocle::CODECINDEX(1, monocle::Codec::H265, "parameters", 2) }),
                                RECORDINGTRACK(0, "token", TrackType::Metadata, "description", false, false, false, 6, { 1 }, { monocle::INDEX(2, 3) }, { })
                              },
                              5)
    }),
  testrecordinglogmessages_({ LOGMESSAGE(0, monocle::Severity::Critical, "Critical1"), LOGMESSAGE(1, monocle::Severity::Err, "Err"), LOGMESSAGE(1, monocle::Severity::Debug, "Debug"), LOGMESSAGE(3, monocle::Severity::Info, "Info"), LOGMESSAGE(4, monocle::Severity::Critical, "Critical2") }),
  testmaxrecordings_(8),
  testmaps_({ MAP(0, "name", "location", "imagemd5") }),
  testmountpoints_({ MOUNTPOINT(0, 1, 2, 3, "path", "type", "source") }),
  testlatitude_("latitude"),
  testlongitude_("longitude"),
  testnumcudadevices_(5),
  testnumcldevices_(6),
  testmaxobjectdetectors_(7),
  testframe_(1, 2, 3, 4, 5, utility::GenerateRandomData<uint8_t>(50), 6, 7, 8, 9, 10, 11, nullptr, nullptr, utility::GenerateRandomData<char>(50)),
  client_(boost::make_shared<client::tests::TestClient>(ioservicepool.GetIoService())),
  server_(boost::make_shared<server::tests::TestServer>(ioservicepool, TESTUSERNAME, TESTPASSWORD, TESTNONCE, teststream_, testname_, testpublickey_, testarchitecture_, testoperatingsystem_, testcompiler_, testdatabasepath_, testversion_, testidentifier_, testenvironmentvariables_, testcommandlinevariables_, testonvifusers_, testgroups_, testusers_, testfiles_, testreceivers_, testrecordings_, testrecordinglogmessages_, testmaxrecordings_, testmaps_, testmountpoints_, testlatitude_, testlongitude_, testnumcudadevices_, testnumcldevices_, testmaxobjectdetectors_, testframe_))
{
  
}

TestProtocol::~TestProtocol()
{

}

void TestProtocol::SetUp()
{
  ASSERT_FALSE(server_->Init({ PORT }, std::vector<uint16_t>()));
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  sock::Connection connection = client_->Init(sock::ProxyParams(), std::string("127.0.0.1"), PORT, false, [&promise](const boost::system::error_code& err) { ASSERT_FALSE(err); promise.set_value(); });
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(WAITMS)));
  future.get();
  ASSERT_EQ(monocle::ErrorCode::Success, client_->Keepalive().get().GetErrorCode());
}

void TestProtocol::TearDown()
{
  client_->Destroy();
  server_->Destroy();
}

///// Tests /////

TEST_F(TestProtocol, AddFile)
{
  const monocle::client::ADDFILERESPONSE addfile = client_->AddFile("mountpoint", "path", true, 16, 16 * 1024 * 1024, true).get();
  ASSERT_EQ(monocle::ErrorCode::Success, addfile.GetErrorCode());
}

TEST_F(TestProtocol, Authentication)
{
  const monocle::client::GETAUTHENTICATIONNONCERESPONSE authenticatenonce = client_->GetAuthenticationNonce().get();
  ASSERT_EQ(monocle::ErrorCode::Success, authenticatenonce.GetErrorCode());
  ASSERT_EQ(monocle::ErrorCode::Success, client_->Authenticate(TESTUSERNAME, TESTNONCE, monocle::AuthenticateDigest(TESTUSERNAME, TESTPASSWORD, authenticatenonce.authenticatenonce_)).get().GetErrorCode());
}

TEST_F(TestProtocol, AuthenticationCallback)
{
  const monocle::client::GETAUTHENTICATIONNONCERESPONSE authenticatenonce = client_->GetAuthenticationNonce().get();
  ASSERT_EQ(monocle::ErrorCode::Success, authenticatenonce.GetErrorCode());
  std::promise<monocle::client::AUTHENTICATERESPONSE> promise;
  std::future<monocle::client::AUTHENTICATERESPONSE> future = promise.get_future();
  monocle::client::Connection connection = client_->Authenticate(TESTUSERNAME, TESTNONCE, monocle::AuthenticateDigest(TESTUSERNAME, TESTPASSWORD, authenticatenonce.authenticatenonce_), [&promise](const std::chrono::steady_clock::duration latency, const monocle::client::AUTHENTICATERESPONSE& authenticate) { promise.set_value(authenticate); });
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(WAITMS)));
  future.get();
}

TEST_F(TestProtocol, BadAuthentication)
{
  const monocle::client::GETAUTHENTICATIONNONCERESPONSE authenticatenonce = client_->GetAuthenticationNonce().get();
  ASSERT_EQ(monocle::ErrorCode::Success, authenticatenonce.GetErrorCode());
  ASSERT_NE(monocle::ErrorCode::Success, client_->Authenticate("badusername", TESTNONCE, monocle::AuthenticateDigest(TESTUSERNAME, TESTPASSWORD, authenticatenonce.authenticatenonce_)).get().GetErrorCode());
}

TEST_F(TestProtocol, ControlStream)
{
  const monocle::client::CONTROLSTREAMRESPONSE controlstream = client_->ControlStream(0, 1, false, false, true, boost::none, 0, 1, false).get();
  ASSERT_EQ(monocle::ErrorCode::Success, controlstream.GetErrorCode());
}

TEST_F(TestProtocol, CreateStream)
{
  const monocle::client::CREATESTREAMRESPONSE createstream = client_->CreateStream(0, 1).get();
  ASSERT_EQ(monocle::ErrorCode::Success, createstream.GetErrorCode());
  ASSERT_EQ(teststream_.token_, createstream.token_);

  for (const monocle::CODECINDEX& codecindex : teststream_.codecindices_)
  {
    ASSERT_NE(createstream.codecindices_.cend(), std::find_if(createstream.codecindices_.cbegin(), createstream.codecindices_.cend(), [&codecindex](const monocle::CODECINDEX& ci) { return (ci.id_ == codecindex.id_); }));
  }
}

TEST_F(TestProtocol, Disconnected)
{
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  std::mutex disconnectedmutex;
  bool disconnected = false;
  client_->SetDisconnectedCallback([this, &promise, &disconnectedmutex, &disconnected]()
  {
    try
    {
      std::lock_guard<std::mutex> lock(disconnectedmutex);
      if (disconnected == false)
      {
        promise.set_value();
        disconnected = true;
      }
    }
    catch (...)
    {

    }
  });
  server_->Destroy();

  boost::unique_future<client::KEEPALIVERESPONSE> keepalive1 = client_->Keepalive();
  boost::unique_future<client::KEEPALIVERESPONSE> keepalive2 = client_->Keepalive();
  boost::unique_future<client::KEEPALIVERESPONSE> keepalive3 = client_->Keepalive();
  boost::unique_future<client::KEEPALIVERESPONSE> keepalive4 = client_->Keepalive();
  boost::unique_future<client::KEEPALIVERESPONSE> keepalive5 = client_->Keepalive();
  boost::unique_future<client::KEEPALIVERESPONSE> keepalive6 = client_->Keepalive();

  ASSERT_TRUE((ErrorCode::OperationCancelled == keepalive1.get().GetErrorCode()) || (ErrorCode::Disconnected == keepalive1.get().GetErrorCode()));
  utility::Sleep(std::chrono::milliseconds(1000));
  ASSERT_TRUE((ErrorCode::OperationCancelled == keepalive2.get().GetErrorCode()) || (ErrorCode::Disconnected == keepalive2.get().GetErrorCode()));
  ASSERT_TRUE((ErrorCode::OperationCancelled == keepalive3.get().GetErrorCode()) || (ErrorCode::Disconnected == keepalive3.get().GetErrorCode()));
  ASSERT_TRUE((ErrorCode::OperationCancelled == keepalive4.get().GetErrorCode()) || (ErrorCode::Disconnected == keepalive4.get().GetErrorCode()));
  ASSERT_TRUE((ErrorCode::OperationCancelled == keepalive5.get().GetErrorCode()) || (ErrorCode::Disconnected == keepalive5.get().GetErrorCode()));
  ASSERT_TRUE((ErrorCode::OperationCancelled == keepalive6.get().GetErrorCode()) || (ErrorCode::Disconnected == keepalive6.get().GetErrorCode()));

  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::seconds(client::Client::GetDefaultTimeout() * 2)));
  future.get();
}

TEST_F(TestProtocol, Frame)
{
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client_->SetMJPEGFrameCallback([this, &promise](const server::tests::TESTFRAME& testframe)
  {
    ASSERT_EQ(testframe_, testframe);
    promise.set_value();
  });
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(WAITMS)));
  future.get();
}

TEST_F(TestProtocol, GetFiles)
{
  const monocle::client::GETFILESRESPONSE getfiles = client_->GetFiles().get();
  ASSERT_EQ(monocle::ErrorCode::Success, getfiles.GetErrorCode());
  ASSERT_TRUE(std::is_permutation(testfiles_.cbegin(), testfiles_.cend(), getfiles.files_.cbegin(), getfiles.files_.cend()));
}

TEST_F(TestProtocol, GetReceivers)
{
  const monocle::client::GETRECEIVERSRESPONSE getreceivers = client_->GetReceivers().get();
  ASSERT_EQ(monocle::ErrorCode::Success, getreceivers.GetErrorCode());
  ASSERT_TRUE(std::is_permutation(testreceivers_.cbegin(), testreceivers_.cend(), getreceivers.receivers_.cbegin(), getreceivers.receivers_.cend()));
}

TEST_F(TestProtocol, GetRecordings)
{
  const monocle::client::GETRECORDINGSRESPONSE getrecordings = client_->GetRecordings().get();
  ASSERT_EQ(monocle::ErrorCode::Success, getrecordings.GetErrorCode());
  ASSERT_TRUE(std::is_permutation(testrecordings_.cbegin(), testrecordings_.cend(), getrecordings.recordings_.cbegin(), getrecordings.recordings_.cend()));
}

TEST_F(TestProtocol, GetTime)
{
  const monocle::client::GETTIMERESPONSE gettimeresponse = client_->GetTime().get();
  ASSERT_EQ(monocle::ErrorCode::Success, gettimeresponse.GetErrorCode());
}

TEST_F(TestProtocol, Keepalive)
{
  const monocle::client::KEEPALIVERESPONSE keepaliveresponse = client_->Keepalive().get();
  ASSERT_EQ(monocle::ErrorCode::Success, keepaliveresponse.GetErrorCode());
}

TEST_F(TestProtocol, Subscribe)
{
  const monocle::client::SUBSCRIBERESPONSE subscriberesponse = client_->Subscribe().get();
  ASSERT_EQ(monocle::ErrorCode::Success, subscriberesponse.GetErrorCode());
  ASSERT_EQ(testname_, subscriberesponse.name_);
  ASSERT_TRUE(std::is_permutation(testgroups_.cbegin(), testgroups_.cend(), subscriberesponse.groups_.cbegin(), subscriberesponse.groups_.cend()));
  ASSERT_TRUE(std::is_permutation(testusers_.cbegin(), testusers_.cend(), subscriberesponse.users_.cbegin(), subscriberesponse.users_.cend()));
  ASSERT_TRUE(std::is_permutation(testfiles_.cbegin(), testfiles_.cend(), subscriberesponse.files_.cbegin(), subscriberesponse.files_.cend()));
  ASSERT_TRUE(std::is_permutation(testreceivers_.cbegin(), testreceivers_.cend(), subscriberesponse.receivers_.cbegin(), subscriberesponse.receivers_.cend()));
  ASSERT_TRUE(std::is_permutation(testrecordings_.cbegin(), testrecordings_.cend(), subscriberesponse.recordings_.cbegin(), subscriberesponse.recordings_.cend()));

  const uint64_t TOKEN = 1;
  const monocle::ReceiverMode RECEIVERMODE = ReceiverMode::AutoConnect;
  const std::string MEDIAURI("mediauri");
  const bool AUTOCREATED = true;
  const monocle::ReceiverState RECEIVERSTATE = ReceiverState::Connected;
  const std::vector<std::string> PARAMETERS = { std::string("parameter") };
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client_->SetReceiverChangedCallback([&](const uint64_t token, const monocle::ReceiverMode mode, const std::string& mediauri, const bool autocreated, const std::string& username, const std::string& password, const std::vector<std::string>& parameters) mutable
  {
    ASSERT_TRUE(TOKEN == token);
    ASSERT_TRUE(RECEIVERMODE == mode);
    ASSERT_TRUE(MEDIAURI == mediauri);
    ASSERT_TRUE(AUTOCREATED == autocreated);
    ASSERT_TRUE(PARAMETERS == parameters);
    promise.set_value();
  });
  for (boost::shared_ptr<server::Connection>& connection : server_->GetConnections())
  {
    ASSERT_FALSE(reinterpret_cast<server::tests::TestConnection*>(connection.get())->SendReceiverChanged(TOKEN, RECEIVERMODE, MEDIAURI, AUTOCREATED, std::string(), std::string(), PARAMETERS));

  }
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(WAITMS)));
  future.get();
}

TEST_F(TestProtocol, SubscribeRecordingLog)
{
  const uint64_t recordingtoken = 1;
  const std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
  const monocle::Severity severity = monocle::Severity::Trace;
  const std::string message = "Trace";

  const monocle::client::SUBSCRIBERECORDINGLOGRESPONSE subscriberecordinglogresponse = client_->SubscribeRecordingLog(recordingtoken).get();
  ASSERT_EQ(monocle::ErrorCode::Success, subscriberecordinglogresponse.GetErrorCode());
  ASSERT_EQ(testrecordinglogmessages_, subscriberecordinglogresponse.messages_);

  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client_->SetRecordingLogMessageCallback([&](const uint64_t precordingtoken, const uint64_t ptime, const monocle::Severity pseverity, const std::string& pmessage)
  {
    ASSERT_TRUE(recordingtoken == precordingtoken);
    ASSERT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count() == ptime);
    ASSERT_TRUE(severity == pseverity);
    ASSERT_TRUE(message == pmessage);
    promise.set_value();
  });

  for (boost::shared_ptr<server::Connection>& connection : server_->GetConnections())
  {
    ASSERT_FALSE(connection.get()->SendRecordingLogMessage(recordingtoken, time, severity, message));

  }
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(WAITMS)));
  future.get();
}

TEST_F(TestProtocol, RecordingTrackStateChanged)
{
  const uint64_t recordingtoken = 1;
  const uint64_t recordingjobtoken = 2;
  const uint64_t recordingjobsourcetoken = 3;
  const uint64_t recordingjobsourcetracktoken = 4;
  const uint64_t tracktoken = 1;
  const uint64_t tracktime = 2;
  const RecordingJobState trackstate = RecordingJobState::Active;
  const std::string error("error");
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client_->SetRecordingTrackStateChangedCallback([recordingtoken, recordingjobtoken, recordingjobsourcetoken, recordingjobsourcetracktoken, tracktoken, tracktime, trackstate, &promise](const uint64_t recording, const uint64_t recordingjob, const uint64_t recordingjobsource, const uint64_t recordingjobsourcetrack, const uint64_t time, const RecordingJobState state)
  {
    ASSERT_TRUE(recordingtoken == recording);
    ASSERT_TRUE(recordingjobtoken == recordingjob);
    ASSERT_TRUE(recordingjobsourcetoken == recordingjobsource);
    ASSERT_TRUE(recordingjobsourcetracktoken == recordingjobsourcetrack);
    ASSERT_TRUE(tracktime == time);
    ASSERT_TRUE(trackstate == state);
    promise.set_value();
  });
  for (boost::shared_ptr<server::Connection>& connection : server_->GetConnections())
  {
    ASSERT_FALSE(reinterpret_cast<server::tests::TestConnection*>(connection.get())->SendRecordingJobSourceTrackStateChanged(recordingtoken, recordingjobtoken, recordingjobsourcetoken, recordingjobsourcetracktoken, tracktime, trackstate, error));

  }
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(5000)));
  future.get();
}

TEST_F(TestProtocol, TrackDeleteData)
{
  const uint64_t recording = 1;
  const uint32_t trackid = 2;
  const boost::optional<uint64_t> start = 3;
  const boost::optional<uint64_t> end = 4;
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client_->SetTrackDeleteDataCallback([recording, trackid, start, end, &promise](const uint64_t responserecording, const uint32_t responsetrackid, const boost::optional<uint64_t>& responsestart, const boost::optional<uint64_t>& responseend)
  {
    ASSERT_TRUE(recording == responserecording);
    ASSERT_TRUE(trackid == responsetrackid);
    ASSERT_TRUE(start == responsestart);
    ASSERT_TRUE(end == responseend);
    promise.set_value();
  });
  for (boost::shared_ptr<server::Connection>& connection : server_->GetConnections())
  {
    ASSERT_FALSE(reinterpret_cast<server::tests::TestConnection*>(connection.get())->SendTrackDeleteData(recording, trackid, start, end));

  }
  ASSERT_EQ(std::future_status::ready, future.wait_for(std::chrono::milliseconds(5000)));
  future.get();
}

}

}
