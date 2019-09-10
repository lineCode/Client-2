// testmediaclient.hpp
//

#ifndef IDGB8R8TPO31PJPDCXUY1JNE41RVHHZEWC
#define IDGB8R8TPO31PJPDCXUY1JNE41RVHHZEWC

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>
#include <thread>

#include "dummymediaservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestMediaClient : public TestClient<onvif::media::MediaClient>
{
 public:

  TestMediaClient();
  virtual ~TestMediaClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyMediaService> dummymediaservice_;


};

}

}

#endif
