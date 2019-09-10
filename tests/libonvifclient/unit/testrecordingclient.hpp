// testrecordingclient.hpp
//

#ifndef IDJR8JY8X5WPQ76D840HTKVUOD4IQGP6YP
#define IDJR8JY8X5WPQ76D840HTKVUOD4IQGP6YP

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyrecordingservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestRecordingClient : public TestClient<onvif::recording::RecordingClient>
{
 public:
  
  TestRecordingClient();
  virtual ~TestRecordingClient();

 protected:
  
  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyRecordingService> dummyrecordingservice_;

 private:
  
};

}

}

#endif
