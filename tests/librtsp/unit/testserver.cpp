// testserver.cpp
//

///// Includes /////

#include "testserver.hpp"

#include <boost/make_shared.hpp>
#include <exception>
#include <future>
#include <stdint.h>
#include <utility/ioservicepool.hpp>

///// Namespaces /////

namespace rtsp
{

namespace server
{

namespace tests
{

///// Globals /////

const boost::posix_time::milliseconds TIMEOUT(30000);

const std::string PATH("path");
const std::string REALM("realm");
const uint16_t HTTPPORT = 8888;
const uint16_t PORT = 5554;
const std::string USERNAME("username");
const std::string PASSWORD("password");
const std::string FAKESESSION("fakesession"); // Used to give easy responses for the test server

///// Declarations /////

utility::IoServicePool TestServer::ioservicepool_(4, 4, []() {}, []() {});
const std::string TestServer::url_(std::string("rtsp://127.0.0.1/") + PATH);
boost::shared_ptr< Client<TestServer> > TestServer::client_;
boost::shared_ptr<StreamTest> TestServer::stream_;
boost::shared_ptr<Server> TestServer::server_;

///// Functions /////

void RtpCallback(const boost::system::error_code& err, void* object, void* data, const boost::asio::ip::address& address, const rtsp::rtp::Packet& rtppacket, const uint8_t* payload, unsigned int payloadsize)
{
  if (err)
  {

    return;
  }

  try
  {
    (reinterpret_cast< std::promise<void>* >(data))->set_value();

  }
  catch (...)
  {
    // Ignore if we have already set the value

  }
}

///// Methods /////

TestServer::TestServer()
{

}

TestServer::~TestServer()
{

}

void TestServer::SetUpTestCase()
{
  // Initialise server
  server_ = boost::make_shared<Server>(REALM, ioservicepool_);
  ASSERT_FALSE(server_->Init(boost::asio::ip::tcp::v4(), { PORT }, { HTTPPORT }));
  stream_ = boost::make_shared<StreamTest>(server_, PATH, std::set<headers::REQUESTTYPE>({ headers::REQUESTTYPE_DESCRIBE, headers::REQUESTTYPE_SETUP, headers::REQUESTTYPE_GETPARAMETER, headers::REQUESTTYPE_SETPARAMETER, headers::REQUESTTYPE_PLAY, headers::REQUESTTYPE_PAUSE, headers::REQUESTTYPE_TEARDOWN }), false, true, REALM, USERNAME, PASSWORD, FAKESESSION);
  server_->AddStream(stream_);

  // Connect to server
  std::promise<void> promise;
  std::future<void> future = promise.get_future();
  client_ = boost::make_shared< Client<TestServer> >(nullptr, boost::make_shared<std::recursive_mutex>(), ioservicepool_.GetIoService(), boost::posix_time::seconds(10), boost::posix_time::seconds(60));
  client_->Init(sock::ProxyParams(), std::string("127.0.0.1"), PORT, USERNAME, PASSWORD);
  sock::Connection connection = client_->Connect([&promise](const boost::system::error_code& err)
  {
    EXPECT_FALSE(err);
    ASSERT_NO_THROW(promise.set_value());
  }, [](){});
  future.get();
}

void TestServer::TearDownTestCase()
{
  client_->Disconnect();
  client_->Destroy();
  client_.reset();

  server_->RemoveStream(stream_);
  stream_->Destroy();
  stream_.reset();
  server_->Destroy();
  server_.reset();

  ioservicepool_.Destroy();
}

void TestServer::SetUp()
{

}

void TestServer::TearDown()
{

}

///// Tests /////

TEST_F(TestServer, BasicAuthentication)
{
  stream_->SetDigestAuthentication(false);
  ASSERT_NO_THROW(client_->DescribeFuture(url_).get());
  stream_->SetDigestAuthentication(true);
}

TEST_F(TestServer, Options)
{
  ASSERT_NO_THROW(client_->OptionsFuture(url_).get());

}

TEST_F(TestServer, Describe)
{
  ASSERT_NO_THROW(client_->DescribeFuture(url_).get());

}

TEST_F(TestServer, GetParameter)
{
  const GetParameterResponse getparameterresponse = client_->GetParameterFuture(url_, FAKESESSION, { "key1" }).get();
  ASSERT_EQ(getparameterresponse.parameters_, std::vector<headers::Parameter>({ headers::Parameter("key1", "value1") }));
}

TEST_F(TestServer, SetParameter)
{
  const SetParameterResponse setparameterresponse = client_->SetParameterFuture(url_, FAKESESSION, { headers::Parameter("key1", "value1") }).get();
  ASSERT_EQ(setparameterresponse.parameters_, std::vector<headers::Parameter>({ headers::Parameter("key1", std::string()) }));
}

TEST_F(TestServer, Play)
{
  ASSERT_NO_THROW(client_->PlayFuture(url_, FAKESESSION, headers::Range(true, 5, 6)).get());

}

TEST_F(TestServer, Pause)
{
  ASSERT_NO_THROW(client_->PauseFuture(url_, FAKESESSION).get());

}

TEST_F(TestServer, Teardown)
{
  ASSERT_NO_THROW(client_->TeardownFuture(url_, FAKESESSION).get());

}

TEST_F(TestServer, BadUrl)
{
  ASSERT_ANY_THROW(client_->OptionsFuture("badurl").get());

}

TEST_F(TestServer, BadUsername)
{
  client_->SetUsernamePassword("badusername", "badpassword");
  ASSERT_THROW(client_->DescribeFuture(url_).get(), boost::system::system_error);
  client_->SetUsernamePassword(USERNAME, PASSWORD);
}

TEST_F(TestServer, Rtp)
{
  std::promise<void> interleavedpromise;
  std::promise<void> udpunicastpromise;
  std::future<void> interleavedfuture = interleavedpromise.get_future();
  std::future<void> udpunicastfuture = udpunicastpromise.get_future();
  
  const rtsp::SetupResponse interleavedsetupresponse = client_->SetupFuture(url_, sdp::ADDRESSTYPE_IP4, headers::PROTOCOLTYPE_TCP, headers::ROUTINGTYPE_UNICAST, headers::MODE_PLAY, 1.0, KEEPALIVEMODE_GETPARAMETER, std::string(), RtpCallback, &interleavedpromise, nullptr).get();
  const rtsp::SetupResponse udpunicastsetupresponse = client_->SetupFuture(url_, sdp::ADDRESSTYPE_IP4, headers::PROTOCOLTYPE_UDP, headers::ROUTINGTYPE_UNICAST, headers::MODE_PLAY, 1.0, KEEPALIVEMODE_GETPARAMETER, std::string(), RtpCallback, &udpunicastpromise, nullptr).get();
  
  ASSERT_NO_THROW(client_->PlayFuture(url_, interleavedsetupresponse.session_, headers::Range(true, 0, boost::none)).get());
  ASSERT_EQ(interleavedfuture.wait_for(std::chrono::milliseconds(10000)), std::future_status::ready);
  ASSERT_NO_THROW(interleavedfuture.get());
  ASSERT_NO_THROW(client_->TeardownFuture(url_, interleavedsetupresponse.session_).get());

  ASSERT_NO_THROW(client_->PlayFuture(url_, udpunicastsetupresponse.session_, headers::Range(true, 0, boost::none)).get());
  ASSERT_EQ(udpunicastfuture.wait_for(std::chrono::milliseconds(10000)), std::future_status::ready);
  ASSERT_NO_THROW(udpunicastfuture.get());
  ASSERT_NO_THROW(client_->TeardownFuture(url_, udpunicastsetupresponse.session_).get());
}

}

}

}
