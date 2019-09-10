// testdeviceclient.hpp
//

#ifndef IDK44AZWIDJALFJHIBL5UY2FSRS53ZNNDO
#define IDK44AZWIDJALFJHIBL5UY2FSRS53ZNNDO

///// Includes /////

#include <gtest/gtest.h>
#include <onvifclient/onvifclient.hpp>
#include <memory>
#include <thread>

#include "dummydeviceservice.hpp"
#include "testclient.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Classes /////

class TestDeviceClient : public TestClient<onvif::device::DeviceClient>
{
  public:

    TestDeviceClient();
    virtual ~TestDeviceClient();

  protected:

    virtual void SetUp();
    virtual void TearDown();

    std::unique_ptr<DummyDeviceService> dummydeviceservice_;

  private:

};

}

}

#endif
