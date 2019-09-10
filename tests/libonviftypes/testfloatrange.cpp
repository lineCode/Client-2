// testfloatrange.cpp
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

TEST(TestFloatRange, Compare)
{
  Compare(FloatRange(1.0f, 2.0f));
}

}

}
