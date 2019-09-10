// testrelativefocusoptions20.cpp
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

TEST(TestRelativeFocusOptions20, Compare)
{
  Compare(RelativeFocusOptions20(FloatRange(5.0f, 6.0f), FloatRange(7.0f, 8.0f)));
}

}

}
