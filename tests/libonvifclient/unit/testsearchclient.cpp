// testsearchclient.cpp
//

///// Includes /////

#include "testsearchclient.hpp"

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
const std::string SEARCHURI("/onvif/search");

///// Methods /////

TestSearchClient::TestSearchClient()
{
  
}

TestSearchClient::~TestSearchClient()
{
  
}

void TestSearchClient::SetUp()
{
  Init(PORT, SEARCHURI);

  // Setup dummy server
  dummysearchservice_ = std::make_unique<DummySearchService>();
  ASSERT_FALSE(httpserver_.AddServer(SEARCHURI, dummysearchservice_.get()));
}

void TestSearchClient::TearDown()
{
  ASSERT_FALSE(httpserver_.RemoveServer(SEARCHURI));
  dummysearchservice_.reset();

  Destroy();
}

///// Tests /////

TEST_F(TestSearchClient, GetServiceCapabilities)
{
  const search::Capabilities capabilities(true, true);
  dummysearchservice_->SetGetServiceCapabilities(capabilities);

  search::GetServiceCapabilitiesResponse getsearchservicecapabilitiesresponse = client_->GetServiceCapabilitiesFuture().get();
  ASSERT_GE(getsearchservicecapabilitiesresponse.latency_, 0);
  ASSERT_FALSE(getsearchservicecapabilitiesresponse.Error());
  ASSERT_TRUE(getsearchservicecapabilitiesresponse.capabilities_.is_initialized());
  ASSERT_EQ(capabilities, *getsearchservicecapabilitiesresponse.capabilities_);
}

}

}
