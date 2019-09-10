// testdurationrange.cpp
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

TEST(TestDurationRange, Compare)
{
  Compare(DurationRange(Duration(0, 1, 2, 3, 4, 5, 0), Duration(1, 2, 3, 4, 5, 6, 0)));
}

}

}
