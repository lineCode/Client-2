// testreplayclient.hpp
//

#ifndef ID33IQP6OQ8LSD434P3PGRKMXC57FEY5TC
#define ID33IQP6OQ8LSD434P3PGRKMXC57FEY5TC

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyreplayservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestReplayClient : public TestClient<onvif::replay::ReplayClient>
{
 public:

  TestReplayClient();
  virtual ~TestReplayClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyReplayService> dummyreplayservice_;

 private:

};

}

}

#endif
