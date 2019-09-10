// testanalyticscapabilities.cpp
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

TEST(TestAnalyticsCapabilities, Compare)
{
  Compare(AnalyticsCapabilities(std::string("xaddr"), true, true));
}

}

}
