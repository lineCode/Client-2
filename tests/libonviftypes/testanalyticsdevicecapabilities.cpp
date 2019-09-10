// testanalyticsdevicecapabilities.cpp
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

TEST(TestAnalyticsDeviceCapabilities, Compare)
{
  Compare(AnalyticsDeviceCapabilities(std::string("xaddr"), true));
}

}

}
