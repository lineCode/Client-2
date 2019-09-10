// testdeviceservice.hpp
//

#ifndef IDG0VWOSGTI0L7NH1G1SZ5OMXO50OI6P4M
#define IDG0VWOSGTI0L7NH1G1SZ5OMXO50OI6P4M

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <onvifserver/httpserver.hpp>
#include <thread>

#include "dummyservice.hpp"
#include "dummydeviceservice.hpp"

///// Namespaces /////

namespace onvif
{

namespace server
{

namespace tests
{

///// Classes /////

class TestDeviceService : public testing::Test
{
 public:

  TestDeviceService();
  virtual ~TestDeviceService();

 protected:

  static void SetUpTestCase();
  static void TearDownTestCase();

  virtual void SetUp();
  virtual void TearDown();

  static ClientThread<onvif::device::DeviceClient> deviceclient_;

  static const std::string username_;
  static const onvif::USERLEVEL userlevel_;
  static const std::string password_;

  HttpServer httpserver_;
  boost::shared_ptr<DummyDeviceService> dummydeviceservice_;
  boost::shared_ptr<DummyService> dummywsdlservice_;

 private:

  void Run();

  std::thread httpserverthread_;
  bool run_;


};

}

}

}

#endif
