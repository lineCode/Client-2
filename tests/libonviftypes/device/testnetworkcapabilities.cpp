// testnetworkcapabilities.cpp
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

TEST(DeviceTestNetworkCapabilities, Compare)
{
  Compare(device::NetworkCapabilities(true, true, true, true, true, 5, true, 5, true));
  CompareElement(device::NetworkCapabilities(true, true, true, true, true, 5, true, 5, true));
}

}

}
