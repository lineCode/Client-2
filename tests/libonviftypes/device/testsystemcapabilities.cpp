 // testsystemcapabilities.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "../testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(DeviceTestSystemCapabilities, Compare)
{
  Compare(device::SystemCapabilities(true, true, true, true, true, true, true, true, true, true, true));
  CompareElement(device::SystemCapabilities(true, true, true, true, true, true, true, true, true, true, true));
}

}

}
