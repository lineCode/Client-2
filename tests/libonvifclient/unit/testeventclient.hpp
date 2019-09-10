// testeventclient.hpp
//

#ifndef ID394ASAX0PPQ2E77U3AYIY0YBQR1JZ06Q
#define ID394ASAX0PPQ2E77U3AYIY0YBQR1JZ06Q

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyeventservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestEventClient : public TestClient<onvif::event::EventClient>
{
 public:

  TestEventClient();
  virtual ~TestEventClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyEventService> dummyeventservice_;

 private:

};

}

}

#endif
