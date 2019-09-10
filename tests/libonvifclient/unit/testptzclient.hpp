// testptzclient.hpp
//

#ifndef IDYW4HLZWK6SBQMG7J6IARWNRK47SOIRF8
#define IDYW4HLZWK6SBQMG7J6IARWNRK47SOIRF8

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyptzservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestPTZClient : public TestClient<onvif::ptz::PTZClient>
{
 public:

  TestPTZClient();
  virtual ~TestPTZClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyPTZService> dummyptzservice_;

 private:

};

}

}

#endif
