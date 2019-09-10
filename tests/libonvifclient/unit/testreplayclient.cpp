// testreplayclient.cpp
//

///// Includes /////

#include "testreplayclient.hpp"

#include <boost/optional.hpp>
#include <onviftypes/onviftypes.hpp>
#include <string>

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const unsigned short PORT = 8000;
const std::string REPLAYURI("/onvif/replay");

///// Methods /////

TestReplayClient::TestReplayClient()
{
  
}

TestReplayClient::~TestReplayClient()
{
  
}

void TestReplayClient::SetUp()
{
  Init(PORT, REPLAYURI);

  // Setup dummy server
  dummyreplayservice_ = std::make_unique<DummyReplayService>();
  ASSERT_FALSE(httpserver_.AddServer(REPLAYURI, dummyreplayservice_.get()));
}

void TestReplayClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(REPLAYURI));
  dummyreplayservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestReplayClient, GetReplayUri)
{
  const StreamSetup streamsetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_RTSP));
  const std::string recordingtoken("RecordingToken");
  const std::string uri("Uri");
  dummyreplayservice_->SetGetReplayUri(uri);

  replay::GetReplayUriResponse getreplayuriresponse = client_->GetReplayUriFuture(streamsetup, recordingtoken).get();
  ASSERT_GE(getreplayuriresponse.latency_, 0);
  ASSERT_FALSE(getreplayuriresponse.Error());
  ASSERT_EQ(streamsetup, getreplayuriresponse.streamsetup_);
  ASSERT_EQ(recordingtoken, getreplayuriresponse.recordingtoken_);
  ASSERT_EQ(uri, *getreplayuriresponse.uri_);
}

TEST_F(TestReplayClient, GetServiceCapabilities)
{
  const replay::Capabilities capabilities(true, FloatAttrList(std::vector<float>({ 5.0f, 6.0f, 7.0f })), true);
  dummyreplayservice_->SetGetServiceCapabilities(capabilities);

  replay::GetServiceCapabilitiesResponse getreplayservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getreplayservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getreplayservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getreplayservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getreplayservicecapabilitiesresponse.capabilities_);
}

}

}
