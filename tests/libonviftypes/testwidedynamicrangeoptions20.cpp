// testwidedynamicrangeoptions20.cpp
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

TEST(TestWideDynamicRangeOptions20, Compare)
{
  Compare(WideDynamicRangeOptions20({ WIDEDYNAMICMODE_ON, WIDEDYNAMICMODE_OFF }, FloatRange(5.0f, 6.0f)));
}

}

}
