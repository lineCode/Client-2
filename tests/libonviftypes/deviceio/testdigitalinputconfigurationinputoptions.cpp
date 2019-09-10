// testdigitalinputconfigurationinputoptions.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(DeviceIOTestDigitalInputConfigurationInputOptions, Compare)
{
  Compare(deviceio::DigitalInputConfigurationInputOptions({ DIGITALIDLESTATE_CLOSED, DIGITALIDLESTATE_OPEN }));
}

}

}
