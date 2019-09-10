// testcontinuousfocusoptions.cpp
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

TEST(TestContinuousFocusOptions, Compare)
{
  Compare(ContinuousFocusOptions(FloatRange(7.0f, 8.0f)));
}

}

}
