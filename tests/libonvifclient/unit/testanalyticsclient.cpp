// testanalyticsclient.cpp
//

///// Includes /////

#include "testanalyticsclient.hpp"

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
const std::string ANALYTICSURI("/onvif/analytics");

///// Methods /////

TestAnalyticsClient::TestAnalyticsClient()
{
  
}

TestAnalyticsClient::~TestAnalyticsClient()
{
  
}

void TestAnalyticsClient::SetUp()
{
  Init(PORT, ANALYTICSURI);

  // Setup dummy server
  dummyanalyticsservice_ = std::make_unique<DummyAnalyticsService>();
  ASSERT_FALSE(httpserver_.AddServer(ANALYTICSURI, dummyanalyticsservice_.get()));
}

void TestAnalyticsClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(ANALYTICSURI));
  dummyanalyticsservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestAnalyticsClient, GetServiceCapabilities)
{
  const analytics::Capabilities capabilities(true, true, true);
  dummyanalyticsservice_->SetGetServiceCapabilities(capabilities);

  analytics::GetServiceCapabilitiesResponse getanalyticsservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getanalyticsservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getanalyticsservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getanalyticsservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getanalyticsservicecapabilitiesresponse.capabilities_);
}

}

}
