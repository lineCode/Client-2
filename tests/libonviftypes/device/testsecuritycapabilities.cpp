// testsecuritycapabilities.cpp
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

TEST(DeviceTestSecurityCapabilities, Compare)
{
  Compare(device::SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, 5, 5, 5));
  CompareElement(device::SecurityCapabilities(true, true, true, true, true, true, true, true, true, true, true, true, true, true, { 5, 6, 7 }, 5, 5, 5));
}

}

}
