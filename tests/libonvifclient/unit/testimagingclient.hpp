// testimagingclient.hpp
//

#ifndef ID2N5B2RSDPH4ZFKSUDGQDWXTA7847TX79
#define ID2N5B2RSDPH4ZFKSUDGQDWXTA7847TX79

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyimagingservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestImagingClient : public TestClient<onvif::imaging::ImagingClient>
{
 public:

  TestImagingClient();
  virtual ~TestImagingClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyImagingService> dummyimagingservice_;

 private:

};

}

}

#endif
