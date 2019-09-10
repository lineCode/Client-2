// testreceiverclient.cpp
//

///// Includes /////

#include "testreceiverclient.hpp"

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
const std::string RECEIVERURI("/onvif/receiver");

///// Methods /////

TestReceiverClient::TestReceiverClient()
{
  
}

TestReceiverClient::~TestReceiverClient()
{
  
}

void TestReceiverClient::SetUp()
{
  Init(PORT, RECEIVERURI);

  // Setup dummy server
  dummyreceiverservice_ = std::make_unique<DummyReceiverService>();
  ASSERT_FALSE(httpserver_.AddServer(RECEIVERURI, dummyreceiverservice_.get()));
}

void TestReceiverClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(RECEIVERURI));
  dummyreceiverservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestReceiverClient, ConfigureReceiver)
{
  const std::string receivertoken("receivertoken");
  const receiver::ReceiverConfiguration configuration(receiver::RECEIVERMODE_ALWAYSCONNECT, std::string("mediauri"), StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP))));
  dummyreceiverservice_->SetConfigureReceiver(receivertoken, configuration);

  receiver::ConfigureReceiverResponse configurereceiverresponse = client_->ConfigureReceiverFuture(receivertoken, configuration).get();
  ASSERT_GE(configurereceiverresponse.latency_, 0);
  ASSERT_FALSE(configurereceiverresponse.Error());
  ASSERT_EQ(receivertoken, configurereceiverresponse.receivertoken_);
}

TEST_F(TestReceiverClient, CreateReceiver)
{
  const receiver::ReceiverConfiguration configuration(receiver::RECEIVERMODE_ALWAYSCONNECT, std::string("mediauri"), StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP))));
  const receiver::Receiver receiver(std::string("token"), configuration);
  dummyreceiverservice_->SetCreateReceiver(receiver);

  receiver::CreateReceiverResponse createreceiverresponse = client_->CreateReceiverFuture(configuration).get();
  ASSERT_GE(createreceiverresponse.latency_, 0);
  ASSERT_FALSE(createreceiverresponse.Error());
  ASSERT_EQ(configuration, createreceiverresponse.configuration_);
  ASSERT_EQ(receiver, *createreceiverresponse.receiver_);
}

TEST_F(TestReceiverClient, DeleteReceiver)
{
  const std::string receivertoken(std::string("ReceiverToken"));

  receiver::DeleteReceiverResponse deletereceiverresponse = client_->DeleteReceiverFuture(receivertoken).get();
  ASSERT_GE(deletereceiverresponse.latency_, 0);
  ASSERT_FALSE(deletereceiverresponse.Error());
  ASSERT_EQ(receivertoken, deletereceiverresponse.receivertoken_);
}

TEST_F(TestReceiverClient, GetReceiver)
{
  const std::string receivertoken("ReceiverToken");
  const receiver::Receiver receiver(receivertoken, receiver::ReceiverConfiguration(receiver::RECEIVERMODE_ALWAYSCONNECT, std::string("mediauri"), StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP)))));
  dummyreceiverservice_->SetGetReceiver(receiver);

  receiver::GetReceiverResponse getreceiverresponse = client_->GetReceiverFuture(receivertoken).get();
  ASSERT_GE(getreceiverresponse.latency_, 0);
  ASSERT_FALSE(getreceiverresponse.Error());
  ASSERT_EQ(receivertoken, getreceiverresponse.receivertoken_);
  ASSERT_EQ(receiver, *getreceiverresponse.receiver_);
}

TEST_F(TestReceiverClient, GetReceivers)
{
  const std::vector<receiver::Receiver> receivers =
  {
    receiver::Receiver(std::string("ReceiverToken"), receiver::ReceiverConfiguration(receiver::RECEIVERMODE_ALWAYSCONNECT, std::string("mediauri"), StreamSetup(STREAM_RTPMULTICAST, Transport(TRANSPORTPROTOCOL_HTTP, Transport(TRANSPORTPROTOCOL_RTSP)))))

  };
  dummyreceiverservice_->SetGetReceivers(receivers);

  receiver::GetReceiversResponse getreceiversresponse = client_->GetReceiversFuture().get();
  ASSERT_GE(getreceiversresponse.latency_, 0);
  ASSERT_FALSE(getreceiversresponse.Error());
  ASSERT_EQ(receivers, getreceiversresponse.receivers_);
}

TEST_F(TestReceiverClient, GetReceiverState)
{
  const std::string receivertoken("ReceiverToken");
  const receiver::ReceiverStateInformation receiverstate(receiver::RECEIVERSTATE_CONNECTING, true);
  dummyreceiverservice_->SetGetReceiverState(receiverstate);

  receiver::GetReceiverStateResponse getreceiverstateresponse = client_->GetReceiverStateFuture(receivertoken).get();
  ASSERT_GE(getreceiverstateresponse.latency_, 0);
  ASSERT_FALSE(getreceiverstateresponse.Error());
  ASSERT_EQ(receivertoken, getreceiverstateresponse.receivertoken_);
  ASSERT_EQ(receiverstate, *getreceiverstateresponse.receiverstate_);
}

TEST_F(TestReceiverClient, GetServiceCapabilities)
{
  const receiver::Capabilities capabilities(true, true, true, 5, 5);
  dummyreceiverservice_->SetGetServiceCapabilities(capabilities);

  receiver::GetServiceCapabilitiesResponse getreceiverservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getreceiverservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getreceiverservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getreceiverservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getreceiverservicecapabilitiesresponse.capabilities_);
}

TEST_F(TestReceiverClient, SetReceiverMode)
{
  const std::string receivertoken("ReceiverToken");
  const receiver::RECEIVERMODE mode = receiver::RECEIVERMODE_ALWAYSCONNECT;

  receiver::SetReceiverModeResponse setreceivermoderesponse = client_->SetReceiverModeFuture(receivertoken, mode).get();
  ASSERT_GE(setreceivermoderesponse.latency_, 0);
  ASSERT_FALSE(setreceivermoderesponse.Error());
  ASSERT_EQ(receivertoken, setreceivermoderesponse.receivertoken_);
  ASSERT_EQ(mode, setreceivermoderesponse.mode_);
}

}

}
