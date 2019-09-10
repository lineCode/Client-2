// testwidedynamicrange.cpp
//

///// Includes /////

#include <gtest/gtest.h>
#include <onviftypes/onviftypes.hpp>

#include "testtypes.hpp"

///// Namespaces /////

namespace onvif
{

namespace tests
{

///// Tests /////

TEST(TestWideDynamicRange, Compare)
{
  Compare(WideDynamicRange(WIDEDYNAMICMODE_OFF, 4.0f));
}

}

}
