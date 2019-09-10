// testwidedynamicrange20.cpp
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

TEST(TestWideDynamicRange20, Compare)
{
  Compare(WideDynamicRange20(WIDEDYNAMICMODE_OFF, 4.0f));
}

}

}
