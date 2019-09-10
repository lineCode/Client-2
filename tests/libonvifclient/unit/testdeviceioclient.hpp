// testdeviceioclient.hpp
//

#ifndef IDYWDCRAFRJP8JY6LFHZZQS4BXGF7PQU67
#define IDYWDCRAFRJP8JY6LFHZZQS4BXGF7PQU67

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>

#include "dummydeviceioservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestDeviceIOClient : public TestClient<onvif::deviceio::DeviceIOClient>
{
 public:

  TestDeviceIOClient();
  virtual ~TestDeviceIOClient();

 protected:

  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<DummyDeviceIOService> dummydeviceioservice_;

 private:

};

}

}

#endif
