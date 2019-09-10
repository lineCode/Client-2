// testclientstress.hpp
//

#ifndef ID01TGOYUNWNS1P7CZBUMROBP9H8D3TM20
#define ID01TGOYUNWNS1P7CZBUMROBP9H8D3TM20

///// Includes /////

#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>
#include <thread>
#include <utility/utility.hpp>
#include <utility/version.hpp>

#include "main.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Globals /////

const int WAITSECONDS = 10; // Time amount of time in seconds to wait for a response from the device before failing

///// Classes /////

class TestClientStress : public testing::Test
{
 public:

  TestClientStress();
  virtual ~TestClientStress();

 protected:
   
  static void SetUpTestCase();
  static void TearDownTestCase();
    
  static boost::shared_ptr<device::DeviceClient> deviceclient_;
  static Capabilities capabilities_;
  static boost::optional<device::DeviceServiceCapabilities> deviceservicecapabilities_;

  template<class T>
  static void Run(boost::shared_ptr< Client<T> > client)
  {
    while(running_)
    {
      utility::Sleep(std::chrono::milliseconds(10));
      ASSERT_FALSE(client->Update());
    }
  }

 private:

  static std::thread devicethread_;
  static bool running_;

};

}

}

#endif
