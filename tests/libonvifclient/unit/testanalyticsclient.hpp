// testanalyticsclient.hpp
//

#ifndef IDY1DGJIRIDPBOAAFX1MDMZ6IXKDJKQ9JL
#define IDY1DGJIRIDPBOAAFX1MDMZ6IXKDJKQ9JL

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyanalyticsservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestAnalyticsClient : public TestClient<onvif::analytics::AnalyticsClient>
{
 public:

  TestAnalyticsClient();
  virtual ~TestAnalyticsClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyAnalyticsService> dummyanalyticsservice_;

 private:

};

}

}

#endif
