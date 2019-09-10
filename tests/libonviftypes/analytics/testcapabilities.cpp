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

TEST(AnalyticsTestCapabilities, Compare)
{
  Compare(analytics::Capabilities(true, true, true));
}

}

}
