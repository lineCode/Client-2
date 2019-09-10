// testreceiverclient.hpp
//

#ifndef IDJR8JY8X5WPQ76D840HTKVUOD4IQGP6YP
#define IDJR8JY8X5WPQ76D840HTKVUOD4IQGP6YP

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummyreceiverservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestReceiverClient : public TestClient<onvif::receiver::ReceiverClient>
{
 public:
  
  TestReceiverClient();
  virtual ~TestReceiverClient();

 protected:
  
  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyReceiverService> dummyreceiverservice_;

 private:
  
};

}

}

#endif
