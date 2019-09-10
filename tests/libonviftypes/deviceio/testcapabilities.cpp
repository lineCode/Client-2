// testcapabilities.cpp
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

TEST(DeviceIOTestCapabilities, Compare)
{
  Compare(deviceio::Capabilities(5, 6, 7, 8, 9, 10, 11, true));
}

}

}
