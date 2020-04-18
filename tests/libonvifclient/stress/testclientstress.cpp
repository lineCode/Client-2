// testclientstress.cpp
//

///// Includes /////

#include "testclientstress.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Declarations /////

boost::shared_ptr<device::DeviceClient> TestClientStress::deviceclient_;
onvif::Capabilities TestClientStress::capabilities_;
boost::optional<device::DeviceServiceCapabilities> TestClientStress::deviceservicecapabilities_;
std::thread TestClientStress::devicethread_;
bool TestClientStress::running_ = false;

///// Methods /////

TestClientStress::TestClientStress()
{

}

TestClientStress::~TestClientStress()
{

}

void TestClientStress::SetUpTestCase()
{
  ASSERT_TRUE(false == running_);

  ASSERT_FALSE(onvif::Init());

  deviceclient_ = boost::make_shared<device::DeviceClient>(boost::make_shared<std::recursive_mutex>());
  ASSERT_FALSE(deviceclient_->Init(g_proxyparams, g_address, g_username, g_password, 0, false, false));
  
  running_ = true;
  devicethread_ = std::thread(boost::bind(&TestClientStress::Run<device::DEVICEOPERATION>, deviceclient_));

  // Block on GetSystemDateAndTime for authentication purposes
  device::GetSystemDateAndTimeResponse getsystemdateandtimeresponse = deviceclient_->GetSystemDateAndTimeFuture().get();
  ASSERT_GE(getsystemdateandtimeresponse.latency_, 0);
  if (getsystemdateandtimeresponse.Message().empty())
  {
    ASSERT_FALSE(getsystemdateandtimeresponse.Error());
    ASSERT_TRUE(getsystemdateandtimeresponse.systemdatetime_.is_initialized());
    ASSERT_TRUE(getsystemdateandtimeresponse.systemdatetime_->datetimetype_.is_initialized());
    ASSERT_TRUE((*getsystemdateandtimeresponse.systemdatetime_->datetimetype_ == DATETIME_MANUAL) || (*getsystemdateandtimeresponse.systemdatetime_->datetimetype_ == DATETIME_NTP));
    ASSERT_TRUE(getsystemdateandtimeresponse.systemdatetime_->utctime_.is_initialized());
    ASSERT_TRUE(getsystemdateandtimeresponse.systemdatetime_->utctime_->GetDateTime());
  
    deviceclient_->SetTimeOffset(getsystemdateandtimeresponse.systemdatetime_->GetUtcOffset());
  }
  
  device::GetCapabilitiesResponse getcapabilitiesresponse = deviceclient_->GetCapabilitiesFuture(onvif::CAPABILITYCATEGORY_ALL).get();
  ASSERT_GE(getcapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getcapabilitiesresponse.Error()) << getcapabilitiesresponse.Message();
  ASSERT_TRUE(getcapabilitiesresponse.capabilities_.is_initialized());
  capabilities_ = *getcapabilitiesresponse.capabilities_;

  if (capabilities_.GetMaxSupportedVersion() >= utility::Version(2, 0, 0))
  {
    device::GetServiceCapabilitiesResponse getservicecapabilitiesresponse = deviceclient_->GetServiceCapabilitiesFuture().get();
    ASSERT_GE(getservicecapabilitiesresponse.latency_, 0);
    ASSERT_FALSE(getservicecapabilitiesresponse.Error()) << getservicecapabilitiesresponse.Message();
    ASSERT_TRUE(getservicecapabilitiesresponse.capabilities_.is_initialized());
    deviceservicecapabilities_ = *getservicecapabilitiesresponse.capabilities_;
  }
}

void TestClientStress::TearDownTestCase()
{
  running_ = false;
  devicethread_.join();
  
  deviceclient_->Destroy();
  deviceclient_ = nullptr;
  
  ASSERT_FALSE(onvif::Destroy());
}

///// Tests /////

TEST_F(TestClientStress, ConcurrentRequests)
{
  deviceclient_->SetMaxConcurrentRequests(1);

  std::vector< boost::unique_future<onvif::device::GetCapabilitiesResponse> > responses;
  for (int i = 0; i < 200; ++i)
  {
    responses.emplace_back(std::move(deviceclient_->GetCapabilitiesFuture(onvif::CAPABILITYCATEGORY_ALL)));

  }

  for (auto& response : responses)
  {
    auto getcapabilitiesresponse = response.get();
    ASSERT_GE(getcapabilitiesresponse.latency_, 0);
    ASSERT_FALSE(getcapabilitiesresponse.Error()) << getcapabilitiesresponse.Message();
  }
}

}

}
