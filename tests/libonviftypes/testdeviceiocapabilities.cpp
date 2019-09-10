// testdeviceiocapabilities.cpp
//

///// Includes /////

#include <onviftypes/onviftypes.hpp>
#include <gtest/gtest.h>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestDeviceIOCapabilities, Compare)
{
  Compare(DeviceIOCapabilities(std::string("xaddr"), 5, 5, 5, 5, 5));
}

}

}
